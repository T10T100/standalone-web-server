#include "sspi.h"
#include "device.h"


            void sspi::delay_ns (uint32_t v)
            {
                while (v--);
            }
        
            void sspi::Spi ()
            {
                //SCK(LOW);
                //MOSI(LOW);
                //CS(HIGH);
            }
            
            void sspi::write (unsigned char v)
            {
                //CS(LOW);
                delay_ns(200);
                for (int i = 0; i < 8; i++) {
                    v >>= i;
                    //SCK(LOW);
                    delay_ns(200);
                    //MOSI( v & 1 ? 1 : 0 );
                    asm volatile ("nop \n\t");
                    //SCK(HIGH);
                    delay_ns(200);
                }
                //CS(HIGH);
            }
            unsigned char sspi::read ()
            {
               unsigned char v = 0;
               //CS(LOW);
               delay_ns(200);
               //SCK(HIGH);
               for (int i = 0; i < 8; i++) {
                   //SCK(LOW);
                   delay_ns(200);
                   asm volatile ("nop \n\t");
                   //v |= (MISO() << i);
                   //SCK(HIGH);
                   delay_ns(200);
                   asm volatile ("nop \n\t");
               }
               //CS(HIGH);
               return v;
            }
            unsigned char sspi::readWrite (unsigned char)
            {
                return 0;
            }
            
            
          

            