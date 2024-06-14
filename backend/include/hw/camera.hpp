#pragma once

#include <cstdint>

// =============================
// Camera module
// =============================

#pragma region

#define CAMERA_MODULES                  \
    /* ESP32 Boards */                  \
    X(ESP32_WROVER_KIT)                 \
    X(ESP_EYE)                          \
    X(ESP32S3_EYE)                      \
    X(M5STACK)                          \
    X(M5STACK_V2)                       \
    X(M5STACK_WIDE)                     \
    X(M5STACK_ESP32CAM)                 \
    X(M5STACK_UNITCAM)                  \
    X(AI_THINKER_FLASH_LED)             \
    X(AI_THINKER_NORMAL_LED)            \
    X(TTGO_T_JOURNAL)                   \
    X(XIAO_ESP32S3)                     \
    /* Espressif Internal Boards */     \
    X(ESP32_CAM_BOARD)                  \
    X(ESP32_CAM_BOARD_W_BOARD_HEADER)   \
    X(ESP32S2_CAM_BOARD)                \
    X(ESP32S2_CAM_BOARD_W_BOARD_HEADER) \
    X(ESP32S3_CAM_LCD)                  \
    X(DFROBOT_FIREBEETLE_2_ESP32S3)     \
    X(DFROBOT_ROMEO_ESP32S3)            \
    X(MAX)

#define X(camera_module) camera_module,
enum CameraModule_e : uint8_t { CAMERA_MODULES };
using CameraModule_t = enum CameraModule_e;
#undef X

#define X(camera_module) #camera_module,
constexpr inline const char* const camera_module_names[] = {CAMERA_MODULES};
#undef X

#undef CAMERA_MODULES

#pragma endregion

constexpr inline CameraModule_t camera_module = []() -> CameraModule_t {
#if defined(ARDUINO_ESP32_WROVER_KIT)
    return ESP32_WROVER_KIT;
#elif defined(ARDUINO_XIAO_ESP32S3)
    return XIAO_ESP32S3;
#elif defined(ARDUINO_ESP32S3_CAM_LCD)
    return ESP32S3_CAM_LCD;
#elif defined(ARDUINO_DFROBOT_ROMEO_ESP32S3)
    return DFROBOT_ROMEO_ESP32S3;
#else
    // Set manually if not using a known board
    #warning "Board model not detected. Please set BOARD_MODEL manually."
    return MAX;
#endif
}();

// =============================
// Camera pinout
// =============================

struct CameraPinout_s {
    int pin_pwdn;
    int pin_reset;
    int pin_xclk;

    int pin_sccb_sda;
    int pin_sccb_scl;

    int pin_d7;
    int pin_d6;
    int pin_d5;
    int pin_d4;
    int pin_d3;
    int pin_d2;
    int pin_d1;
    int pin_d0;

    int pin_vsync;
    int pin_href;
    int pin_pclk;

    int pin_led = -1;

    constexpr CameraPinout_s() noexcept {
        switch (camera_module) {
            case ESP32_WROVER_KIT: {
                this->pin_pwdn  = -1;
                this->pin_reset = -1;
                this->pin_xclk  = 21;

                this->pin_sccb_sda = 26;
                this->pin_sccb_scl = 27;

                this->pin_d7 = 35;
                this->pin_d6 = 34;
                this->pin_d5 = 39;
                this->pin_d4 = 36;
                this->pin_d3 = 19;
                this->pin_d2 = 18;
                this->pin_d1 = 5;
                this->pin_d0 = 4;

                this->pin_vsync = 25;
                this->pin_href  = 23;
                this->pin_pclk  = 22;
            } break;
            case ESP_EYE: {
                this->pin_pwdn  = -1;
                this->pin_reset = -1;
                this->pin_xclk  = 4;

                this->pin_sccb_sda = 18;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 36;
                this->pin_d6 = 37;
                this->pin_d5 = 38;
                this->pin_d4 = 39;
                this->pin_d3 = 35;
                this->pin_d2 = 14;
                this->pin_d1 = 13;
                this->pin_d0 = 34;

                this->pin_vsync = 5;
                this->pin_href  = 27;
                this->pin_pclk  = 25;

                this->pin_led = 22;
            } break;
            case ESP32S3_EYE: {
                this->pin_pwdn  = -1;
                this->pin_reset = -1;
                this->pin_xclk  = 15;

                this->pin_sccb_sda = 4;
                this->pin_sccb_scl = 5;

                this->pin_d7 = 16;
                this->pin_d6 = 17;
                this->pin_d5 = 18;
                this->pin_d4 = 12;
                this->pin_d3 = 10;
                this->pin_d2 = 8;
                this->pin_d1 = 9;
                this->pin_d0 = 11;

                this->pin_vsync = 6;
                this->pin_href  = 7;
                this->pin_pclk  = 13;
            } break;
            case M5STACK: {
                this->pin_pwdn  = -1;
                this->pin_reset = 15;
                this->pin_xclk  = 27;

                this->pin_sccb_sda = 25;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 19;
                this->pin_d6 = 36;
                this->pin_d5 = 18;
                this->pin_d4 = 39;
                this->pin_d3 = 5;
                this->pin_d2 = 34;
                this->pin_d1 = 35;
                this->pin_d0 = 32;

                this->pin_vsync = 22;
                this->pin_href  = 26;
                this->pin_pclk  = 21;
            } break;
            case M5STACK_V2: {
                this->pin_pwdn  = -1;
                this->pin_reset = 15;
                this->pin_xclk  = 27;

                this->pin_sccb_sda = 22;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 19;
                this->pin_d6 = 36;
                this->pin_d5 = 18;
                this->pin_d4 = 39;
                this->pin_d3 = 5;
                this->pin_d2 = 34;
                this->pin_d1 = 35;
                this->pin_d0 = 32;

                this->pin_vsync = 25;
                this->pin_href  = 26;
                this->pin_pclk  = 21;
            } break;
            case M5STACK_WIDE: {
                this->pin_pwdn  = -1;
                this->pin_reset = 15;
                this->pin_xclk  = 27;

                this->pin_sccb_sda = 22;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 19;
                this->pin_d6 = 36;
                this->pin_d5 = 18;
                this->pin_d4 = 39;
                this->pin_d3 = 5;
                this->pin_d2 = 34;
                this->pin_d1 = 35;
                this->pin_d0 = 32;

                this->pin_vsync = 25;
                this->pin_href  = 26;
                this->pin_pclk  = 21;

                this->pin_led = 2;
            } break;
            case M5STACK_ESP32CAM: {
                this->pin_pwdn  = -1;
                this->pin_reset = 15;
                this->pin_xclk  = 27;

                this->pin_sccb_sda = 25;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 19;
                this->pin_d6 = 36;
                this->pin_d5 = 18;
                this->pin_d4 = 39;
                this->pin_d3 = 5;
                this->pin_d2 = 34;
                this->pin_d1 = 35;
                this->pin_d0 = 17;

                this->pin_vsync = 22;
                this->pin_href  = 26;
                this->pin_pclk  = 21;
            } break;
            case M5STACK_UNITCAM: {
                this->pin_pwdn  = -1;
                this->pin_reset = 15;
                this->pin_xclk  = 27;

                this->pin_sccb_sda = 25;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 19;
                this->pin_d6 = 36;
                this->pin_d5 = 18;
                this->pin_d4 = 39;
                this->pin_d3 = 5;
                this->pin_d2 = 34;
                this->pin_d1 = 35;
                this->pin_d0 = 32;

                this->pin_vsync = 22;
                this->pin_href  = 26;
                this->pin_pclk  = 21;
            } break;
            case AI_THINKER_FLASH_LED: {
                this->pin_pwdn  = 32;
                this->pin_reset = -1;
                this->pin_xclk  = 0;

                this->pin_sccb_sda = 26;
                this->pin_sccb_scl = 27;

                this->pin_d7 = 35;
                this->pin_d6 = 34;
                this->pin_d5 = 39;
                this->pin_d4 = 36;
                this->pin_d3 = 21;
                this->pin_d2 = 19;
                this->pin_d1 = 18;
                this->pin_d0 = 5;

                this->pin_vsync = 25;
                this->pin_href  = 23;
                this->pin_pclk  = 22;

                this->pin_led = 4;
            } break;
            case AI_THINKER_NORMAL_LED: {
                this->pin_pwdn  = 32;
                this->pin_reset = -1;
                this->pin_xclk  = 0;

                this->pin_sccb_sda = 26;
                this->pin_sccb_scl = 27;

                this->pin_d7 = 35;
                this->pin_d6 = 34;
                this->pin_d5 = 39;
                this->pin_d4 = 36;
                this->pin_d3 = 21;
                this->pin_d2 = 19;
                this->pin_d1 = 18;
                this->pin_d0 = 5;

                this->pin_vsync = 25;
                this->pin_href  = 23;
                this->pin_pclk  = 22;

                this->pin_led = 33;
            } break;
            case TTGO_T_JOURNAL: {
                this->pin_pwdn  = 0;
                this->pin_reset = 15;
                this->pin_xclk  = 27;

                this->pin_sccb_sda = 25;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 19;
                this->pin_d6 = 36;
                this->pin_d5 = 18;
                this->pin_d4 = 39;
                this->pin_d3 = 5;
                this->pin_d2 = 34;
                this->pin_d1 = 35;
                this->pin_d0 = 17;

                this->pin_vsync = 22;
                this->pin_href  = 26;
                this->pin_pclk  = 21;
            } break;
            case XIAO_ESP32S3: {
                this->pin_pwdn  = -1;
                this->pin_reset = -1;
                this->pin_xclk  = 10;

                this->pin_sccb_sda = 40;
                this->pin_sccb_scl = 39;

                this->pin_d7 = 48;
                this->pin_d6 = 11;
                this->pin_d5 = 12;
                this->pin_d4 = 14;
                this->pin_d3 = 16;
                this->pin_d2 = 18;
                this->pin_d1 = 17;
                this->pin_d0 = 15;

                this->pin_vsync = 38;
                this->pin_href  = 47;
                this->pin_pclk  = 13;
            } break;
            case ESP32_CAM_BOARD: {
                this->pin_pwdn  = 32;
                this->pin_reset = 33;
                this->pin_xclk  = 4;

                this->pin_sccb_sda = 18;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 36;
                this->pin_d6 = 19;
                this->pin_d5 = 21;
                this->pin_d4 = 39;
                this->pin_d3 = 35;
                this->pin_d2 = 14;
                this->pin_d1 = 13;
                this->pin_d0 = 34;

                this->pin_vsync = 5;
                this->pin_href  = 27;
                this->pin_pclk  = 25;
            } break;
            case ESP32_CAM_BOARD_W_BOARD_HEADER: {
                this->pin_pwdn  = 32;
                this->pin_reset = 33;
                this->pin_xclk  = 4;

                this->pin_sccb_sda = 18;
                this->pin_sccb_scl = 23;

                this->pin_d7 = 36;
                this->pin_d6 = 19;
                this->pin_d5 = 21;
                this->pin_d4 = 39;
                this->pin_d3 = 13;
                this->pin_d2 = 14;
                this->pin_d1 = 35;
                this->pin_d0 = 34;

                this->pin_vsync = 5;
                this->pin_href  = 27;
                this->pin_pclk  = 25;
            } break;
            case ESP32S2_CAM_BOARD: {
                this->pin_pwdn  = 1;
                this->pin_reset = 2;
                this->pin_xclk  = 42;

                this->pin_sccb_sda = 41;
                this->pin_sccb_scl = 18;

                this->pin_d7 = 16;
                this->pin_d6 = 39;
                this->pin_d5 = 40;
                this->pin_d4 = 15;
                this->pin_d3 = 13;
                this->pin_d2 = 5;
                this->pin_d1 = 12;
                this->pin_d0 = 14;

                this->pin_vsync = 38;
                this->pin_href  = 4;
                this->pin_pclk  = 3;
            } break;
            case ESP32S2_CAM_BOARD_W_BOARD_HEADER: {
                this->pin_pwdn  = 1;
                this->pin_reset = 2;
                this->pin_xclk  = 42;

                this->pin_sccb_sda = 41;
                this->pin_sccb_scl = 18;

                this->pin_d7 = 16;
                this->pin_d6 = 39;
                this->pin_d5 = 40;
                this->pin_d4 = 15;
                this->pin_d3 = 12;
                this->pin_d2 = 5;
                this->pin_d1 = 13;
                this->pin_d0 = 14;

                this->pin_vsync = 38;
                this->pin_href  = 4;
                this->pin_pclk  = 3;
            } break;
            case ESP32S3_CAM_LCD: {
                this->pin_pwdn  = -1;
                this->pin_reset = -1;
                this->pin_xclk  = 40;

                this->pin_sccb_sda = 17;
                this->pin_sccb_scl = 18;

                this->pin_d7 = 39;
                this->pin_d6 = 41;
                this->pin_d5 = 42;
                this->pin_d4 = 12;
                this->pin_d3 = 3;
                this->pin_d2 = 14;
                this->pin_d1 = 47;
                this->pin_d0 = 13;

                this->pin_vsync = 21;
                this->pin_href  = 38;
                this->pin_pclk  = 11;
            } break;
            case DFROBOT_FIREBEETLE_2_ESP32S3:
            case DFROBOT_ROMEO_ESP32S3: {
                this->pin_pwdn  = -1;
                this->pin_reset = -1;
                this->pin_xclk  = 45;

                this->pin_sccb_sda = 1;
                this->pin_sccb_scl = 2;

                this->pin_d7 = 48;
                this->pin_d6 = 46;
                this->pin_d5 = 8;
                this->pin_d4 = 7;
                this->pin_d3 = 4;
                this->pin_d2 = 41;
                this->pin_d1 = 40;
                this->pin_d0 = 39;

                this->pin_vsync = 6;
                this->pin_href  = 42;
                this->pin_pclk  = 5;
            } break;
            case MAX:
            default:
                static_assert(!(camera_module >= MAX), "Camera model not supported");
                return;
        }
    }
};
using CameraPinout_t = struct CameraPinout_s;

constexpr inline CameraPinout_t camera_pinout = {};
