#include "led.h"

void led_on(led_t self)
{
    set_gpio_High(self.led_gpio);
    self.state = ON;
}
void led_off(led_t self)
{
    set_gpio_Low(self.led_gpio);
    self.state = OFF;
}
void led_toggle(led_t self)
{
    if (self.state == ON)
    {
        led_off(self);
    }
    else
    {
        led_on(self);
    }
}