#include "gpio.h"

void gpio_pin_init (GPIO_TypeDef *port, uint32_t pin, uint32_t speed, uint32_t mode)
{
    static GPIO_InitTypeDef  GPIO_InitStruct;
    
    GPIO_InitStruct.Mode     = mode;
    GPIO_InitStruct.Pull     = GPIO_NOPULL;
    GPIO_InitStruct.Speed    = speed;
    GPIO_InitStruct.Pin      = pin;
	
	HAL_GPIO_Init(port, &GPIO_InitStruct);	
}

void gpio_alt_init (GPIO_TypeDef *port, uint32_t pin, uint32_t speed, uint32_t mode, uint32_t alt)
{
    static GPIO_InitTypeDef  GPIO_InitStruct;
    
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Mode      = mode;
    GPIO_InitStruct.Speed     = speed;
    GPIO_InitStruct.Pin       = pin;
    GPIO_InitStruct.Alternate = alt;
	
    HAL_GPIO_Init(port, &GPIO_InitStruct);	
}

uint32_t gpio_pin_read(GPIO_TypeDef *port, uint32_t pin)
{
    return ((port->IDR & pin) != 0 ? 1 : 0);
}

void gpio_pin_set (GPIO_TypeDef *port, uint32_t pin, bool value)
{
    if (value == true) {
        port->BSRR = pin;
    } else {
        port->BSRR = pin << 16;
    }
}

void gpio_pin_set (GPIO_TypeDef *port, uint32_t pin)
{
    port->BSRR = pin;
}

void gpio_pin_reset (GPIO_TypeDef *port, uint32_t pin)
{
    port->BSRR = pin << 16;
}

void gpio_port_set (GPIO_TypeDef *port, uint32_t pin)
{
    port->BSRR = pin;
}

void gpio_port_reset (GPIO_TypeDef *port, uint32_t pin)
{
    port->BSRR = pin << 16;
}

void gpio_port_out (GPIO_TypeDef *port, uint32_t pin)
{
    port->ODR = pin;
}


