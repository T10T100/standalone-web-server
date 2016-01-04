#ifndef DRIVER_INTERFACE
#define DRIVER_INTERFACE
#include "Thread.h"
#include "Queue.h"
#include "memory_template.h"


int StubDriverCaller (void *r);

typedef int (*DriverListener_t) (void *);

class DriverInterface : public DefaultArrayListBaseNode<DriverInterface> {
    
    private :
        Runnable *caller;
        int32_t IRQ_N;
        int32_t ID;
    
        QueueBase<Runnable *, 16> waiters;
        QueueBase<void *, 48> inStream;
        QueueBase<void *, 48> outStream;
    
        void *Core;
        
        DriverInterface () {}
            
    public :
        template<typename O>
        void inWrite (O *o)
        {
           inStream.push(o); 
        }
        template<typename O>
        O *inRead ()
        {
           return (O *)inStream.pop();
        }
        template<typename O>
        void outWrite (O *o)
        {
           outStream.push(o); 
        }
        template<typename O>
        O *outRead ()
        {
           return (O *)inStream.pop();
        }
        
        
        bool isInputEmpty ()
        {
            return inStream.isEmpty();
        }
        bool isOutputEmpty ()
        {
            return outStream.isEmpty();
        }
    
};
    


#endif

/*End of file*/

