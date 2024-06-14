#pragma once

#include <WiFi.h>

// =============================
// Wi-Fi Modes
// =============================

#define WIFI_MODES \
    X(NULL)        \
    X(STA)         \
    X(AP)          \
    X(APSTA)       \
    X(NAN)         \
    X(MAX)

#define X(wifi_modes) #wifi_modes,
constexpr inline const char* const wifi_mode_names[] = {WIFI_MODES};
static_assert(sizeof(wifi_mode_names) / sizeof(wifi_mode_names[0]) == WIFI_MODE_MAX + 1,
              "wifi_mode_names size mismatch");
#undef X
#undef WIFI_MODES

// =============================
// Wi-Fi Security
// =============================

#define WIFI_SECURITY  \
    X(OPEN)            \
    X(WEP)             \
    X(WPA_PSK)         \
    X(WPA2_PSK)        \
    X(WPA_WPA2_PSK)    \
    X(WPA2_ENTERPRISE) \
    X(WPA3_PSK)        \
    X(WPA2_WPA3_PSK)   \
    X(WAPI_PSK)        \
    X(OWE)             \
    X(WPA3_ENT_192)    \
    X(MAX)

#define X(wifi_security) #wifi_security,
constexpr inline const char* const wifi_security_names[] = {WIFI_SECURITY};
static_assert(sizeof(wifi_security_names) / sizeof(wifi_security_names[0]) == WIFI_AUTH_MAX + 1,
              "wifi_security_names size mismatch");
#undef X
#undef WIFI_SECURITY
