#include "gpio.h"

#ifdef STM32H7
    #include "stm32h7xx_hal.h"
#elif defined(STM32F4)
    #include "stm32f4xx_hal.h"
#endif

void set_gpio_High(gpio_t self)
{
    HAL_GPIO_WritePin(self.port, self.pin, GPIO_PIN_SET);
}
void set_gpio_Low(gpio_t self);