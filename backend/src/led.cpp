#include "led.hpp"

#include <driver/ledc.h>
#include <esp32-hal-ledc.h>

constexpr uint32_t CONFIG_LEDC_FREQ_HZ    = 5000;
constexpr uint8_t  CONFIG_LEDC_RESOLUTION = 8;
constexpr uint32_t CONFIG_LEDC_MAX_DUTY   = (1U << CONFIG_LEDC_RESOLUTION) - 1;

bool     is_ledc_attached = false;
uint8_t  led_pin          = -1;
uint32_t led_duty         = ~0;

void led::enable(const bool enable) {
    // Turn LED On or Off
    if (is_ledc_attached) {
        uint32_t duty = enable ? led_duty : 0;
        if (enable && (led_duty > CONFIG_LEDC_MAX_DUTY)) {
            led_duty = duty = CONFIG_LEDC_MAX_DUTY;
        }
        ledcWrite(led_pin, duty);
    }
}

void led::setup(const uint8_t pin) {
    is_ledc_attached = ledcAttach(pin, CONFIG_LEDC_FREQ_HZ, CONFIG_LEDC_RESOLUTION);
    led_pin          = pin;
}
