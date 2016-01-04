#ifndef GPIO_INTERFACE
#define GPIO_INTERFACE

#include "main.h"

void gpio_pin_init (GPIO_TypeDef *port, uint32_t pin, uint32_t speed, uint32_t mode);
void gpio_alt_init (GPIO_TypeDef *port, uint32_t pin, uint32_t speed, uint32_t mode, uint32_t alt);

void gpio_pin_set (GPIO_TypeDef *port, uint32_t pin, bool value);
void gpio_pin_set (GPIO_TypeDef *port, uint32_t pin);
void gpio_pin_reset (GPIO_TypeDef *port, uint32_t pin);

uint32_t gpio_pin_read(GPIO_TypeDef *port, uint32_t pin);

void gpio_port_set (GPIO_TypeDef *port, uint32_t pin);
void gpio_port_reset (GPIO_TypeDef *port, uint32_t pin);

void gpio_port_out (GPIO_TypeDef *port, uint32_t pin);


/*Fast gpio access macros*/



/*Fast gpio access macros*/



#endif

