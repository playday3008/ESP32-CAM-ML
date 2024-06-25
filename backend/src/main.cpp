#include <Arduino.h>

#include <cstring>

#include <tuple>
#include <utility>

#include <esp_log.h>
#include <esp_camera.h>
#include <esp_heap_caps.h>

#include "hw/camera.hpp"
#include "types/camera.hpp"
#include "types/wifi.hpp"
#include "config.hpp"
#include "json.hpp"

#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <StreamUtils.h>

#include "bundle.h"
#include "led.hpp"
#include "stream.hpp"
#include "ota.hpp"
#include "app.hpp"
#include "error.hpp"

inline void dump_camera_specs() {
    log_i("Board model: %s", camera_module_names[camera_module]);
    log_i("Camera pinout:");
    log_i("  PWDN:     %d", camera_pinout.pin_pwdn);
    log_i("  RESET:    %d", camera_pinout.pin_reset);
    log_i("  XCLK:     %d", camera_pinout.pin_xclk);
    log_i("  SCCB SDA: %d", camera_pinout.pin_sccb_sda);
    log_i("  SCCB SCL: %d", camera_pinout.pin_sccb_scl);
    log_i("  Y9 (D7):  %d", camera_pinout.pin_d7);
    log_i("  Y8 (D6):  %d", camera_pinout.pin_d6);
    log_i("  Y7 (D5):  %d", camera_pinout.pin_d5);
    log_i("  Y6 (D4):  %d", camera_pinout.pin_d4);
    log_i("  Y5 (D3):  %d", camera_pinout.pin_d3);
    log_i("  Y4 (D2):  %d", camera_pinout.pin_d2);
    log_i("  Y3 (D1):  %d", camera_pinout.pin_d1);
    log_i("  Y2 (D0):  %d", camera_pinout.pin_d0);
    log_i("  VSYNC:    %d", camera_pinout.pin_vsync);
    log_i("  HREF:     %d", camera_pinout.pin_href);
    log_i("  PCLK:     %d", camera_pinout.pin_pclk);
    log_i("  LED:      %d", camera_pinout.pin_led);
}

inline void setup_spiffs() {
    if (!SPIFFS.begin()) {
        if constexpr (SPIFFS_FORMAT_IF_FAILED) {
            log_w("SPIFFS Mount Failed, formatting...");
            if (!SPIFFS.format()) {
                log_e("Failed to format SPIFFS");
                blink_error<ERR_SPIFFS>(ERR_SPIFFS_FORMAT, false);
            } else {
                if (SPIFFS.begin()) return;
            }
        }

        log_e("SPIFFS Mount Failed");
        blink_error<ERR_SPIFFS>(ERR_SPIFFS_MOUNT, false);
    }

    log_i("SPIFFS Mounted");
}

inline void setup_settings() {
    if (!SPIFFS.exists(SPIFFS_SETTINGS_PATH)) {
        log_i("No settings file found");
        log_i("Creating default settings");

        File file = SPIFFS.open(SPIFFS_SETTINGS_PATH, "w", true);
        if (!file) {
            log_e("Failed to create default settings");

            blink_error<ERR_SETTINGS>(ERR_SETTINGS_CREATE, true);
            if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
                blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
            }
            ESP.restart();
        }

        JsonDocument doc;
        doc = g_settings;
        doc.shrinkToFit();
        // Can't use file in serializeJson() directly
        // Having the same issue as here:
        // https://github.com/esp8266/Arduino/issues/8655
        String output;
        serializeJson(doc, output);
        size_t written = file.print(output);
        file.close();
        if (written != output.length()) {
            log_e("Failed to write default settings to file");
            log_i("Expected: %d, Actual: %d", output.length(), written);

            blink_error<ERR_SETTINGS>(ERR_SETTINGS_WRITE, true);
            if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
                blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
            }
            ESP.restart();
        }

        if constexpr (ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG) {
            log_i("Default settings are:");
            Serial.println("```json");
            serializeJsonPretty(doc, Serial);
            Serial.println();
            Serial.println("```");
        }

        log_i("Default settings created!");
    }

    log_i("Reading settings");

    Settings_t new_settings(g_settings);

    File file = SPIFFS.open(SPIFFS_SETTINGS_PATH, "r");
    if (!file) {
        log_e("Failed to open settings file");

        blink_error<ERR_SETTINGS>(ERR_SETTINGS_READ, true);
        if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
            blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
        }
        ESP.restart();
    }

    JsonDocument         doc;
    ReadBufferingStream  bufferingStream(file, 64);
    DeserializationError error;
    if constexpr (ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG) {
        ReadLoggingStream loggingStream(bufferingStream, Serial);
        error = deserializeJson(doc, loggingStream);
        Serial.println();
    } else {
        error = deserializeJson(doc, bufferingStream);
    }
    file.close();
    if (error) {
        log_e("deserializeJson() failed: %s", error.c_str());

        blink_error<ERR_SETTINGS>(ERR_SETTINGS_PARSE, true);
        if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
            blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
        }
        ESP.restart();
    }

    new_settings = doc.template as<Settings_t>();

    // Check magic number
    if (g_settings.magic != new_settings.magic) {
        log_w("Settings magic number mismatch, need 0x%016llx, got 0x%016llx",
              g_settings.magic,
              new_settings.magic);
        log_w("Restoring default settings");

        blink_error<ERR_SETTINGS>(ERR_SETTINGS_MAGIC, true);
        if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
            blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
        }
        ESP.restart();
    }

    g_settings = std::move(new_settings);

    log_i("Settings read successfully!");

    // Print settings
    if constexpr (ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG) {
        JsonDocument doc;
        doc = g_settings;
        doc.shrinkToFit();
        log_d("Settings JSON:");
        Serial.println("```json");
        serializeJsonPretty(doc, Serial);
        Serial.println();
        Serial.println("```");
    }
}

inline void setup_wifi() {
    if (strlen(g_settings.wifi.sta.ssid) > 0) {
        // There is a saved Wi-Fi configuration
        log_i("Connecting to saved Wi-Fi: %s", g_settings.wifi.sta.ssid);

        WiFi.setHostname(g_settings.wifi.hostname);
        WiFi.setMinSecurity(g_settings.wifi.security);

        if (!g_settings.wifi.sta.dhcp) {
            log_i("Setting static IP address");
            if (!WiFi.config(g_settings.wifi.sta.local_ip,
                             g_settings.wifi.sta.gateway,
                             g_settings.wifi.sta.subnet,
                             g_settings.wifi.sta.dns1,
                             g_settings.wifi.sta.dns2)) {
                log_e("Failed to set static IP address");
                blink_error<ERR_WIFI>(ERR_WIFI_STA_STATIC_IP, true);
                if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
                    blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
                }
                ESP.restart();
            }
        } else {
            log_i("Using DHCP");
        }

        WiFi.begin(g_settings.wifi.sta.ssid, g_settings.wifi.sta.pass);
        WiFi.enableIPv6();

        const uint32_t tmr = millis();
        while (!WiFi.isConnected()) {
            delay(WIFI_RETRY_DELAY);
            if (millis() - tmr > g_settings.wifi.timeout) {
                log_i("");
                log_i("Failed to connect to saved Wi-Fi");
                log_i("Falling back to Wi-Fi Portal");
                blink_error<ERR_WIFI>(ERR_WIFI_STA_CONNECT, true);
                WiFi.disconnect(false, true);
                break;
            }
        }

        if (WiFi.isConnected()) {
            log_i("");
            log_n("Wi-Fi Connected!");
            log_n("IPv4 Address: %s", WiFi.localIP().toString().c_str());
            WiFi.onEvent(
                [](WiFiEvent_t event, WiFiEventInfo_t info) {
                    log_n("IPv6 Address: [%s]", WiFi.linkLocalIPv6().toString().c_str());
                },
                WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP6);
        }
    }

    if (!WiFi.isConnected() || g_settings.wifi.mode == WIFI_AP) {
        // There is no saved Wi-Fi configuration
        // or the saved Wi-Fi configuration is invalid
        log_i("No saved Wi-Fi configuration or the saved Wi-Fi configuration is invalid");
        log_i("Starting Wi-Fi Portal");

        // Set Wi-Fi mode to AP
        WiFi.setHostname(g_settings.wifi.hostname);
        WiFi.softAPConfig(g_settings.wifi.ap.local_ip,
                          g_settings.wifi.ap.gateway,
                          g_settings.wifi.ap.subnet);
        WiFi.softAP(g_settings.wifi.ap.ssid, g_settings.wifi.ap.pass);
        WiFi.softAPenableIPv6();

        log_n("Wi-Fi Portal Started!");
        log_n("SSID:         %s", g_settings.wifi.ap.ssid);
        log_n("Password:     %s", g_settings.wifi.ap.pass);
        log_n("IPv4 Address: %s", WiFi.softAPIP().toString().c_str());
        WiFi.onEvent(
            [](WiFiEvent_t event, WiFiEventInfo_t info) {
                log_n("IPv6 Address: [%s]", WiFi.softAPlinkLocalIPv6().toString().c_str());
            },
            WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_GOT_IP6);
    }
}

inline camera_config_t init_camera_config() {
    return camera_config_t{
      .pin_pwdn  = camera_pinout.pin_pwdn,
      .pin_reset = camera_pinout.pin_reset,
      .pin_xclk  = camera_pinout.pin_xclk,

      .pin_sccb_sda = camera_pinout.pin_sccb_sda,
      .pin_sccb_scl = camera_pinout.pin_sccb_scl,

      .pin_d7 = camera_pinout.pin_d7,
      .pin_d6 = camera_pinout.pin_d6,
      .pin_d5 = camera_pinout.pin_d5,
      .pin_d4 = camera_pinout.pin_d4,
      .pin_d3 = camera_pinout.pin_d3,
      .pin_d2 = camera_pinout.pin_d2,
      .pin_d1 = camera_pinout.pin_d1,
      .pin_d0 = camera_pinout.pin_d0,

      .pin_vsync = camera_pinout.pin_vsync,
      .pin_href  = camera_pinout.pin_href,
      .pin_pclk  = camera_pinout.pin_pclk,

      .xclk_freq_hz = g_settings.camera.xclk_freq_hz,

      .ledc_timer   = g_settings.camera.ledc_timer,
      .ledc_channel = g_settings.camera.ledc_channel,

      .pixel_format = g_settings.camera.pixel_format,
      .frame_size   = g_settings.camera.frame_size,

      .jpeg_quality = g_settings.camera.jpeg_quality,
      .fb_count     = g_settings.camera.fb_count,
      .fb_location  = g_settings.camera.fb_location,
      .grab_mode    = g_settings.camera.grab_mode,

      .sccb_i2c_port = -1,  // unused
    };
}

inline void prep_camera_module() {
    switch (camera_module) {
        case ESP_EYE:
            pinMode(13, INPUT_PULLUP);
            pinMode(14, INPUT_PULLUP);
            break;
        default:
            break;
    }
}

inline void setup_camera_module(camera_config_t* camera_config) {
    const esp_err_t err = esp_camera_init(camera_config);
    if (err != ESP_OK) {
        log_e("Camera initialization failed with error 0x%X", err);

        blink_error<ERR_CAMERA>(ERR_CAMERA_INIT, false);
        if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
            blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
        }
        ESP.restart();
    }
}

inline sensor_t* config_sensor() {
    sensor_t* sensor = esp_camera_sensor_get();
    if (sensor == nullptr) {
        log_e("Failed to get camera sensor");

        blink_error<ERR_CAMERA>(ERR_CAMERA_SENSOR, false);
        ESP.restart();
    }

    switch (sensor->id.PID) {
        case OV3660_PID:
            sensor->set_vflip(sensor, true);     // flip it back
            sensor->set_brightness(sensor, 1);   // up the brightness just a bit
            sensor->set_saturation(sensor, -2);  // lower the saturation
            break;
        default:
            break;
    }

    switch (camera_module) {
        case M5STACK_WIDE:
        case M5STACK_ESP32CAM:
            sensor->set_vflip(sensor, true);
            sensor->set_hmirror(sensor, true);
            break;
        case ESP32S3_EYE:
        case XIAO_ESP32S3:
            sensor->set_vflip(sensor, true);
            break;
        default:
            break;
    }

    // Decrease frame size for higher initial frame rate
    // sensor->set_framesize(sensor, FRAMESIZE_SVGA);

    return sensor;
}

inline void setup_led() {
    if (camera_pinout.pin_led != -1) {
        led::setup(camera_pinout.pin_led);
        log_i("LED is on pin %d", camera_pinout.pin_led);
    } else {
        log_i("No LED pin specified");
    }
}

inline void setup_frontend() {
    if (SPIFFS.exists(SPIFFS_INDEX_BUNDLE_PATH)) {
        File bundle = SPIFFS.open(SPIFFS_INDEX_BUNDLE_PATH, "r");
        if (!bundle) {
            log_e("Failed to open Index bundle");

            blink_error<ERR_FRONTEND>(ERR_FRONTEND_INDEX_OPEN, true);
            if (!SPIFFS.remove(SPIFFS_INDEX_BUNDLE_PATH)) {
                blink_error<ERR_FRONTEND>(ERR_FRONTEND_INDEX_REMOVE, true);
            }
            ESP.restart();
        }

        index_full_html_gz_size = bundle.size();
        index_full_html_gz      = reinterpret_cast<uint8_t*>(
            heap_caps_malloc(index_full_html_gz_size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM));
        if (index_full_html_gz == nullptr) {
            log_e("Failed to allocate memory for Index bundle");

            blink_error<ERR_FRONTEND>(ERR_FRONTEND_INDEX_ALLOC, true);
            if (!SPIFFS.remove(SPIFFS_INDEX_BUNDLE_PATH)) {
                blink_error<ERR_FRONTEND>(ERR_FRONTEND_INDEX_REMOVE, true);
            }
            ESP.restart();
        }
        size_t read = bundle.read(index_full_html_gz, index_full_html_gz_size);
        if (read != index_full_html_gz_size) {
            log_e("Failed to read Index bundle");
            log_i("Expected: %d, Actual: %d", index_full_html_gz_size, read);

            blink_error<ERR_FRONTEND>(ERR_FRONTEND_INDEX_READ, true);
            if (!SPIFFS.remove(SPIFFS_INDEX_BUNDLE_PATH)) {
                blink_error<ERR_FRONTEND>(ERR_FRONTEND_INDEX_REMOVE, true);
            }
            ESP.restart();
        }
        bundle.close();
        log_d("Index bundle pointer: %p", index_full_html_gz);
        log_d("Index bundle size: %d", index_full_html_gz_size);

        log_i("Index bundle loaded");
    }

    if (SPIFFS.exists(SPIFFS_API_BUNDLE_PATH)) {
        File bundle = SPIFFS.open(SPIFFS_API_BUNDLE_PATH, "r");
        if (!bundle) {
            log_e("Failed to open API bundle");

            blink_error<ERR_FRONTEND>(ERR_FRONTEND_APP_OPEN, true);
            if (!SPIFFS.remove(SPIFFS_API_BUNDLE_PATH)) {
                blink_error<ERR_FRONTEND>(ERR_FRONTEND_APP_REMOVE, true);
            }
            ESP.restart();
        }

        api_full_html_gz_size = bundle.size();
        api_full_html_gz      = reinterpret_cast<uint8_t*>(
            heap_caps_malloc(api_full_html_gz_size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM));
        if (api_full_html_gz == nullptr) {
            log_e("Failed to allocate memory for App bundle");

            blink_error<ERR_FRONTEND>(ERR_FRONTEND_APP_ALLOC, true);
            if (!SPIFFS.remove(SPIFFS_API_BUNDLE_PATH)) {
                blink_error<ERR_FRONTEND>(ERR_FRONTEND_APP_REMOVE, true);
            }
            ESP.restart();
        }
        size_t read = bundle.read(api_full_html_gz, api_full_html_gz_size);
        if (read != api_full_html_gz_size) {
            log_e("Failed to read App bundle");
            log_i("Expected: %d, Actual: %d", api_full_html_gz_size, read);

            blink_error<ERR_FRONTEND>(ERR_FRONTEND_APP_READ, true);
            if (!SPIFFS.remove(SPIFFS_INDEX_BUNDLE_PATH)) {
                blink_error<ERR_FRONTEND>(ERR_FRONTEND_APP_REMOVE, true);
            }
            ESP.restart();
        }
        bundle.close();
        log_d("API bundle pointer: %p", api_full_html_gz);
        log_d("API bundle size: %d", api_full_html_gz_size);

        log_i("API bundle loaded");
    }
}

#pragma weak setup  // Make it weak to allow tests to override it
void setup() {
    // Sleep for a while to allow the serial monitor to start
    sleep(1);

    // Setup Serial
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.setDebugOutput(SERIAL_OUTPUT_DEBUG);
    log_i();
    log_i("### Firmware Initializing ###");

    log_i();
    log_i("Initialize board model and camera pinout.");
    dump_camera_specs();
    log_i("Initialize board model and camera pinout. Done!");

    log_i();
    log_i("Setup SPIFFS");
    setup_spiffs();
    log_i("Setup SPIFFS. Done!");

    log_i();
    log_i("Setup settings");
    setup_settings();
    log_i("Setup Settings. Done!");

    log_i();
    log_i("Setup Wi-Fi.");
    setup_wifi();
    log_i("Setup Wi-Fi. Done!");

    log_i();
    log_i("Init camera configuration.");
    camera_config_t camera_config = init_camera_config();
    log_i("Init camera configuration. Done!");

    log_i();
    log_i("Prepare camera module.");
    prep_camera_module();
    log_i("Prepare camera module. Done!");

    log_i();
    log_i("Setup camera module.");
    setup_camera_module(&camera_config);
    log_i("Setup camera module. Done!");

    log_i();
    log_i("Sensor/Board specific configuration.");
    sensor_t* sensor = config_sensor();
    std::ignore      = sensor;
    log_i("Sensor/Board specific configuration. Done!");

    log_i();
    log_i("Initialize LED.");
    setup_led();
    log_i("Initialize LED. Done!");

    log_i();
    log_i("Setup frontend");
    setup_frontend();
    log_i("Setup frontend. Done!");

    log_i();
    log_i("Start Stream server.");
    stream::start();
    log_i("Start Stream server. Done!");

    log_i();
    log_i("Start OTA server.");
    ota::start();
    log_i("Start OTA server. Done!");

    log_i();
    log_i("Start App server.");
    app::start();
    log_i("Start App server. Done!");

    // Blink LED to indicate successful boot
    pinMode(LED_TO_BLINK, OUTPUT);
    for (int i = 0; i < 25; i++) {
        digitalWrite(LED_TO_BLINK, LED_TO_BLINK_HIGH);
        delay(50);
        digitalWrite(LED_TO_BLINK, LED_TO_BLINK_LOW);
        delay(50);
    }

    // Delete settings if BOOT button is pressed (Pull-up on GPIO0)
    pinMode(GPIO_NUM_0, INPUT_PULLUP);
    if (digitalRead(GPIO_NUM_0) == LOW) {
        log_i("BOOT button pressed, deleting settings");
        if (!SPIFFS.remove(SPIFFS_SETTINGS_PATH)) {
            log_e("Failed to delete settings");
            blink_error<ERR_SETTINGS>(ERR_SETTINGS_REMOVE, true);
        } else {
            log_i("Settings deleted");

            ESP.restart();
        }
    }
}

void loop() {
    // yield();
    delay(MAIN_LOOP_DELAY);
}

#include "tests/main.hpp"

namespace main::test {
    camera_config_t init_camera_config() { return ::init_camera_config(); }
    void            prep_camera_module() { return ::prep_camera_module(); }
    void            setup_camera_module(camera_config_t* camera_config) {
        return ::setup_camera_module(camera_config);
    }
    sensor_t* config_sensor() { return ::config_sensor(); }
}  // namespace main::test
