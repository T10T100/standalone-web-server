#ifndef EVENT_CLASS
#define EVENT_CLASS

#include "vmapi.h"
#include "Thread.h"
#include "Queue.h"
#include "syncClass.h"

#define EventServerID 0x0B00B002

typedef int (*Event_t) (void *);

int eventServer (void *server);

extern int eventServerFinish ();

class EventFactory {
    private :
        QueueBase<Event_t, (int)32> eventQueue;
        bool firedUp;
        bool isEmpty;
        bool force;
        
        
    public :
        EventFactory () {}
        void fireEvents ()
        {
            firedUp = true;
            while (eventQueue.isEmpty() == false) {
                (*eventQueue.pop()) (this);
            }
            eventServerFinish();
            isEmpty = true;
        }    
        void addEvent (Event_t e)
        {
            if (eventQueue.isFull() == false) {
                eventQueue.push(e);
            }
        }
   
};




#endif



/*End of file*/

