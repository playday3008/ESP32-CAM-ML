#pragma once

#include <WString.h>

#include "json.hpp"

namespace app {
    namespace test {
        extern const char *Settings_schema;
        extern const char *Sensor_schema;

        String generate_openapi_json();
        String generate_settings_json(bool types = false);
        // TODO: process_settings_json
        String generate_sensor_json(sensor_t *s);
        bool   process_sensor_json(sensor_t *s, JsonObject obj);
    }  // namespace test
}  // namespace app
