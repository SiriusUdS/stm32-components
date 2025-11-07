#include "gpio.h"

typedef enum led_state
{
    OFF,
    ON
}led_state_t;

typedef struct led
{
    gpio_t led_gpio;
    led_state_t state;
}led_t;

void led_on(led_t self);
void led_off(led_t self);
void led_toggle(led_t self);
