#include <Arduino.h>
#include <unity.h>

#include "hw/camera.hpp"

#include "json.hpp"

#include "tests/main.hpp"
#include "tests/app.hpp"

sensor_t *sensor = nullptr;

void suiteSetUp(void) {
    camera_config_t camera_config = main::test::init_camera_config();
    main::test::prep_camera_module();
    main::test::setup_camera_module(&camera_config);
    sensor = main::test::config_sensor();
}

void test_settings_schema(void) {
    JsonDocument         doc;
    DeserializationError error = deserializeJson(doc, app::test::Settings_schema);
    TEST_ASSERT_EQUAL(DeserializationError::Ok, error.code());
}

void test_sensor_schema(void) {
    JsonDocument         doc;
    DeserializationError error = deserializeJson(doc, app::test::Sensor_schema);
    TEST_ASSERT_EQUAL(DeserializationError::Ok, error.code());
}

void test_generate_openapi_json(void) {
    String openapi_json_str = app::test::generate_openapi_json(sensor);
    TEST_ASSERT_TRUE(openapi_json_str);
    {
        JsonDocument         doc;
        DeserializationError error = deserializeJson(doc, openapi_json_str);
        TEST_ASSERT_EQUAL(DeserializationError::Ok, error.code());

        JsonObject openapi_json = doc.template as<JsonObject>();
        {
            // TODO: Fill in the blanks
            JsonVariant info = openapi_json["info"];
            TEST_ASSERT_TRUE(info.template is<JsonObject>());
            {}

            JsonVariant servers = openapi_json["servers"];
            TEST_ASSERT_TRUE(servers.template is<JsonArray>());
            {}

            JsonVariant tags = openapi_json["tags"];
            TEST_ASSERT_TRUE(tags.template is<JsonArray>());
            {}

            JsonVariant paths = openapi_json["paths"];
            TEST_ASSERT_TRUE(paths.template is<JsonObject>());
            {}

            JsonVariant components = openapi_json["components"];
            TEST_ASSERT_TRUE(components.template is<JsonObject>());
            {}
        }
    }
}

void test_generate_settings_json(void) {
    String settings_json = app::test::generate_settings_json(sensor, false);
    TEST_ASSERT_TRUE(settings_json);
    {
        JsonDocument         doc;
        DeserializationError error = deserializeJson(doc, settings_json);
        TEST_ASSERT_EQUAL(DeserializationError::Ok, error.code());

        doc = g_settings;
        doc.shrinkToFit();

        String json;
        serializeJson(doc, json);
        TEST_ASSERT_EQUAL_STRING(settings_json.c_str(), json.c_str());
    }
}

void test_generate_settings_json_with_types(void) {
    String settings_json = app::test::generate_settings_json(sensor, true);
    {
        JsonDocument         doc;
        DeserializationError error = deserializeJson(doc, settings_json);
        TEST_ASSERT_EQUAL(DeserializationError::Ok, error.code());

        JsonObject types = doc["$types"].template as<JsonObject>();
        TEST_ASSERT(types);
    }
}

void test_generate_sensor_json(void) {
    String sensor_json = app::test::generate_sensor_json(sensor);
    TEST_ASSERT_TRUE(sensor_json);
    {
        JsonDocument         doc;
        DeserializationError error = deserializeJson(doc, sensor_json);
        TEST_ASSERT_EQUAL(DeserializationError::Ok, error.code());
    }
}

void test_process_sensor_json(void) {
    String               sensor_json = app::test::generate_sensor_json(sensor);
    JsonDocument         doc;
    DeserializationError error = deserializeJson(doc, sensor_json);
    TEST_ASSERT_EQUAL(DeserializationError::Ok, error.code());
    JsonObject obj = doc.as<JsonObject>();
    TEST_ASSERT_TRUE(app::test::process_sensor_json(sensor, obj));
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    pinMode(LED_BUILTIN, OUTPUT);

    UNITY_BEGIN();
    suiteSetUp();
    // Run test suite

    RUN_TEST(test_settings_schema);
    RUN_TEST(test_sensor_schema);

    RUN_TEST(test_generate_openapi_json);

    RUN_TEST(test_generate_settings_json);
    RUN_TEST(test_generate_settings_json_with_types);

    RUN_TEST(test_generate_sensor_json);
    RUN_TEST(test_process_sensor_json);

    UNITY_END();
}
