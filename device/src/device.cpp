#include "device.h"
#include "gpio.h"


void __delay_ns(int us)
{
    for (volatile int i = 0; i < us; i++);
}

void __delay_ms(int ms)
{
    HAL_Delay(ms);
}



int StepMotorAB::HLInit ()
{
    /*
    gpio_pin_init(step_motor_sdi_port, step_motor_sdi_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    gpio_pin_init(step_motor_sdo_port, step_motor_sdo_pin, GPIO_SPEED_HIGH, GPIO_MODE_INPUT);
    gpio_pin_init(step_motor_sck_port, step_motor_sck_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    gpio_pin_init(step_motor_csn_port, step_motor_csn_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    */
    return 0;
}

int StepMotorAB::HLDeInit ()
{
    /**/
    return 0;
}
        
int StepMotorAB::setCoilAPhase (int f)
{
    if (f == 0) {
        return 1;
    }
    /*
    if (gpio_pin_read(step_motor_sdo_port, step_motor_sdo_pin) == true) {
        return -2;
    }
    gpio_pin_set(step_motor_sdi_port, step_motor_sdi_pin, f < 0 ? false : true);
    */
    return 0;
}

int StepMotorAB::setCoilBPhase (int f)
{
    if (f == 0) {
        return 1;
    }
    /*
    if (gpio_pin_read(step_motor_sdo_port, step_motor_sdo_pin) == true) {
        return -2;
    }
    gpio_pin_set(step_motor_csn_port, step_motor_csn_pin, f < 0 ? false : true);
    */
    return 0;
}

