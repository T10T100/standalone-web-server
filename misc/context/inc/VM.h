#ifndef VM_CLASS
#define VM_CLASS
#include "device.h"
#include "Thread.h"
#include "syncClass.h"
#include "stack.h"
#include "contextSwitching.h"
#include "RuntimeEventListener.h"
#include "Event.h"
#include "driverRequest.h"
#include "TouchScreenAdapter.h"
#include "serverInterface.h"
#include "timerServer.h"
#include "vmapi.h"

#define MAX_RUNNABLE_PRIORITY 8

int SystemEventBurner (void *);
extern int eventServer (void *);
extern int timerServer (void *);
extern int sensorInputServer (void *);

extern uint32_t touchSensorDriverID;


class Runtime : public virtual Synchronizer<Runtime>,
                public EventFactory,
                public TimerInterface,
                public ServerFactory<DefaultServerListener> ,
                public Allocator<void *>,
				public virtual ThreadFactory {
    private :
        MemoryAllocator allocator;
        Runnable *running;
        ArrayListBase<Runnable> arrayOfRunnables[MAX_RUNNABLE_PRIORITY];
        ArrayListBase<Runnable> waitList;
        bool canSwitch;
    
    
        ArrayListBase<RuntimeEventListener> arrayOfListeners;
    
        uint32_t mills;
    
        void *alloc (uint32_t size)
        {
            return allocator.New(size);
        }
        
        void free (void *p)
        {
            allocator.Delete(p);
        }
        
        
        void fireSystemEvents ()
        {
            RuntimeEventListener *l = arrayOfListeners.getFirst();
            while (l != nullptr) {
                l->run();
                l = l->nextLink;
            }
        }
    
        template <typename Link>
        void runnablesTick (Link link)
        {
           Runnable *iterator = waitList.getFirst();
           Runnable *r;
               while (iterator != nullptr) {
                   r = iterator;
                   iterator = iterator->nextLink;
                   if (r->testSelf(link) != false) {
                       waitList.remove(r);
                       push(r);
                   } else {    
                   }
                   
               }
        }
        
        Runnable *getAvailable (int32_t priority)
        {
            Runnable *r = nullptr;
            for (int i = 0; i < MAX_RUNNABLE_PRIORITY; i++) {
                if (arrayOfRunnables[i].isEmpty() == false) {
                    return arrayOfRunnables[i].removeFirst();
                }
            }
            return r;
        }
    
        __value_in_regs DwArg dispatch (void * frame)
        {
            DwArg retArg = {(Word) frame, running->getAccessLvl()};
            if (mills++ % 4 != 0 || mills == 0) {
                return retArg;
            }
            if (canSwitch == false) {
                return retArg;
            }
            Synchronize<Runtime> sync(this);
            if (sync.test() == false) {
                return retArg;
            }
            //fireSystemEvents();
            
            running->setFrame((RuntimeFrame *)frame); 
            runnablesTick(this);
            
            push(running);
            running = pop();
            
            return running->getFrame((RuntimeFrame *)frame);
        }
        
        __value_in_regs DwArg dispatchSVC (SVC_arg arg)
        {
            Synchronize<Runtime> sync(this);
            if (sync.test() == false) {
                return running->getFrame();
            }
            RuntimeFrame *frame = (RuntimeFrame *)arg.a3;
            running->setFrame(frame);
            return resolveAPICall(frame, arg);
        }
        
        __value_in_regs DwArg run ()
        {
            running = arrayOfRunnables[0].removeFirst();
            return running->getFrame(0);
        }
        
        DwArg resolveAPICall (RuntimeFrame *frame, SVC_arg a)
        {
            uint32_t id     = running->getId();
            DwArg ret = {(Word)frame, (Word)id};
            
            switch (a.a0) {
                case vm::__create: addRunnable ((Runnable_t)a.a1, (char *)a.a2);

                case vm::__kill:
                        
                    break;
                case vm::__sleep:
                        if (id == IddleThreadID) { break; }
                        
                        running->setWait(a.a1);
                        waitList.addFirst(running);
                        running = getAvailable (0);
                    return running->getFrame(frame);

                case vm::__runtimeDispatchEvent:
                        RuntimeEventListener *l = (RuntimeEventListener *)allocator.New(sizeof(RuntimeEventListener));
                        (*l)((RuntimeEventListener_t) a.a1);    
                        arrayOfListeners.addFirst(l);
                    break;
                case vm::__pushEvent:
                            //push( invokeServer((int32_t)param0) );
                            //addEvent((Event_t) param0);
                    break;
                case vm::__addTimer:
                        TimerInterface::addListener( newTimerListener( (ServerListener_t)a.a1, a.a2, 0), 0 );
                    break;
                case vm::__addSensorListener:
                        addServerListener((ServerListener_t)a.a1, TouchSensorDriverID, (uint32_t)a.a2);
                    break;
                case vm::__close: 
                            /*Delete(running);*/ /**/
                           running = getAvailable(0); /**/
                    return running->getFrame(frame);
                case vm::__invokeServer:
                           push( invokeServer((int32_t) a.a1) );
                case vm::__startCritical:
                        canSwitch = false;
                    break;
                case vm::__endCritical:
                        canSwitch = true;
                    break;
                default :
                    break;
            }
            return ret;
        }
           
        
        
        Runnable *pop ()
        {
            return getAvailable(0);
        }
        
    public :
        void push (Runnable *r)
        {
            Synchronize<Runtime> sync(this);
            if (sync.test() == false) {
                return;
            }
            if (r != nullptr) {
                arrayOfRunnables[r->getPriority()].addLast(r);
            }
        }    
        SensorAdapter sensorAdapter;
    
        /*Friends*/
        friend __value_in_regs DwArg vmtick (void *arg);
        friend __value_in_regs DwArg vmsvc (SVC_arg arg);
        friend __value_in_regs DwArg vmstart();
        friend int vminit (uint32_t heapStart, uint32_t heapSize);
        friend int eventServer (Runnable *server);
        friend int eventServerFinish ();
        friend int driverSensorServer (Runnable *server);
        friend __value_in_regs DwArg vmfault (SVC_arg arg);
        /*Friends*/
    
        
    
        Runtime ()
        {
            running = nullptr;
            canSwitch = true;
        }
        int init (uint32_t heapStart, uint32_t heapSize)
        {
           this->allocator(heapStart, heapSize);
           
           addIddle(SystemEventBurner, 0);
           installServer(eventServer, EventServerID, 1, "Event server", 0);
           TimerInterface::operator () (timerServer, 1, TimerServerID, 0);
            
           sensorAdapter(); 
           installServer(touchSensorServer, TouchSensorDriverID, 6, "Touch Screen Server", &sensorAdapter);
           return 0;
        }
        
        void addRunnable (Runnable_t runnable, char *name, uint32_t priority = 0)
        {
            arrayOfRunnables[0].addLast( newThread(runnable, 3, DefaultThreadID, name) );
        }
        
		void addIddle (Runnable_t runnable, uint32_t priority = 0)
        {
            arrayOfRunnables[0].addFirst( newServer(runnable, 6, IddleThreadID, "Iddle") );
        }
     
};



#endif


/*End of file*/

