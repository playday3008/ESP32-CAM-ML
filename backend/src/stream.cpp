#include "stream.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <esp_log.h>
#include <esp_timer.h>
#include <esp_http_server.h>
#include <img_converters.h>

#include "tools/ra_filter.hpp"
#include "led.hpp"
#include "types/camera.hpp"

#include "error.hpp"

#define PART_BOUNDARY "TotallyRandomBoundaryString123"

static constexpr const char STREAM_CONTENT_TYPE[] =
    "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static constexpr const char STREAM_BOUNDARY[]    = "\r\n--" PART_BOUNDARY "\r\n";
static constexpr const char STREAM_PART[]        = "Content-Type: image/jpeg\r\n"
                                                   "Content-Length: %zu\r\n"
                                                   "X-Timestamp: %ld.%06ld\r\n"
                                                   "X-Framerate: %.1f\r\n"
                                                   "X-Average-Framerate: %.1f\r\n\r\n";
static constexpr size_t     STREAM_PART_FULL_LEN = sizeof(STREAM_PART) * 1.5f;

static constexpr uint8_t FRAME2JPG_QUALITY = 80;

static esp_err_t stream_handler(httpd_req_t *req) {
    esp_err_t    ret = ESP_OK;
    camera_fb_t *fb  = nullptr;
    timeval      timestamp{};
    uint8_t     *buf     = nullptr;
    size_t       buf_len = 0;
    char        *part_buf[STREAM_PART_FULL_LEN];

    int64_t  last_frame     = 0;
    int64_t  frame_time     = 0;
    uint32_t avg_frame_time = 0;

    RaFilter ra_filter{};

    ret = httpd_resp_set_type(req, STREAM_CONTENT_TYPE);
    if (ret != ESP_OK) {
        log_e("Failed to set content type, err: %d", ret);
        blink_error<ERR_STREAM_SERVER>(ERR_STREAM_SET_CT, true);
        return ret;
    }

    ret = httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    if (ret != ESP_OK) {
        log_e("Failed to set Access-Control-Allow-Origin, err: %d", ret);
        blink_error<ERR_STREAM_SERVER>(ERR_STREAM_SET_HDR, true);
        return ret;
    }

    led::enable(true);

    while (true) {
        if constexpr (ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO)
            last_frame = esp_timer_get_time();

        fb = esp_camera_fb_get();
        if (!fb) {
            log_e("Failed to get frame from frambuffer");
            blink_error<ERR_STREAM_SERVER>(ERR_STREAM_GET_FB, true);
            ret = ESP_FAIL;
        } else {
            timestamp.tv_sec  = fb->timestamp.tv_sec;
            timestamp.tv_usec = fb->timestamp.tv_usec;
            if (fb->format != PIXFORMAT_JPEG) {
                if (!frame2jpg(fb, FRAME2JPG_QUALITY, &buf, &buf_len)) {
                    log_e("Failed to encode frame to JPEG");
                    blink_error<ERR_STREAM_SERVER>(ERR_STREAM_ENCODE_JPEG, true);
                    ret = ESP_FAIL;
                }
                esp_camera_fb_return(fb);
                fb = nullptr;
            } else {
                buf_len = fb->len;
                buf     = fb->buf;
            }
        }

        if (ret == ESP_OK) {
            ret = httpd_resp_send_chunk(req, STREAM_BOUNDARY, sizeof(STREAM_BOUNDARY) - 1);
        }
        if (ret == ESP_OK) {
            const ssize_t hlen = snprintf(reinterpret_cast<char *>(part_buf),
                                          STREAM_PART_FULL_LEN,
                                          STREAM_PART,
                                          buf_len,
                                          static_cast<long int>(timestamp.tv_sec),
                                          static_cast<long int>(timestamp.tv_usec),
                                          1000.f / frame_time,
                                          1000.f / avg_frame_time);
            ret = httpd_resp_send_chunk(req, reinterpret_cast<const char *>(part_buf), hlen);
        }
        if (ret == ESP_OK) {
            ret = httpd_resp_send_chunk(req, reinterpret_cast<const char *>(buf), buf_len);
        }

        if (fb) {
            esp_camera_fb_return(fb);
            fb  = nullptr;
            buf = nullptr;
        } else if (buf) {
            free(buf);
            buf = nullptr;
        }

        if (ret != ESP_OK) {
            log_w("Failed to send the frame, err: %d", ret);
            break;
        }

        if constexpr (ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO) {
            frame_time     = (esp_timer_get_time() - last_frame) / 1000;
            avg_frame_time = ra_filter(static_cast<int32_t>(frame_time));
            log_i("MJPG: %luB %lldms (%.1ffps), AVG: %lums (%.1ffps)",
                  buf_len,
                  frame_time,
                  1000.f / frame_time,
                  avg_frame_time,
                  1000.f / avg_frame_time);
        }
    }

    led::enable(false);

    return ret;
}

httpd_handle_t stream_httpd = nullptr;

void stream::start() {
    httpd_config_t config   = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    config.task_priority   += 1;
    config.server_port     += 1;
    config.ctrl_port       += 1;

    const httpd_uri_t stream_uri = {
      .uri      = "/stream",
      .method   = HTTP_GET,
      .handler  = stream_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    log_i("Starting Stream Server on port: '%d'", config.server_port);
    esp_err_t res = httpd_start(&stream_httpd, &config);
    if (res == ESP_OK) {
        res = httpd_register_uri_handler(stream_httpd, &stream_uri);
        if (res != ESP_OK) {
            log_e("Failed to register URI handler, err: %d", res);
            blink_error<ERR_STREAM_SERVER>(ERR_STREAM_REG_URI, true);
        }
    } else {
        log_e("Failed to start Stream Server, err: %d", res);
        blink_error<ERR_STREAM_SERVER>(ERR_STREAM_START, true);
    }
}
