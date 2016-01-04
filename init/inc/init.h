#ifndef STATIC_INITITALIZATION
#define STATIC_INITITALIZATION
#include <stdint.h>
#include "main.h"


enum DeviceSpecificPinsNumbers {
    enc28j60_reset_pin = GPIO_PIN_8, /*PI*/
    enc28j60_int_pin = GPIO_PIN_13, /*PC*/
    
    spi3_miso_pin = GPIO_PIN_5, /*PE*/
    spi3_mosi_pin = GPIO_PIN_6, /*PE*/
    spi3_sck_pin = GPIO_PIN_2, /*PE*/
    spi3_cs_pin = GPIO_PIN_3, /*PE*/   
    
    /*coil_out_pin = GPIO_PIN_2,*//*PA*/
    A_pin = GPIO_PIN_2, /*PH*/
    B_pin = GPIO_PIN_3, /*PH*/
    C_pin = GPIO_PIN_2, /*PA*/
    D_pin = GPIO_PIN_4, /*PH*/
    
};


#define enc28j60_reset_port GPIOI
#define enc28j60_int_port GPIOC

#define spi3_miso_port GPIOE
#define spi3_mosi_port GPIOE
#define spi3_sck_port GPIOE
#define spi3_cs_port GPIOE

#define coil_out_port GPIOA

#define A_port GPIOH
#define B_port GPIOH
#define C_port GPIOA
#define D_port GPIOH


void core_init();
static void MPU_Config(void);
void clock_enable ();
void gpio_init ();
void spi_init ();

void SPI_Enc28j60_Init(void);


#endif

