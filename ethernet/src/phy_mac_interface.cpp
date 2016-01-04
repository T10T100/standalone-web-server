#include "phy_mac_interface.h"


#define ENC28_J60_PHY_MAC 1

#ifndef PHY_MAC_CHIP
#define PHY_MAC_CHIP ENC28_J60_PHY_MAC
#endif


#if (PHY_MAC_CHIP == ENC28_J60_PHY_MAC)
    #include "enc28j60.h"
    unsigned char *PhyMacInterface::mcuInit (unsigned char *args)
    {
        enc28j60Init(args);
        
        enc28j60PhyWrite(PHLCON, 0x476);	
        return (unsigned char *)"enc28j60.h";
    }
    unsigned int PhyMacInterface::chipPacketReceive(unsigned int maxlen, unsigned char* packet)
    {
        return enc28j60PacketReceive(maxlen, packet);
    }
#else 
    unsigned char *PhyMacInterface::mcuInit (unsigned char *args)
    {
        return (unsigned char *)"No Phy\Mac chip presented yet";	
    }
    unsigned int PhyMacInterface::chipPacketReceive(unsigned int maxlen, unsigned char* packet)
    {
        return 0;
    }
#endif


