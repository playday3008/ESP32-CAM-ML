#pragma once

#include <cstddef>
#include <cinttypes>  // Also includes <cstdint>

#include <utility>

#include <WString.h>

#include <WiFi.h>

#include "types/camera.hpp"

// =============================
// Version definitions
// =============================

constexpr uint8_t  MAJOR_VERSION = 1;
constexpr uint8_t  MINOR_VERSION = 2;
constexpr uint8_t  PATCH_VERSION = 0;
constexpr uint8_t  REVISION      = 0;
constexpr uint32_t FIRMWARE_MAGIC =
    (static_cast<uint32_t>(MAJOR_VERSION) << 24U | static_cast<uint32_t>(MINOR_VERSION) << 16U |
     static_cast<uint32_t>(PATCH_VERSION) << 8U | REVISION);

// =============================
// Serial settings
// =============================

constexpr uint32_t SERIAL_BAUD_RATE = 115200;
constexpr bool SERIAL_OUTPUT_DEBUG  = ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG ? true : false;

// =============================
// SPIFFS settings
// =============================

constexpr const char* SPIFFS_SETTINGS_PATH     = "/settings.jsonc";
constexpr const char* SPIFFS_INDEX_BUNDLE_PATH = "/index.html.gz";
constexpr const char* SPIFFS_API_BUNDLE_PATH   = "/api.html.gz";
constexpr bool        SPIFFS_FORMAT_IF_FAILED  = true;

// =============================
// WiFi settings
// =============================

constexpr uint32_t WIFI_RETRY_DELAY = 200;

constexpr uint32_t WIFI_TIMEOUT   = 20 * 1000;
constexpr size_t   WIFI_SSID_SIZE = 0x20 + 1;
constexpr size_t   WIFI_PASS_SIZE = 0x40 + 1;
constexpr size_t   WIFI_HOST_SIZE = 0x40 + 1;

#define WIFI_DEFAULT_HOST "ESP32-Sense"

#define WIFI_AP_DEFAULT_SSID "ESP32-Sense-AP"
#define WIFI_AP_DEFAULT_PASS "TotallySecurePassword1234!"
constexpr uint8_t WIFI_AP_DEFAULT_LOCAL_IP[4] = {192, 168, 69, 1};
constexpr uint8_t WIFI_AP_DEFAULT_GATEWAY[4]  = {192, 168, 69, 1};
constexpr uint8_t WIFI_AP_DEFAULT_SUBNET[4]   = {255, 255, 255, 0};

#define WIFI_STA_DEFAULT_SSID ""
#define WIFI_STA_DEFAULT_PASS ""
constexpr bool    WIFI_STA_DEFAULT_DHCP        = true;
constexpr uint8_t WIFI_STA_DEFAULT_LOCAL_IP[4] = {};
constexpr uint8_t WIFI_STA_DEFAULT_GATEWAY[4]  = {};
constexpr uint8_t WIFI_STA_DEFAULT_SUBNET[4]   = {};
constexpr uint8_t WIFI_STA_DEFAULT_DNS1[4]     = {94, 140, 14, 140};
constexpr uint8_t WIFI_STA_DEFAULT_DNS2[4]     = {94, 140, 14, 141};

// =============================
// OTA settings
// =============================

constexpr const char* OTA_DEFAULT_PATH     = "/update";
constexpr const char* OTA_DEFAULT_USERNAME = "";
constexpr const char* OTA_DEFAULT_PASSWORD = "";

// =============================
// Settings
// =============================

constexpr uint32_t MAIN_LOOP_DELAY = 15000;

struct Settings_s {
    const uint64_t magic;
    struct WiFi_s {
        wifi_mode_t      mode                     = WIFI_MODE_AP;
        uint32_t         timeout                  = WIFI_TIMEOUT;
        char             hostname[WIFI_HOST_SIZE] = WIFI_DEFAULT_HOST;
        wifi_auth_mode_t security                 = WIFI_AUTH_WPA_WPA2_PSK;
        struct AP_s {
            char      ssid[WIFI_SSID_SIZE] = WIFI_AP_DEFAULT_SSID;
            char      pass[WIFI_PASS_SIZE] = WIFI_AP_DEFAULT_PASS;
            IPAddress local_ip{IPv4, WIFI_AP_DEFAULT_LOCAL_IP};
            IPAddress gateway{IPv4, WIFI_AP_DEFAULT_GATEWAY};
            IPAddress subnet{IPv4, WIFI_AP_DEFAULT_SUBNET};
        } ap{};
        struct STA_s {
            char      ssid[WIFI_SSID_SIZE] = WIFI_STA_DEFAULT_SSID;
            char      pass[WIFI_PASS_SIZE] = WIFI_STA_DEFAULT_PASS;
            bool      dhcp                 = WIFI_STA_DEFAULT_DHCP;
            IPAddress local_ip{IPv4, WIFI_STA_DEFAULT_LOCAL_IP};
            IPAddress gateway{IPv4, WIFI_STA_DEFAULT_GATEWAY};
            IPAddress subnet{IPv4, WIFI_STA_DEFAULT_SUBNET};
            IPAddress dns1{IPv4, WIFI_STA_DEFAULT_DNS1};
            IPAddress dns2{IPv4, WIFI_STA_DEFAULT_DNS2};
        } sta{};
    } wifi{};
    struct OTA_s {
        String path     = OTA_DEFAULT_PATH;
        String username = OTA_DEFAULT_USERNAME;
        String password = OTA_DEFAULT_PASSWORD;
    } ota{};
    CameraSettings_t camera{};

    Settings_s()
        : magic((static_cast<uint64_t>(FIRMWARE_MAGIC) << 32U) |
                static_cast<uint64_t>(sizeof(*this) & 0xFFFFU)){};
    explicit Settings_s(const uint64_t magic) : magic(magic){};

    Settings_s(const Settings_s& rhs) : magic(rhs.magic) {
        this->wifi   = rhs.wifi;
        this->ota    = rhs.ota;
        this->camera = rhs.camera;
    }

    Settings_s(Settings_s&& rhs) noexcept : magic(rhs.magic) {
        this->wifi   = rhs.wifi;
        this->ota    = rhs.ota;
        this->camera = rhs.camera;
    }

    Settings_s& operator=(const Settings_s& rhs) {
        if (this != &rhs) {
            const_cast<uint64_t&>(this->magic) = rhs.magic;
            this->wifi                         = rhs.wifi;
            this->ota                          = rhs.ota;
            this->camera                       = rhs.camera;
        }
        return *this;
    }

    Settings_s& operator=(Settings_s&& rhs) noexcept {
        if (this != &rhs) {
            const_cast<uint64_t&>(this->magic) = rhs.magic;
            this->wifi                         = std::move(rhs.wifi);
            this->ota                          = std::move(rhs.ota);
            this->camera                       = std::move(rhs.camera);
        }
        return *this;
    }

    ~Settings_s() = default;
};
using Settings_t = struct Settings_s;

#pragma pack(push, 4)
inline Settings_t g_settings{};
#pragma pack(pop)
