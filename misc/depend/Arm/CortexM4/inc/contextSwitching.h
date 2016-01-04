#ifndef CONTEXT_SWITCHING
#define CONTEXT_SWITCHING

#include <stdint.h>

typedef uint32_t Word;
typedef uint16_t HWord;
typedef uint8_t Byte;

#pragma import VMTick   
#pragma import StackSwitchPSV   
#pragma import VMSvc
#pragma import VMInit
#pragma import VMStart

#define CPU_PRIV_ACCESS 0
#define CPU_UNPRIV_ACCESS 1

#define CPU_USE_MSP 0
#define CPU_USE_PSP 2

#define ThreadAccessLevel0 (CPU_USE_PSP | CPU_UNPRIV_ACCESS)
#define ThreadAccessLevel1 (CPU_USE_PSP | CPU_PRIV_ACCESS)
#define ThreadAccessLevel2 (CPU_USE_MSP | CPU_UNPRIV_ACCESS)
#define ThreadAccessLevel3 (CPU_USE_MSP | CPU_PRIV_ACCESS)

class RuntimeFrame {
    private :
    
    public :
    RuntimeFrame ()
    {
        
    }
    void operator () (RuntimeFrame *runtime)
    {
        (*this) = runtime;
    }
    void operator = (void *r)
    {
        (*this) = (RuntimeFrame *)r;
    }
    void init (RuntimeFrame *runtime)
    {
        this->R0 = runtime->R0;
        this->R1 = runtime->R1;
        this->R2 = runtime->R2;
        this->R3 = runtime->R3;
        this->R4 = runtime->R4;
        this->R5 = runtime->R5;
        this->R6 = runtime->R6;
        this->R7 = runtime->R7;
        this->R8 = runtime->R8;
        this->R9 = runtime->R9;
        this->R10 = runtime->R10;
        this->R11 = runtime->R11;
        this->R12 = runtime->R12;
        this->LR = runtime->LR;
        this->PC = runtime->PC;
        this->XPSR = runtime->XPSR;
    }
    Word R11;
    Word R10;
    Word R9;
    Word R8;
    Word R7;
    Word R6;
    Word R5;
    Word R4;
    Word R0;
    Word R1;
    Word R2;
    Word R3;
    Word R12;
    Word LR;
    Word PC;
    Word XPSR;
};    
  
    
typedef struct {
   Word a0;
   Word a1;
   Word a2;
   Word a3; /*!*/
} SVC_arg;



#endif


/*End of file*/

