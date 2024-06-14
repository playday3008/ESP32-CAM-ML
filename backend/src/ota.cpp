#include "ota.hpp"

#include <esp_log.h>
#include <esp_http_server.h>

#include <Update.h>

#include "error.hpp"

#include "config.hpp"

static constexpr const char OTA_INDEX[] =
    R"(<!DOCTYPE html>
      <html lang='en'>
      
      <head>
          <meta charset='utf-8'>
          <meta name='viewport' content='width=device-width,initial-scale=1' />
      </head>
      
      <body>
          <form method='POST' action='' enctype='multipart/form-data'>
              Firmware:<br>
              <input type='file' accept='.bin,.bin.gz' name='firmware'>
              <input type='submit' value='Update Firmware'>
          </form>
          <form method='POST' action='' enctype='multipart/form-data'>
              FileSystem:<br>
              <input type='file' accept='.bin,.bin.gz,.image' name='filesystem'>
              <input type='submit' value='Update FileSystem'>
          </form>
      </body>
      
      </html>)";
static constexpr const char OTA_SUCCESS[] =
    "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...";

static esp_err_t update_handler(httpd_req_t* req) {
    esp_err_t ret = ESP_OK;

    switch (req->method) {
        case HTTP_GET: {
            ret = httpd_resp_send(req, OTA_INDEX, sizeof(OTA_INDEX) - 1);
            if (ret != ESP_OK) {
                log_e("Failed to send response, err: %d", ret);
                blink_error<ERR_OTA_SERVER>(ERR_OTA_HTTP_GET, true);
            }
        } break;
        case HTTP_POST: {
            // _server->on(
            //     path.c_str(),
            //     HTTP_POST,
            //     [&]() {
            //         if (Update.hasError()) {
            //             _server->send(200,
            //                           F("text/html"),
            //                           String(F("Update error: ")) + _updaterError);
            //         } else {
            //             _server->client().setNoDelay(true);
            //             _server->send_P(200, PSTR("text/html"), successResponse);
            //             delay(100);
            //             _server->client().stop();
            //             ESP.restart();
            //         }
            //     },
            //     [&]() {
            //         // handler for the file upload, gets the sketch bytes, and writes
            //         // them through the Update object
            //         HTTPUpload &upload = _server->upload();
            //
            //         if (upload.status == UPLOAD_FILE_START) {
            //             _updaterError.clear();
            //             if (_serial_output) {
            //                 Serial.setDebugOutput(true);
            //             }
            //
            //             if (_serial_output) {
            //                 Serial.printf("Update: %s\n", upload.filename.c_str());
            //             }
            //             if (upload.name == "filesystem") {
            //                 if (!Update.begin(SPIFFS.totalBytes(),
            //                                   U_SPIFFS)) {  // start with max available size
            //                     if (_serial_output) {
            //                         Update.printError(Serial);
            //                     }
            //                 }
            //             } else {
            //                 uint32_t maxSketchSpace =
            //                     (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
            //                 if (!Update.begin(maxSketchSpace,
            //                                   U_FLASH)) {  // start with max available size
            //                     _setUpdaterError();
            //                 }
            //             }
            //         } else if (upload.status == UPLOAD_FILE_WRITE &&
            //                    !_updaterError.length()) {
            //             if (_serial_output) {
            //                 Serial.printf(".");
            //             }
            //             if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            //                 _setUpdaterError();
            //             }
            //         } else if (upload.status == UPLOAD_FILE_END &&
            //                    !_updaterError.length()) {
            //             if (Update.end(true)) {  // true to set the size to the current progress
            //                 if (_serial_output) {
            //                     Serial.printf("Update Success: %u\nRebooting...\n",
            //                                   upload.totalSize);
            //                 }
            //             } else {
            //                 _setUpdaterError();
            //             }
            //             if (_serial_output) {
            //                 Serial.setDebugOutput(false);
            //             }
            //         } else if (upload.status == UPLOAD_FILE_ABORTED) {
            //             Update.end();
            //             if (_serial_output) {
            //                 Serial.println("Update was aborted");
            //             }
            //         }
            //         delay(0);
            //     });
            ret = httpd_resp_send_err(req, HTTPD_405_METHOD_NOT_ALLOWED, "Not implemented");
            if (ret != ESP_OK) {
                log_e("Failed to send response, err: %d", ret);
                blink_error<ERR_OTA_SERVER>(ERR_OTA_HTTP_POST, true);
            }
        } break;
        default: {
            ret = httpd_resp_send_404(req);
            break;
        }
    }

    return ret;
}

httpd_handle_t ota_httpd = nullptr;

void ota::start() {
    httpd_config_t config   = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    config.task_priority   -= 1;
    config.server_port      = 3232;
    config.ctrl_port        = 3233;

    const httpd_uri_t update_get_uri = {
      .uri      = g_settings.ota.path.c_str(),
      .method   = HTTP_GET,
      .handler  = update_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    const httpd_uri_t update_post_uri = {
      .uri      = g_settings.ota.path.c_str(),
      .method   = HTTP_POST,
      .handler  = update_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    log_i("Starting OTA server on port: '%d'", config.server_port);
    esp_err_t res = httpd_start(&ota_httpd, &config);
    if (res == ESP_OK) {
        res = httpd_register_uri_handler(ota_httpd, &update_get_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;

        res = httpd_register_uri_handler(ota_httpd, &update_post_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;

        if (false) {
        ota_register_uri_handler_failed:
            log_e("Failed to register URI handler, err: %d", res);
            blink_error<ERR_OTA_SERVER>(ERR_OTA_REG_URI, true);
        }
    } else {
        log_e("Failed to start OTA Server, err: %d", res);
        blink_error<ERR_OTA_SERVER>(ERR_OTA_START, true);
    }
}
