#pragma once

#include <esp_camera.h>

namespace main {
    namespace test {
        camera_config_t init_camera_config();
        void            prep_camera_module();
        void            setup_camera_module(camera_config_t* camera_config);
        sensor_t*       config_sensor();
    }  // namespace test
}  // namespace main
