#ifndef PHY_MAC_INTERFACE
#define PHY_MAC_INTERFACE

#include "syncClass.h"



class PhyMacInterface : public Synchronizer<PhyMacInterface> {
    private :
        
        virtual unsigned char *mcuInit (unsigned char *);
        unsigned char *parseForPhy (unsigned char *args)
            {
                return args;
                
                /*Must be mac and other general predefinitions
                  parsed from input string   for Phy\Mac levels.
                  some things may be :
                
                
                
                
                */
            }
            virtual unsigned int chipPacketReceive(unsigned int maxlen, unsigned char* packet);
    public :
        PhyMacInterface () : Synchronizer<PhyMacInterface>()
        {
            
        }
        
        void init (unsigned char *args)
        {
            
            mcuInit( parseForPhy(args) );
            
            
        }
    
        
        
        
        void hwReset (int cause)
        {
            /*Hardware reset
              there must be a call to the embed os, 
              where can be changed driver status, 
              to avoid unresolved states of the Phy\Mac mcu.
              In normal mode this reset appears only when the device is powered up.
              The other cases can be understanding as unnormal
             */
            
            
        }
        
        void swReset ()
        {
            /*
              Software reset
              -\\-
            */
        }
        
        
        unsigned int packetReceive(unsigned int maxlen, unsigned char* packet)
        {
            return chipPacketReceive(maxlen, packet);
        }
    
};




#endif


/*End of file*/

