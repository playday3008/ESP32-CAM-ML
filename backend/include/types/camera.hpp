#pragma once

#include <Arduino.h>

#include <cstddef>

#include <esp_camera.h>

// =============================
// Pixel formats
// =============================

#pragma region

#define PIXFORMATS \
    X(RGB565)      \
    X(YUV422)      \
    X(YUV420)      \
    X(GRAYSCALE)   \
    X(JPEG)        \
    X(RGB888)      \
    X(RAW)         \
    X(RGB444)      \
    X(RGB555)

#define X(pixformat) #pixformat,
constexpr inline const char* const pixformat_names[] = {PIXFORMATS};
static_assert(sizeof(pixformat_names) / sizeof(pixformat_names[0]) == PIXFORMAT_RGB555 + 1,
              "pixformat_names size mismatch");
#undef X
#undef PIXFORMATS

#pragma endregion

// =============================
// Frame sizes
// =============================

#pragma region

#define FRAMESIZES    \
    X(96X96)          \
    X(QQVGA)          \
    X(QCIF)           \
    X(HQVGA)          \
    X(240X240)        \
    X(QVGA)           \
    X(CIF)            \
    X(HVGA)           \
    X(VGA)            \
    X(SVGA)           \
    X(XGA)            \
    X(HD)             \
    X(SXGA)           \
    X(UXGA)           \
    /* 3MP Sensors */ \
    X(FHD)            \
    X(P_HD)           \
    X(P_3MP)          \
    X(QXGA)           \
    /* 5MP Sensors */ \
    X(QHD)            \
    X(WQXGA)          \
    X(P_FHD)          \
    X(QSXGA)          \
    X(INVALID)

#define X(framesize) #framesize,
constexpr inline const char* const framesize_names[] = {FRAMESIZES};
static_assert(sizeof(framesize_names) / sizeof(framesize_names[0]) == FRAMESIZE_INVALID + 1,
              "framesize_names size mismatch");
#undef X
#undef FRAMESIZES

inline framesize_t get_max_framesize(camera_sensor_info_t* si) {
    // clang-format off
    switch (si->pid) {
        default: // Will return the smallest size
        case OV7725_PID:   return FRAMESIZE_VGA;
        case OV7670_PID:   return FRAMESIZE_VGA;
        case GC2145_PID:   return FRAMESIZE_VGA;
        case GC032A_PID:   return FRAMESIZE_VGA;
        case GC0308_PID:   return FRAMESIZE_VGA;
        case BF3005_PID:   return FRAMESIZE_VGA;
        case BF20A6_PID:   return FRAMESIZE_VGA;
        case SC030IOT_PID: return FRAMESIZE_VGA;
        case SC031GS_PID:  return FRAMESIZE_VGA;
        case NT99141_PID:  return FRAMESIZE_HD;
        case SC101IOT_PID: return FRAMESIZE_HD;
        case OV9650_PID:   return FRAMESIZE_SXGA;
        case OV2640_PID:   return FRAMESIZE_UXGA;
        case OV3660_PID:   return FRAMESIZE_QXGA;
        case OV5640_PID:   return FRAMESIZE_QSXGA;
    }
    // clang-format on
}

#pragma endregion

// =============================
// Camera grab modes
// =============================

#pragma region

#define GRAB_MODES \
    X(WHEN_EMPTY)  \
    X(LATEST)

#define X(grab_mode) #grab_mode,
constexpr inline const char* const grab_mode_names[] = {GRAB_MODES};
static_assert(sizeof(grab_mode_names) / sizeof(grab_mode_names[0]) == CAMERA_GRAB_LATEST + 1,
              "grab_mode_names size mismatch");
#undef X
#undef GRAB_MODES

#pragma endregion

// =============================
// Camera frame buffer location
// =============================

#pragma region

#define FB_LOCATIONS \
    X(IN_PSRAM)      \
    X(IN_DRAM)

#define X(fb_location) #fb_location,
constexpr inline const char* const fb_location_names[] = {FB_LOCATIONS};
static_assert(sizeof(fb_location_names) / sizeof(fb_location_names[0]) == CAMERA_FB_IN_DRAM + 1,
              "fb_location_names size mismatch");
#undef X
#undef FB_LOCATIONS

#pragma endregion

// =============================
// Camera settings
// =============================

constexpr int    CAMERA_DEFAULT_XCLK_FREQ    = 20 * 1000 * 1000;  // 20 MHz
constexpr int    CAMERA_DEFAULT_JPEG_QUALITY = 12;
constexpr size_t CAMERA_DEFAULT_FB_COUNT     = 1;

constexpr int    CAMERA_DEFAULT_PSRAM_JPEG_QUALITY = 10;
constexpr size_t CAMERA_DEFAULT_PSRAM_FB_COUNT     = 2;

struct CameraSettings_s {
    /// Frequency of XCLK signal
    int xclk_freq_hz = CAMERA_DEFAULT_XCLK_FREQ;

    /// LEDC timer to be used for generating XCLK
    ledc_timer_t ledc_timer = LEDC_TIMER_0;
    /// LEDC channel to be used for generating XCLK
    ledc_channel_t ledc_channel = LEDC_CHANNEL_0;

    /// Format of the pixel data
    pixformat_t pixel_format = PIXFORMAT_JPEG;
    /// Size of the output image
    framesize_t frame_size = FRAMESIZE_SVGA;

    /// Quality of JPEG output. 0-63 lower means higher quality
    int jpeg_quality = CAMERA_DEFAULT_JPEG_QUALITY;
    /// Number of frame buffers to be allocated.
    /// If more than one, then each frame will be acquired (double speed)
    size_t fb_count = CAMERA_DEFAULT_FB_COUNT;
    /// The location where the frame buffer will be allocated
    camera_fb_location_t fb_location = CAMERA_FB_IN_DRAM;
    /// When buffers should be filled
    camera_grab_mode_t grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    CameraSettings_s() {
        // If PSRAM is available, then increase the frame size and quality
        if (psramFound()) {
            // frame_size   = FRAMESIZE_UXGA;
            jpeg_quality = CAMERA_DEFAULT_PSRAM_JPEG_QUALITY;
            fb_count     = CAMERA_DEFAULT_PSRAM_FB_COUNT;
            fb_location  = CAMERA_FB_IN_PSRAM;
            grab_mode    = CAMERA_GRAB_LATEST;
        }
    }
};
using CameraSettings_t = struct CameraSettings_s;
