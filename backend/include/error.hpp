#pragma once

#include <esp_arduino_version.h>

#if ESP_ARDUINO_VERSION < ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    #error "Unsupported ESP Arduino version, need at least 3.0.0"
#endif

#include <Arduino.h>

#include <cstdint>

#include <type_traits>

constexpr uint8_t LED_TO_BLINK      = LED_BUILTIN;
constexpr uint8_t LED_TO_BLINK_HIGH = LOW;
constexpr uint8_t LED_TO_BLINK_LOW  = HIGH;

enum ErrorKind_u : uint8_t {
    ERR_NONE   = 0,
    ERR_SPIFFS = 1,
    ERR_SETTINGS,
    ERR_WIFI,
    ERR_CAMERA,
    ERR_FRONTEND,
    ERR_APP_SERVER,
    ERR_STREAM_SERVER,
    ERR_OTA_SERVER,
};
using ErrorKind_t = enum ErrorKind_u;

enum ErrorSPIFFS_u : uint8_t {
    ERR_SPIFFS_MOUNT = 1,
    ERR_SPIFFS_FORMAT,
};

enum ErrorSettings_u : uint8_t {
    ERR_SETTINGS_CREATE = 1,
    ERR_SETTINGS_READ,
    ERR_SETTINGS_WRITE,
    ERR_SETTINGS_REMOVE,
    ERR_SETTINGS_PARSE,
    ERR_SETTINGS_MAGIC,
};

enum ErrorWiFi_u : uint8_t {
    ERR_WIFI_STA_STATIC_IP = 1,
    ERR_WIFI_STA_CONNECT,
};

enum ErrorCamera_u : uint8_t {
    ERR_CAMERA_INIT = 1,
    ERR_CAMERA_SENSOR,
};

enum ErrorFrontend_u : uint8_t {
    ERR_FRONTEND_INDEX_ALLOC = 1,
    ERR_FRONTEND_INDEX_OPEN,
    ERR_FRONTEND_INDEX_READ,
    ERR_FRONTEND_INDEX_REMOVE,
    ERR_FRONTEND_APP_ALLOC,
    ERR_FRONTEND_APP_OPEN,
    ERR_FRONTEND_APP_READ,
    ERR_FRONTEND_APP_REMOVE,
};

enum ErrorApp_u : uint8_t {
    ERR_APP_START = 1,
    ERR_APP_REG_URI
};

enum ErrorStream_u : uint8_t {
    ERR_STREAM_START = 1,
    ERR_STREAM_REG_URI,
    ERR_STREAM_GET_FB,
    ERR_STREAM_ENCODE_JPEG,
    ERR_STREAM_SET_CT,
    ERR_STREAM_SET_HDR
};

enum ErrorOTA_u : uint8_t {
    ERR_OTA_START = 1,
    ERR_OTA_REG_URI,
    ERR_OTA_HTTP_GET,
    ERR_OTA_HTTP_POST,
};

/**
 * @brief Error code type
 *
 * @tparam kind Error kind
 */
template <ErrorKind_t kind>
// clang-format off
using ErrorCode_t =
typename std::conditional<
    kind == ErrorKind_t::ERR_SPIFFS,
    ErrorSPIFFS_u,
    typename std::conditional<
        kind == ErrorKind_t::ERR_SETTINGS,
        ErrorSettings_u,
        typename std::conditional<
            kind == ErrorKind_t::ERR_WIFI,
            ErrorWiFi_u,
            typename std::conditional<
                kind == ErrorKind_t::ERR_CAMERA,
                ErrorCamera_u,
                typename std::conditional<
                    kind == ErrorKind_t::ERR_FRONTEND,
                    ErrorFrontend_u,
                    typename std::conditional<
                        kind == ErrorKind_t::ERR_APP_SERVER,
                        ErrorApp_u,
                        typename std::conditional<
                            kind == ErrorKind_t::ERR_STREAM_SERVER,
                            ErrorStream_u,
                            typename std::conditional<
                                kind == ErrorKind_t::ERR_OTA_SERVER,
                                ErrorOTA_u,
                                uint8_t
                            >::type
                        >::type
                    >::type
                >::type
            >::type
        >::type
    >::type
>::type;
// clang-format on

/**
 * @brief Report error by blinking the LED
 *
 * @tparam kind Error kind (number of blinks)
 * @param times Error code (number of blinks)
 * 
 * @note Function is synchronous
 */
template <ErrorKind_t kind>
inline void blink_error(ErrorCode_t<kind> times, bool once) {
    pinMode(LED_TO_BLINK, OUTPUT);
    do {
#pragma unroll
        for (uint8_t i = 0; i < static_cast<uint8_t>(kind); i++) {
            digitalWrite(LED_TO_BLINK, LED_TO_BLINK_HIGH);
            delay(100);
            digitalWrite(LED_TO_BLINK, LED_TO_BLINK_LOW);
            delay(100);
        }
        delay(500);
#pragma unroll
        for (uint8_t i = 0; i < static_cast<uint8_t>(times); i++) {
            digitalWrite(LED_TO_BLINK, LED_TO_BLINK_HIGH);
            delay(100);
            digitalWrite(LED_TO_BLINK, LED_TO_BLINK_LOW);
            delay(100);
        }
        delay(1000);
    } while (!once);
}
