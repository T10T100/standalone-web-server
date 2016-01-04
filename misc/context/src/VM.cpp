#include "VM.h"

extern Runtime runtime;
extern Device  device;

__value_in_regs DwArg vmtick (void *arg)
{
    return runtime.dispatch(arg);
}
__value_in_regs DwArg vmsvc (SVC_arg arg)
{
   return runtime.dispatchSVC(arg); 
}

__value_in_regs DwArg vmfault (SVC_arg arg)
{
    Graphic<ColorDepth> *g = device.contentPane->getGraphic();
    
    device.contentPane->fill(0xC000);
    auto frame = (RuntimeFrame *)arg.a3;
    device.clear();
    device.println("Hard Fault Error at address :  \n");
    device.println( "\nPC~~~~~~");
    device.printHex( (uint32_t)frame->PC );
    device.println("Cause : Bad Memory Address \n");
    device.println( "\n(PC - 1)~~~~~~");
    device.printHex(*(uint32_t *)(frame->PC));
    device.println( "\n(PC)~~~~~~");
    device.printHex( *(uint32_t *)(frame->PC - 4) );
    
    device.println( "\nXPSR~~~~~~");
    device.printHex( (uint32_t)frame->XPSR);
    
    device.println( "\nThread~~~~~~");
    device.print( runtime.running->getName());
    
    device.println( "\nThread Stack address~~~~~~");
    device.printHex( (uint32_t)arg.a3);
    
    device.println( "\nStack Dump : \n\n");
    Word * w = (Word *)runtime.running->getStackRoof();
    for (int i = runtime.running->getStackSize() / 4; i >= 0; i--) {
        device.printHex( w[i] );
        device.print("  ");
    }
    g->setText(device.getStream(), ColorWhite);
    device.fill(device.contentPane->getFrame());
    DwArg retArg = {0, 0};
    while (runtime.sensorAdapter.TouchSensorIT() <= 0);
    
    return retArg;
}


__value_in_regs DwArg vmstart()
{
    return runtime.run();
}
int vminit (uint32_t heapStart, uint32_t heapSize)
{
    return runtime.init(heapStart, heapSize);
}

int eventServerFinish ()
{
    return 0;
}

extern void CMSIS_Tick();

extern "C"
    void *RuntimeInit (void *p)
    {
       /*TODO  Auxiliary Control Register of SCB - set DISMCYCINT, DISFOLD ? 
        Write SCR
        Writte CCR
        Write other control registers
        */
       return (void *)0;
    }


extern "C"
__value_in_regs DwArg VMTick (void *arg)
{
    CMSIS_Tick();
    return vmtick (arg);
}

extern "C"
void *VMInit (SVC_arg arg)
{
    return (void *)vminit(arg.a1, arg.a2);
}

extern "C"
__value_in_regs DwArg VMStart ()
{
    return vmstart();
}

extern "C"
__value_in_regs DwArg VMSvc (SVC_arg arg)
{
    return vmsvc(arg);  
}

extern "C"
__value_in_regs DwArg VMHardFault (SVC_arg arg)
{
    return vmfault(arg);  
}

extern "C"
void *StackSwitchPSV (void *frame, int32_t link)
{
    return frame;
}


