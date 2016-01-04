#ifndef DRIVER_REQUEST_CLASS
#define DRIVER_REQUEST_CLASS

#include "vmapi.h"
#include "Thread.h"
#include "ArrayListStatic.h"
#include "syncClass.h"

typedef int (*Driver_t) (void *);

class DriverSensorHandler : public DefaultArrayListBaseNode<DriverSensorHandler> {
private :
   Driver_t handler;

public :
    DriverSensorHandler () {}
    DriverSensorHandler (Driver_t handler)
    {
        this->handler = handler;
    }
    void operator () (Driver_t handler)
    {
        this->handler = handler;
    }
    int run (void *arg)
    {
        return  (*handler) (arg);
    }
    
    
};

class DriverSensorServer {
    private :
        ArrayListBase<DriverSensorHandler> driverHandlers;
    
    public :
        DriverSensorServer () {}
        void fireSensorDriverHandlers ()
        {
            DriverSensorHandler *it = driverHandlers.getFirst();
            while (it != nullptr) {
                it->run((void *) this);
                it = it->nextLink;
            }
        }    
        void addDriverSensorHandler (DriverSensorHandler *handler)
        {
            driverHandlers.addLast(handler);
        }
    
    
};

int driverSensorServer (Runnable *server);

/*
#include "vmapi.h"
#include "Thread.h"
#include "Queue.h"
#include "syncClass.h"

typedef int (*Event_t) (void *);

extern int eventServerFinish ();

class EventFactory : public Synchronizer<EventFactory> {
    private :
        QueueBase<Event_t, (int)32> eventQueue;
        bool firedUp;
        bool isEmpty;
        bool force;
        
        
    public :
        EventFactory () {}
        void fireEvents ()
        {
            Synchronize<EventFactory> sync(this);
            if (sync.test() == false) {
                return;
            }
            firedUp = true;
            while (eventQueue.isEmpty() == false) {
                (*eventQueue.pop()) (this);
            }
            eventServerFinish();
            isEmpty = true;
        }    
        void addEvent (Event_t e)
        {
           Synchronize<EventFactory> sync(this);
            if (sync.test() == false) {
                return;
            } 
            if (eventQueue.isFull() == false) {
                eventQueue.push(e);
            }
        }
   
};

int eventServer (Runnable *server);
*/




#endif

/*End of file*/


