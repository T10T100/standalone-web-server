#ifndef RUNTIME_EVENT_LISTENER
#define RUNTIME_EVENT_LISTENER
#include "ArrayListStatic.h"

typedef void (* RuntimeEventListener_t) (void);

class RuntimeEventListener : public DefaultArrayListBaseNode<RuntimeEventListener> {
    private :
        RuntimeEventListener_t listener;
    public :
        RuntimeEventListener ()
        {
            this->listener = nullptr;
        }
        void operator () (RuntimeEventListener_t listener)
        {
            this->listener = listener;
        }
        void run ()
        {
            (*listener)();
        }
    
};

#endif

/*End of file*/

