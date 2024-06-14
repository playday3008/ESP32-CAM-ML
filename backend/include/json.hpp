#pragma once

#define ARDUINOJSON_USE_LONG_LONG   1
#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>

#include "types/camera.hpp"
#include "config.hpp"

namespace ArduinoJson {
    template <>
    struct Converter<CameraSettings_t> {
        static bool toJson(const CameraSettings_t& src, JsonVariant dst) {
#define X(dst, src, name) dst[#name] = src.name
            X(dst, src, xclk_freq_hz);
            X(dst, src, ledc_timer);
            X(dst, src, ledc_channel);
            X(dst, src, pixel_format);
            X(dst, src, frame_size);
            X(dst, src, jpeg_quality);
            X(dst, src, fb_count);
            X(dst, src, fb_location);
            X(dst, src, grab_mode);
#undef X
            return true;
        }

        static CameraSettings_t fromJson(JsonVariantConst src) {
            CameraSettings_t camera_settings{};
#define X(name) camera_settings.name = src[#name].template as<decltype(camera_settings.name)>()
            X(xclk_freq_hz);
            X(ledc_timer);
            X(ledc_channel);
            X(pixel_format);
            X(frame_size);
            X(jpeg_quality);
            X(fb_count);
            X(fb_location);
            X(grab_mode);
#undef X
            return camera_settings;
        }

        static bool checkJson(JsonVariantConst src) {
#define X(name) src[#name].template is<decltype(CameraSettings_t{}.name)>()
            // clang-format off
            return X(xclk_freq_hz) &&
                   X(ledc_timer) &&
                   X(ledc_channel) &&
                   X(pixel_format) &&
                   X(frame_size) &&
                   X(jpeg_quality) &&
                   X(fb_count) &&
                   X(fb_location) &&
                   X(grab_mode);
            // clang-format on
#undef X
        }
    };

    template <>
    struct Converter<Settings_t> {
        static bool toJson(const Settings_t& src, JsonVariant dst) {
#define X(dst, name, where) dst[#name] = where.name
            X(dst, magic, src);
            JsonObject wifi = dst["wifi"].template to<JsonObject>();
            {
                X(wifi, mode, src.wifi);
                X(wifi, timeout, src.wifi);
                X(wifi, hostname, src.wifi);
                X(wifi, security, src.wifi);
                JsonObject wifi_ap = wifi["ap"].template to<JsonObject>();
                {
                    X(wifi_ap, ssid, src.wifi.ap);
                    X(wifi_ap, pass, src.wifi.ap);
                    X(wifi_ap, local_ip, src.wifi.ap);
                    X(wifi_ap, gateway, src.wifi.ap);
                    X(wifi_ap, subnet, src.wifi.ap);
                }
                JsonObject wifi_sta = wifi["sta"].template to<JsonObject>();
                {
                    X(wifi_sta, ssid, src.wifi.sta);
                    X(wifi_sta, pass, src.wifi.sta);
                    X(wifi_sta, dhcp, src.wifi.sta);
                    X(wifi_sta, local_ip, src.wifi.sta);
                    X(wifi_sta, gateway, src.wifi.sta);
                    X(wifi_sta, subnet, src.wifi.sta);
                    X(wifi_sta, dns1, src.wifi.sta);
                    X(wifi_sta, dns2, src.wifi.sta);
                }
            }
            JsonObject ota = dst["ota"].template to<JsonObject>();
            {
                X(ota, path, src.ota);
                X(ota, username, src.ota);
                X(ota, password, src.ota);
            }
            X(dst, camera, src);
#undef X
            return true;
        }

        static Settings_t fromJson(JsonVariantConst src) {
#define X(dst, name, where)    dst.name = where[#name].template as<decltype(dst.name)>()
#define X_IP(dst, name, where) dst.name = IPAddress(where[#name].template as<const char*>())
            Settings_t settings(src["magic"].template as<decltype(Settings_t{}.magic)>());

            JsonObjectConst wifi = src["wifi"];
            {
                X(settings.wifi, mode, wifi);
                X(settings.wifi, timeout, wifi);
                strlcpy(settings.wifi.hostname,
                        wifi["hostname"] | WIFI_DEFAULT_HOST,
                        sizeof(settings.wifi.hostname));
                X(settings.wifi, security, wifi);
                JsonObjectConst wifi_ap = wifi["ap"];
                {
                    strlcpy(settings.wifi.ap.ssid,
                            wifi_ap["ssid"] | WIFI_AP_DEFAULT_SSID,
                            sizeof(settings.wifi.ap.ssid));
                    strlcpy(settings.wifi.ap.pass,
                            wifi_ap["pass"] | WIFI_AP_DEFAULT_PASS,
                            sizeof(settings.wifi.ap.pass));
                    X_IP(settings.wifi.ap, local_ip, wifi_ap);
                    X_IP(settings.wifi.ap, gateway, wifi_ap);
                    X_IP(settings.wifi.ap, subnet, wifi_ap);
                }
                JsonObjectConst wifi_sta = wifi["sta"];
                {
                    strlcpy(settings.wifi.sta.ssid,
                            wifi_sta["ssid"] | WIFI_STA_DEFAULT_SSID,
                            sizeof(settings.wifi.sta.ssid));
                    strlcpy(settings.wifi.sta.pass,
                            wifi_sta["pass"] | WIFI_STA_DEFAULT_PASS,
                            sizeof(settings.wifi.sta.pass));
                    X(settings.wifi.sta, dhcp, wifi_sta);
                    X_IP(settings.wifi.sta, local_ip, wifi_sta);
                    X_IP(settings.wifi.sta, gateway, wifi_sta);
                    X_IP(settings.wifi.sta, subnet, wifi_sta);
                    X_IP(settings.wifi.sta, dns1, wifi_sta);
                    X_IP(settings.wifi.sta, dns2, wifi_sta);
                }
            }
            JsonObjectConst ota = src["ota"];
            {
                X(settings.ota, path, ota);
                X(settings.ota, username, ota);
                X(settings.ota, password, ota);
            }
            X(settings, camera, src);
#undef X_IP
#undef X
            return settings;
        }

        static bool checkJson(JsonVariantConst src) {
            return src["magic"].template is<decltype(Settings_t{}.magic)>() &&
                   src["wifi"]["mode"].template is<decltype(Settings_t{}.wifi.mode)>() &&
                   src["wifi"]["timeout"].template is<decltype(Settings_t{}.wifi.timeout)>() &&
                   src["wifi"]["hostname"].template is<const char*>() &&
                   src["wifi"]["security"].template is<decltype(Settings_t{}.wifi.security)>() &&
                   src["wifi"]["ap"]["ssid"].template is<const char*>() &&
                   src["wifi"]["ap"]["pass"].template is<const char*>() &&
                   src["wifi"]["ap"]["local_ip"].template is<const char*>() &&
                   src["wifi"]["ap"]["gateway"].template is<const char*>() &&
                   src["wifi"]["ap"]["subnet"].template is<const char*>() &&
                   src["wifi"]["sta"]["ssid"].template is<const char*>() &&
                   src["wifi"]["sta"]["pass"].template is<const char*>() &&
                   src["wifi"]["sta"]["dhcp"].template is<decltype(Settings_t{}.wifi.sta.dhcp)>() &&
                   src["wifi"]["sta"]["local_ip"].template is<const char*>() &&
                   src["wifi"]["sta"]["gateway"].template is<const char*>() &&
                   src["wifi"]["sta"]["subnet"].template is<const char*>() &&
                   src["wifi"]["sta"]["dns1"].template is<const char*>() &&
                   src["wifi"]["sta"]["dns2"].template is<const char*>() &&
                   src["ota"]["path"].template is<const char*>() &&
                   src["ota"]["username"].template is<const char*>() &&
                   src["ota"]["password"].template is<const char*>() &&
                   src["camera"].template is<decltype(Settings_t{}.camera)>();
        }
    };
}  // namespace ArduinoJson
