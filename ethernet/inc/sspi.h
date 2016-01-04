#ifndef SOFTWARE_SPI
#define SOFTWARE_SPI

#include "stdint.h"



namespace sspi {

    #define HIGH 1
    #define LOW 0

            void delay_ns (uint32_t v);
            void Spi ();
            void write (unsigned char v);
            unsigned char read ();
            unsigned char readWrite (unsigned char);

};


#endif

