#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>

#ifdef STM32H7
    #include "stm32h7xx.h"
#elif defined(STM32F4)
    #include "stm32f4xx.h"
#endif

typedef struct gpio
{
    GPIO_TypeDef * port;
    uint16_t pin;
}gpio_t;

void set_gpio_High(gpio_t self);
void set_gpio_Low(gpio_t self);

#endif