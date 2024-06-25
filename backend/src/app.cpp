#include "app.hpp"

#include <memory>

#include <esp_log.h>
#include <esp_http_server.h>
#include <esp_camera.h>

#include <FS.h>
#include <SPIFFS.h>

#include "hw/camera.hpp"
#include "types/camera.hpp"
#include "types/wifi.hpp"
#include "config.hpp"
#include "json.hpp"

#include <StreamUtils.h>

#include "bundle.h"
#include "config.hpp"
#include "error.hpp"

using unique_buf_t = std::unique_ptr<char, decltype(&free)>;

static constexpr const char *const Settings_schema = []() {
    return R"json(
{}
)json";
}();

static constexpr const char *const Sensor_schema = []() {
    return R"json(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "type": "object",
    "properties": {
        "framesize": {
            "type": "integer",
            "minimum": 0,
            "maximum": 21,
            "format": "int32"
        },
        "brightness": {
            "type": "integer",
            "minimum": -2,
            "maximum": 2,
            "format": "int8"
        },
        "contrast": {
            "type": "integer",
            "minimum": -2,
            "maximum": 2,
            "format": "int8"
        },
        "saturation": {
            "type": "integer",
            "minimum": -2,
            "maximum": 2,
            "format": "int8"
        },
        "sharpness": {
            "type": "integer",
            "minimum": -2,
            "maximum": 2,
            "format": "int8"
        },
        "denoise": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "gainceiling": {
            "type": "integer",
            "minimum": 0,
            "maximum": 6,
            "format": "int8"
        },
        "quality": {
            "type": "integer",
            "minimum": 1,
            "maximum": 63,
            "format": "uint8"
        },
        "colorbar": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "whitebal": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "gain_ctrl": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "exposure_ctrl": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "hmirror": {
            "type": "integer",
            "minimum": 0,
            "maximum": 1,
            "format": "uint8"
        },
        "vflip": {
            "type": "integer",
            "minimum": 0,
            "maximum": 1,
            "format": "uint8"
        },
        "aec2": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "awb_gain": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "agc_gain": {
            "type": "integer",
            "minimum": 0,
            "maximum": 30,
            "format": "uint8"
        },
        "aec_value": {
            "type": "integer",
            "minimum": 0,
            "maximum": 65535,
            "format": "uint16"
        },
        "special_effect": {
            "type": "integer",
            "minimum": 0,
            "maximum": 6,
            "format": "uint8"
        },
        "wb_mode": {
            "type": "integer",
            "minimum": 0,
            "maximum": 4,
            "format": "uint8"
        },
        "ae_level": {
            "type": "integer",
            "minimum": -2,
            "maximum": 2,
            "format": "int8"
        },
        "dcw": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "bpc": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "wpc": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "raw_gma": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        },
        "lenc": {
            "type": "integer",
            "minimum": 0,
            "maximum": 255,
            "format": "uint8"
        }
    },
    "additionalProperties": true,
    "required": [
        "framesize",
        "brightness",
        "saturation",
        "sharpness",
        "denoise",
        "gainceiling",
        "quality",
        "colorbar",
        "whitebal",
        "gain_ctrl",
        "exposure_ctrl",
        "hmirror",
        "vflip",
        "aec2",
        "awb_gain",
        "agc_gain",
        "aec_value",
        "special_effect",
        "wb_mode",
        "ae_level",
        "dcw",
        "bpc",
        "wpc",
        "raw_gma",
        "lenc"
    ]
}
)json";
}();

inline String generate_openapi_json(sensor_t *s) {
    JsonDocument doc;
    doc["openapi"]  = "3.1.0";
    JsonObject info = doc["info"].template to<JsonObject>();
    {
        info["title"]       = "ESP32-CAM-ML";
        info["description"] = "ESP32-CAM-ML API";
        info["version"] =
            String(MAJOR_VERSION) + "." + String(MINOR_VERSION) + "." + String(PATCH_VERSION);
    }
    // JsonObject externalDocs = doc["externalDocs"].template to<JsonObject>();
    //{}
    JsonArray servers = doc["servers"].template to<JsonArray>();
    {
        IPAddress ipv4 = WiFi.localIP();
        IPAddress ipv6 = WiFi.linkLocalIPv6();
        if (WiFi.getMode() == WIFI_AP) {
            ipv4 = WiFi.softAPIP();
            ipv6 = WiFi.softAPlinkLocalIPv6();
        }
        JsonObject ipv4_server = servers[0].template to<JsonObject>();
        {
            ipv4_server["url"] = "http://" + ipv4.toString() + ":{port}";
            JsonObject vars    = ipv4_server["variables"].template to<JsonObject>();
            {
                JsonArray ports = vars["port"]["enum"].template to<JsonArray>();
                {
                    ports.add(80);
                    ports.add(81);
                    ports.add(3232);
                }
                vars["port"]["default"] = "80";
            }
        }
        if (ipv6) {
            JsonObject ipv6_server = servers[1].template to<JsonObject>();
            {
                ipv6_server["url"] = "http://[" + ipv6.toString() + "]" + ":{port}";
                JsonObject vars    = ipv6_server["variables"].template to<JsonObject>();
                {
                    JsonArray ports = vars["port"]["enum"].template to<JsonArray>();
                    {
                        ports.add(80);
                        ports.add(81);
                        ports.add(3232);
                    }
                    vars["port"]["default"] = "80";
                }
            }
        }
    }
    JsonArray tags = doc["tags"].template to<JsonArray>();
    {
        tags[0]["name"]        = "App";
        tags[0]["description"] = "App API";

        tags[1]["name"]        = "API";
        tags[1]["description"] = "Self API";

        tags[2]["name"]        = "Settings";
        tags[2]["description"] = "Settings API";

        tags[3]["name"]        = "Sensor";
        tags[3]["description"] = "Sensor API";

        tags[4]["name"]        = "Stream";
        tags[4]["description"] = "Stream API";

        tags[5]["name"]        = "OTA";
        tags[5]["description"] = "OTA API";
    }
    JsonObject paths = doc["paths"].template to<JsonObject>();
    {
        JsonObject app = paths["/"].template to<JsonObject>();
        {
            JsonObject get = app["get"].template to<JsonObject>();
            {
                get["tags"][0]       = "App";
                get["summary"]       = "Get App";
                get["description"]   = "Web App of ESP32-CAM-ML";
                get["operationId"]   = "getApp";
                JsonObject responses = get["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Open App";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        { content["text/html"]["schema"]["type"] = "string"; }
                    }
                }
            }
        }
        JsonObject api = paths["/api"].template to<JsonObject>();
        {
            JsonObject get = api["get"].template to<JsonObject>();
            {
                get["tags"][0]       = "API";
                get["summary"]       = "Get API Docs";
                get["description"]   = "Web App API Docs of ESP32-CAM-ML";
                get["operationId"]   = "getAPI";
                JsonObject responses = get["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Open API Docs";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        { content["text/html"]["schema"]["type"] = "string"; }
                    }
                }
            }
        }
        JsonObject openapi = paths["/api/openapi.json"].template to<JsonObject>();
        {
            JsonObject get = openapi["get"].template to<JsonObject>();
            {
                get["tags"][0]       = "API";
                get["summary"]       = "Get OpenAPI JSON";
                get["description"]   = "Web App OpenAPI JSON Docs of ESP32-CAM-ML";
                get["operationId"]   = "getAPIjson";
                JsonObject responses = get["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Get OpenAPI JSON Docs";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        { content["application/json"]["schema"]["type"] = "string"; }
                    }
                }
            }
        }
        JsonObject settings = paths["/settings"].template to<JsonObject>();
        {
            JsonObject get = settings["get"].template to<JsonObject>();
            {
                get["tags"][0]       = "Settings";
                get["summary"]       = "Get Settings JSON";
                get["description"]   = "Settings JSON of ESP32-CAM-ML";
                get["operationId"]   = "getSettings";
                JsonObject responses = get["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Get Settings JSON";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        { content["application/json"]["schema"]["type"] = "string"; }
                    }
                }
            }
            JsonObject post = settings["post"].template to<JsonObject>();
            {
                post["tags"][0]        = "Settings";
                post["summary"]        = "Set Settings JSON";
                post["description"]    = "Set Settings JSON of ESP32-CAM-ML";
                post["operationId"]    = "setSettings";
                JsonObject requestBody = post["requestBody"].template to<JsonObject>();
                {
                    requestBody["content"]["application/json"]["schema"]["$ref"] =
                        "#/components/schemas/Settings";
                    requestBody["required"] = true;
                }
                JsonObject responses = post["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Applied Settings JSON";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        { content["application/json"]["schema"]["type"] = "string"; }
                    }
                    JsonObject res_400 = responses["400"].template to<JsonObject>();
                    {
                        res_400["description"] = "Bad Request";
                        JsonObject content     = res_400["content"].template to<JsonObject>();
                        { content["text/plain"]["schema"]["type"] = "string"; }
                    }
                    JsonObject res_500 = responses["500"].template to<JsonObject>();
                    {
                        res_500["description"] = "Internal Server Error";
                        JsonObject content     = res_500["content"].template to<JsonObject>();
                        { content["text/plain"]["schema"]["type"] = "string"; }
                    }
                }
            }
        }
        JsonObject sensor = paths["/sensor"].template to<JsonObject>();
        {
            JsonObject get = sensor["get"].template to<JsonObject>();
            {
                get["tags"][0]       = "Sensor";
                get["summary"]       = "Get Sensor Settings";
                get["description"]   = "Sensor Settings of ESP32-CAM-ML";
                get["operationId"]   = "getSensorSettings";
                JsonObject responses = get["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Sensor Settings";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        { content["application/json"]["schema"]["type"] = "string"; }
                    }
                    JsonObject res_400 = responses["400"].template to<JsonObject>();
                    {
                        res_400["description"] = "Bad Request";
                        JsonObject content     = res_400["content"].template to<JsonObject>();
                        { content["text/plain"]["schema"]["type"] = "string"; }
                    }
                }
            }
            JsonObject post = sensor["post"].template to<JsonObject>();
            {
                post["tags"][0]        = "Sensor";
                post["summary"]        = "Set Sensor Settings";
                post["description"]    = "Set Sensor Settings of ESP32-CAM-ML";
                post["operationId"]    = "setSensorSettings";
                JsonObject requestBody = post["requestBody"].template to<JsonObject>();
                {
                    requestBody["content"]["application/json"]["schema"]["$ref"] =
                        "#/components/schemas/Sensor";
                    requestBody["required"] = true;
                }
                JsonObject responses = post["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Applied Sensor Settings";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        { content["application/json"]["schema"]["type"] = "string"; }
                    }
                    JsonObject res_400 = responses["400"].template to<JsonObject>();
                    {
                        res_400["description"] = "Bad Request";
                        JsonObject content     = res_400["content"].template to<JsonObject>();
                        { content["text/plain"]["schema"]["type"] = "string"; }
                    }
                }
            }
        }
        JsonObject stream = paths["/stream"].template to<JsonObject>();
        {
            JsonObject get = stream["get"].template to<JsonObject>();
            {
                get["tags"][0]       = "Stream";
                get["summary"]       = "Camera Stream";
                get["description"]   = "Camera Live Stream";
                get["operationId"]   = "getStream";
                JsonObject responses = get["responses"].template to<JsonObject>();
                {
                    JsonObject res_200 = responses["200"].template to<JsonObject>();
                    {
                        res_200["description"] = "Camera Stream";
                        JsonObject content     = res_200["content"].template to<JsonObject>();
                        {
                            content["multipart/x-mixed-replace"]["schema"]["type"]   = "string";
                            content["multipart/x-mixed-replace"]["schema"]["format"] = "binary";
                        }
                    }
                }
            }
        }
        // JsonObject ota = paths["/ota"].template to<JsonObject>();
        //{}
    }
    JsonObject components = doc["components"].template to<JsonObject>();
    {
        JsonObject schemas = components["schemas"].template to<JsonObject>();
        {
            JsonObject Settings = schemas["Settings"].template to<JsonObject>();
            {
                Settings["type"] = "object";
                JsonDocument         tmp;
                DeserializationError error = deserializeJson(tmp, Settings_schema);
                if (error) {
                    log_e("deserializeJson() failed: %s", error.c_str());
                } else {
                    if (tmp.containsKey("properties")) {
                        Settings["properties"] = tmp["properties"].template as<JsonObject>();
                    } else {
                        Settings["properties"] = tmp.template as<JsonObject>();
                    }
                }
            }
            JsonObject Sensor = schemas["Sensor"].template to<JsonObject>();
            {
                Sensor["type"] = "object";
                JsonDocument         tmp;
                DeserializationError error = deserializeJson(tmp, Sensor_schema);
                if (error) {
                    log_e("deserializeJson() failed: %s", error.c_str());
                } else {
                    if (tmp.containsKey("properties")) {
                        auto si       = esp_camera_sensor_get_info(&s->id);
                        auto max_size = get_max_framesize(si);
                        tmp["properties"]["framesize"]["maximum"] = max_size;
                        Sensor["properties"] = tmp["properties"].template as<JsonObject>();
                    } else {
                        Sensor["properties"] = tmp.template as<JsonObject>();
                    }
                }
            }
        }
    }
    doc.shrinkToFit();

    String json;
    serializeJson(doc, json);

    return json;
}

inline String generate_settings_json(sensor_t *s, bool types = false) {
    JsonDocument doc;
    doc = g_settings;
    // Add enum types
    if (types) {
        JsonObject types = doc["$types"].template to<JsonObject>();
        {
            JsonObject wifi = types["wifi"].template to<JsonObject>();
            {
                JsonObject mode = wifi["mode"].template to<JsonObject>();
                {
                    JsonArray mode_enum = mode["$enum"].template to<JsonArray>();
                    for (size_t i = 0; i < sizeof(wifi_mode_names) / sizeof(wifi_mode_names[0]);
                         i++) {
                        mode_enum.add(wifi_mode_names[i]);
                    }
                }
                JsonObject security = wifi["security"].template to<JsonObject>();
                {
                    JsonArray security_enum = security["$enum"].template to<JsonArray>();
                    for (size_t i = 0;
                         i < sizeof(wifi_security_names) / sizeof(wifi_security_names[0]);
                         i++) {
                        security_enum.add(wifi_security_names[i]);
                    }
                }
            }
            JsonObject camera = types["camera"].template to<JsonObject>();
            {
                JsonArray pixel_format = camera["pixel_format"].template to<JsonArray>();
                {
                    for (size_t i = 0; i < sizeof(pixformat_names) / sizeof(pixformat_names[0]);
                         i++) {
                        pixel_format.add(pixformat_names[i]);
                    }
                }
                JsonArray frame_size = camera["frame_size"].template to<JsonArray>();
                {
                    auto si       = esp_camera_sensor_get_info(&s->id);
                    auto max_size = get_max_framesize(si);
                    for (size_t i = 0; i < max_size; i++) {
                        frame_size.add(framesize_names[i]);
                    }
                }
                JsonArray fb_location = camera["fb_location"].template to<JsonArray>();
                {
                    for (size_t i = 0; i < sizeof(fb_location_names) / sizeof(fb_location_names[0]);
                         i++) {
                        fb_location.add(fb_location_names[i]);
                    }
                }
                JsonArray grab_mode = camera["grab_mode"].template to<JsonArray>();
                {
                    for (size_t i = 0; i < sizeof(grab_mode_names) / sizeof(grab_mode_names[0]);
                         i++) {
                        grab_mode.add(grab_mode_names[i]);
                    }
                }
            }
        }
    }
    {
        // Cleanup Camera options
        {
            // Remove fb_count
            doc["camera"].remove("fb_count");
            // Remove fb_location
            doc["camera"].remove("fb_location");
            // Remove grab_mode
            doc["camera"].remove("grab_mode");
        }
        // Remove OTA settings
        doc.remove("ota");
    }
    doc.shrinkToFit();

    String json;
    serializeJson(doc, json);

    return json;
}

inline String generate_sensor_json(sensor_t *s) {
    JsonDocument doc;
    doc["pixformat"]     = s->pixformat;
    doc["framesize"]     = s->status.framesize;
    doc["brightness"]    = s->status.brightness;
    doc["contrast"]      = s->status.contrast;
    doc["saturation"]    = s->status.saturation;
    doc["sharpness"]     = s->status.sharpness;
    doc["denoise"]       = s->status.denoise;
    doc["gainceiling"]   = s->status.gainceiling;
    doc["quality"]       = s->status.quality;
    doc["colorbar"]      = s->status.colorbar;
    doc["whitebal"]      = s->status.awb;
    doc["gain_ctrl"]     = s->status.agc;
    doc["exposure_ctrl"] = s->status.aec;
    doc["hmirror"]       = s->status.hmirror;
    doc["vflip"]         = s->status.vflip;

    doc["aec2"]      = s->status.aec2;
    doc["awb_gain"]  = s->status.awb_gain;
    doc["agc_gain"]  = s->status.agc_gain;
    doc["aec_value"] = s->status.aec_value;

    doc["special_effect"] = s->status.special_effect;
    doc["wb_mode"]        = s->status.wb_mode;
    doc["ae_level"]       = s->status.ae_level;

    doc["dcw"] = s->status.dcw;
    doc["bpc"] = s->status.bpc;
    doc["wpc"] = s->status.wpc;

    doc["raw_gma"] = s->status.raw_gma;
    doc["lenc"]    = s->status.lenc;

    doc.shrinkToFit();

    String json;
    serializeJson(doc, json);

    return json;
}

inline bool process_sensor_json(sensor_t *s, JsonObject obj) {
    for (auto kv : obj) {
        const char *key   = kv.key().c_str();
        uint32_t    value = kv.value().as<uint32_t>();
        if (strcmp(key, "pixformat") == 0) {
            s->set_pixformat(s, static_cast<decltype(s->pixformat)>(value));
        } else if (strcmp(key, "framesize") == 0) {
            auto fs       = static_cast<decltype(s->status.framesize)>(value);
            auto si       = esp_camera_sensor_get_info(&s->id);
            auto max_size = get_max_framesize(si);
            if (fs > max_size) {
                log_w("Invalid framesize: %d", fs);
                return false;
            }
            s->set_framesize(s, fs);
        } else if (strcmp(key, "brightness") == 0) {
            s->set_brightness(s, static_cast<decltype(s->status.brightness)>(value));
        } else if (strcmp(key, "contrast") == 0) {
            s->set_contrast(s, static_cast<decltype(s->status.contrast)>(value));
        } else if (strcmp(key, "saturation") == 0) {
            s->set_saturation(s, static_cast<decltype(s->status.saturation)>(value));
        } else if (strcmp(key, "sharpness") == 0) {
            s->set_sharpness(s, static_cast<decltype(s->status.sharpness)>(value));
        } else if (strcmp(key, "denoise") == 0) {
            s->set_denoise(s, static_cast<decltype(s->status.denoise)>(value));
        } else if (strcmp(key, "gainceiling") == 0) {
            s->set_gainceiling(s, static_cast<gainceiling_t>(value));
        } else if (strcmp(key, "quality") == 0) {
            s->set_quality(s, static_cast<decltype(s->status.quality)>(value));
        } else if (strcmp(key, "colorbar") == 0) {
            s->set_colorbar(s, static_cast<decltype(s->status.colorbar)>(value));
        } else if (strcmp(key, "whitebal") == 0) {
            s->set_whitebal(s, static_cast<decltype(s->status.awb)>(value));
        } else if (strcmp(key, "gain_ctrl") == 0) {
            s->set_gain_ctrl(s, static_cast<decltype(s->status.agc)>(value));
        } else if (strcmp(key, "exposure_ctrl") == 0) {
            s->set_exposure_ctrl(s, static_cast<decltype(s->status.aec)>(value));
        } else if (strcmp(key, "hmirror") == 0) {
            s->set_hmirror(s, static_cast<decltype(s->status.hmirror)>(value));
        } else if (strcmp(key, "vflip") == 0) {
            s->set_vflip(s, static_cast<decltype(s->status.vflip)>(value));
        } else if (strcmp(key, "aec2") == 0) {
            s->set_aec2(s, static_cast<decltype(s->status.aec2)>(value));
        } else if (strcmp(key, "awb_gain") == 0) {
            s->set_awb_gain(s, static_cast<decltype(s->status.awb_gain)>(value));
        } else if (strcmp(key, "agc_gain") == 0) {
            s->set_agc_gain(s, static_cast<decltype(s->status.agc_gain)>(value));
        } else if (strcmp(key, "aec_value") == 0) {
            s->set_aec_value(s, static_cast<decltype(s->status.aec_value)>(value));
        } else if (strcmp(key, "special_effect") == 0) {
            s->set_special_effect(s, static_cast<decltype(s->status.special_effect)>(value));
        } else if (strcmp(key, "wb_mode") == 0) {
            s->set_wb_mode(s, static_cast<decltype(s->status.wb_mode)>(value));
        } else if (strcmp(key, "ae_level") == 0) {
            s->set_ae_level(s, static_cast<decltype(s->status.ae_level)>(value));
        } else if (strcmp(key, "dcw") == 0) {
            s->set_dcw(s, static_cast<decltype(s->status.dcw)>(value));
        } else if (strcmp(key, "bpc") == 0) {
            s->set_bpc(s, static_cast<decltype(s->status.bpc)>(value));
        } else if (strcmp(key, "wpc") == 0) {
            s->set_wpc(s, static_cast<decltype(s->status.wpc)>(value));
        } else if (strcmp(key, "raw_gma") == 0) {
            s->set_raw_gma(s, static_cast<decltype(s->status.raw_gma)>(value));
        } else if (strcmp(key, "lenc") == 0) {
            s->set_lenc(s, static_cast<decltype(s->status.lenc)>(value));
        } else {
            log_w("Unknown key: %s", key);
        }
    }

    return true;
}

static esp_err_t api_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    if (api_full_html_gz_size > 0) {
        return httpd_resp_send(req,
                               reinterpret_cast<const char *>(api_full_html_gz),
                               api_full_html_gz_size);
    }
    return httpd_resp_send(req,
                           reinterpret_cast<const char *>(api_stub_html_gz),
                           api_stub_html_gz_size);
}

static esp_err_t openapi_json_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

    auto s = esp_camera_sensor_get();
    if (!s) {
        log_e("Failed to get sensor");

        return httpd_resp_send_500(req);
    }
    const String json = generate_openapi_json(s);

    return httpd_resp_send(req, json.c_str(), json.length());
}

static esp_err_t index_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    if (index_full_html_gz_size > 0) {
        return httpd_resp_send(req,
                               reinterpret_cast<const char *>(index_full_html_gz),
                               index_full_html_gz_size);
    }
    return httpd_resp_send(req,
                           reinterpret_cast<const char *>(index_stub_html_gz),
                           index_stub_html_gz_size);
}

static esp_err_t settings_handler(httpd_req_t *req) {
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    switch (req->method) {
        case HTTP_GET: {
            httpd_resp_set_type(req, "application/json");

            auto s = esp_camera_sensor_get();
            if (!s) {
                log_e("Failed to get sensor");

                return httpd_resp_send_500(req);
            }
            const String json = generate_settings_json(s, true);

            return httpd_resp_send(req, json.c_str(), json.length());
        }
        case HTTP_POST: {
            size_t buf_len = req->content_len;
            if (buf_len > 0) {
                unique_buf_t buf(reinterpret_cast<char *>(malloc(buf_len)), &free);
                if (buf) {
                    int res = httpd_req_recv(req, buf.get(), buf_len);
                    log_i("httpd_req_recv(%p, %p, %d) => %d", req, buf.get(), buf_len, res);
                    if (res == buf_len) {
                        {
                            Settings_t new_settings(g_settings);

                            JsonDocument         doc;
                            DeserializationError error = deserializeJson(doc, buf.get(), buf_len);
                            if (error) {
                                log_e("deserializeJson() failed: %s", error.c_str());

                                return httpd_resp_send_err(req,
                                                           HTTPD_400_BAD_REQUEST,
                                                           "Bad Request");
                            }

                            new_settings = doc.template as<Settings_t>();

                            // Check magic number
                            if (g_settings.magic != new_settings.magic) {
                                log_w(
                                    "Settings magic number mismatch, need 0x%016llx, got 0x%016llx",
                                    g_settings.magic,
                                    new_settings.magic);

                                return httpd_resp_send_err(req,
                                                           HTTPD_400_BAD_REQUEST,
                                                           "Bad Request");
                            }

                            // Apply some changes immediately
                            {
                                auto s = esp_camera_sensor_get();
                                if (g_settings.camera.pixel_format !=
                                    new_settings.camera.pixel_format) {
                                    log_i("Camera pixel format changed from %d to %d",
                                          g_settings.camera.pixel_format,
                                          new_settings.camera.pixel_format);
                                    if (s) {
                                        s->set_pixformat(s, new_settings.camera.pixel_format);
                                    }
                                }
                                if (g_settings.camera.frame_size !=
                                    new_settings.camera.frame_size) {
                                    log_i("Camera frame size changed from %d to %d",
                                          g_settings.camera.frame_size,
                                          new_settings.camera.frame_size);
                                    if (s) {
                                        auto fs = static_cast<decltype(s->status.framesize)>(
                                            new_settings.camera.frame_size);
                                        auto si       = esp_camera_sensor_get_info(&s->id);
                                        auto max_size = get_max_framesize(si);
                                        if (fs > max_size) {
                                            log_w("Invalid framesize: %d", fs);
                                            return httpd_resp_send_err(req,
                                                                       HTTPD_400_BAD_REQUEST,
                                                                       "Bad Request");
                                        }
                                        s->set_framesize(s, fs);
                                    }
                                }
                                if (g_settings.camera.jpeg_quality !=
                                    new_settings.camera.jpeg_quality) {
                                    log_i("Camera jpeg quality changed from %d to %d",
                                          g_settings.camera.jpeg_quality,
                                          new_settings.camera.jpeg_quality);
                                    if (s) {
                                        s->set_quality(s, new_settings.camera.jpeg_quality);
                                    }
                                }
                            }

                            g_settings = std::move(new_settings);

                            {
                                File file = SPIFFS.open(SPIFFS_SETTINGS_PATH, "w", true);
                                if (!file) {
                                    log_e("Failed to open file for writing");

                                    return httpd_resp_send_500(req);
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

                                    return httpd_resp_send_500(req);
                                }
                            }
                        }

                        File file = SPIFFS.open(SPIFFS_SETTINGS_PATH, "r");
                        if (!file) {
                            log_e("Failed to open settings file");

                            return httpd_resp_send_500(req);
                        }
                        String json = file.readString();
                        file.close();

                        httpd_resp_set_type(req, "application/json");
                        return httpd_resp_send(req, json.c_str(), json.length());
                    }
                    return httpd_resp_send_500(req);
                }
            }
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Bad Request");
        }
        default:
            return httpd_resp_send_err(req, HTTPD_405_METHOD_NOT_ALLOWED, "Method Not Allowed");
    }
}

static esp_err_t sensor_handler(httpd_req_t *req) {
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    switch (req->method) {
        case HTTP_GET: {
            httpd_resp_set_type(req, "application/json");

            auto s = esp_camera_sensor_get();
            if (s) {
                const String json = generate_sensor_json(s);

                return httpd_resp_send(req, json.c_str(), json.length());
            } else {
                return httpd_resp_send_500(req);
            }
        }
        case HTTP_POST: {
            size_t buf_len = req->content_len;
            if (buf_len > 0) {
                unique_buf_t buf(reinterpret_cast<char *>(malloc(buf_len)), &free);
                if (buf) {
                    int res = httpd_req_recv(req, buf.get(), buf_len);
                    log_i("httpd_req_recv(%p, %p, %d) => %d", req, buf.get(), buf_len, res);
                    if (res == buf_len) {
                        auto s = esp_camera_sensor_get();
                        if (s) {
                            JsonDocument         doc;
                            DeserializationError error = deserializeJson(doc, buf.get(), buf_len);
                            if (error) {
                                log_e("deserializeJson() failed: %s", error.c_str());

                                return httpd_resp_send_err(req,
                                                           HTTPD_400_BAD_REQUEST,
                                                           "Bad Request");
                            }

                            if (!process_sensor_json(s, doc.as<JsonObject>())) {
                                return httpd_resp_send_err(req,
                                                           HTTPD_400_BAD_REQUEST,
                                                           "Bad Request");
                            }
                            httpd_resp_set_type(req, "application/json");

                            const String json = generate_sensor_json(s);

                            return httpd_resp_send(req, json.c_str(), json.length());
                        }
                    }
                    return httpd_resp_send_500(req);
                }
                return httpd_resp_send_500(req);
            }
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Bad Request");
        }
        default:
            return httpd_resp_send_err(req, HTTPD_405_METHOD_NOT_ALLOWED, "Method Not Allowed");
    }
}

httpd_handle_t app_httpd = nullptr;

void app::start() {
    httpd_config_t config   = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 16;

    const httpd_uri_t index_uri = {
      .uri      = "/",
      .method   = HTTP_GET,
      .handler  = index_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    const httpd_uri_t api_uri = {
      .uri      = "/api",
      .method   = HTTP_GET,
      .handler  = api_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    const httpd_uri_t openapi_uri = {
      .uri      = "/api/openapi.json",
      .method   = HTTP_GET,
      .handler  = openapi_json_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    const httpd_uri_t settings_get_uri = {
      .uri      = "/settings",
      .method   = HTTP_GET,
      .handler  = settings_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    const httpd_uri_t settings_post_uri = {
      .uri      = "/settings",
      .method   = HTTP_POST,
      .handler  = settings_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    const httpd_uri_t sensor_get_uri = {
      .uri      = "/sensor",
      .method   = HTTP_GET,
      .handler  = sensor_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    const httpd_uri_t sensor_post_uri = {
      .uri      = "/sensor",
      .method   = HTTP_POST,
      .handler  = sensor_handler,
      .user_ctx = nullptr,
#ifdef CONFIG_HTTPD_WS_SUPPORT
      .is_websocket             = true,
      .handle_ws_control_frames = false,
      .supported_subprotocol    = nullptr,
#endif
    };

    log_i("Starting App Server on port: '%d'", config.server_port);
    esp_err_t res = httpd_start(&app_httpd, &config);
    if (res == ESP_OK) {
        res = httpd_register_uri_handler(app_httpd, &index_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;
        res = httpd_register_uri_handler(app_httpd, &api_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;
        res = httpd_register_uri_handler(app_httpd, &openapi_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;
        res = httpd_register_uri_handler(app_httpd, &settings_get_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;
        res = httpd_register_uri_handler(app_httpd, &settings_post_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;
        res = httpd_register_uri_handler(app_httpd, &sensor_get_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;
        res = httpd_register_uri_handler(app_httpd, &sensor_post_uri);
        if (res != ESP_OK) goto ota_register_uri_handler_failed;

        if (false) {
        ota_register_uri_handler_failed:
            log_e("Failed to register URI handler, err: %d", res);
            blink_error<ERR_APP_SERVER>(ERR_APP_REG_URI, true);
        }
    } else {
        log_e("Failed to start OTA Server, err: %d", res);
        blink_error<ERR_APP_SERVER>(ERR_APP_START, true);
    }
}

#include "tests/app.hpp"

namespace app::test {
    const char *Settings_schema = ::Settings_schema;
    const char *Sensor_schema   = ::Sensor_schema;

    String generate_openapi_json(sensor_t *s) { return ::generate_openapi_json(s); }
    String generate_settings_json(sensor_t *s, bool types) {
        return ::generate_settings_json(s, types);
    }
    String generate_sensor_json(sensor_t *s) { return ::generate_sensor_json(s); }
    bool process_sensor_json(sensor_t *s, JsonObject obj) { return ::process_sensor_json(s, obj); }
}  // namespace app::test
