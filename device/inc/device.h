#ifndef DEVICE_IMPLEMENTATION
#define DEVICE_IMPLEMENTATION

#include "init.h"
#include "gpio.h"
#include "web_interface.h"

#include "step_motor.h"

extern WebServerInterface *__wFace;


void __delay_ns(int us);
void __delay_ms(int ms);

class Device {
    private :
        StepMotorAB stepMotor;
    public :
        Device () 
        {
            core_init();
            clock_enable();
            gpio_init();
            spi_init();
            HAL_Delay(200);
            gpio_pin_set(enc28j60_reset_port, enc28j60_reset_pin);
            HAL_Delay(200);
            SPI_Enc28j60_Init();
            static WebServerInterface web(this);
            __wFace = &web;
            coilSet(true);
            /*
            if (stepMotor.mount((char *)"") < 0) {
                
            }
            */
            goIddle();
            __delay_ms(100);
            /*adbc - x*/
            /*adcb - x*/
            /*acdb - x*/
            /*acbd - x*/
            /*dabc - x*/
            /*dacb - ccw*/
            /*bcad - cw*/
            /*abcd*/
            for (;;) {
                energizeAB();
                energizeBC();
                energizeCD();
                energizeDA();
                //__delay_ms(1);
            }
            web.run();
                
            /*<->*/
        }    
        
        void goIddle ()
        {
            __delay_ns(150000);
            gpio_pin_reset(A_port, A_pin);
            gpio_pin_reset(B_port, B_pin);
            gpio_pin_reset(C_port, C_pin);
            gpio_pin_reset(D_port, D_pin);
            __delay_ns(50000);
        }
        
        void energizeA ()
        {
            gpio_pin_set(A_port, A_pin);
            goIddle();
        }
        void energizeB ()
        {
            gpio_pin_set(B_port, B_pin);
            goIddle();
        }
        void energizeC ()
        {
            gpio_pin_set(C_port, C_pin);
            goIddle();
        }
        void energizeD ()
        {
            gpio_pin_set(D_port, D_pin);
            goIddle();
        }
        
        void energizeAB ()
        {
            gpio_pin_set(A_port, A_pin);
            gpio_pin_set(B_port, B_pin);
            goIddle();
        }
        void energizeBC ()
        {
            gpio_pin_set(B_port, B_pin);
            gpio_pin_set(C_port, C_pin);
            goIddle();
        }
        void energizeCD ()
        {
            gpio_pin_set(C_port, C_pin);
            gpio_pin_set(D_port, D_pin);
            goIddle();
        }
        void energizeDA ()
        {
            gpio_pin_set(D_port, D_pin);
            gpio_pin_set(A_port, A_pin);
            goIddle();
        }
        
        void coilSet (bool value)
        {
            //gpio_pin_set(coil_out_port, coil_out_pin, value);
        }
    
    
    
};


#endif

