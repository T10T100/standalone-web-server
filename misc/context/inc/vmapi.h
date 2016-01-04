#ifndef VMAPI_INTERFACE
#define VMAPI_INTERFACE

#include "contextSwitching.h"
#include "Thread.h"


namespace vm {
  



/*Next enumeration contains numeric representation of 
  called supervisor  functions (UpCall) throught SVC exception*/
    
    
  enum VectorNum {
      __init         =  0, 
      __start        =  1,
      __create       =  2,
      __kill         =  3,
      __sleep        =  4,
      __pushEvent    =  5,
      __runtimeDispatchEvent = 6,
      __close        =  7,
      __addTimer     =  8,
      __addSensorListener = 9,
      __invokeServer = 10,
      __startCritical = 11,
      __endCritical = 12,
      
      
      
      __EOFVectorApi
      
  };


    
/*
  */    

__value_in_regs int init (uint32_t heapStart, uint32_t heapSize);
__value_in_regs int start (); 
__value_in_regs int create (Runnable_t entryPoint, char *name);
__value_in_regs int kill (int cause);
__value_in_regs int sleep (int mills);
__value_in_regs int pushEvent (int (*eventListener) (void *));
__value_in_regs int runtimeDispatchEvent (int (*eventistener) (void *));
__value_in_regs int close (/*server*/); 
__value_in_regs int addTimer (int (*timerListener) (void *), uint32_t);
__value_in_regs int addSensorListener (int (*reqListener) (void *), int32_t);
__value_in_regs int invokeServer (int32_t id);
  
__value_in_regs int startCritical ();
__value_in_regs int endCritical ();
  
class CleanUp {
    private :
        
    public :
        CleanUp ()
        {
            
        }
        ~CleanUp ()
        {
            kill(-1);
        }
};
    
};


  

#endif


/*End of file*/


