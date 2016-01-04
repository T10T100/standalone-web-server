#ifndef TIMER_SERVER_CLASS
#define TIMER_SERVER_CLASS

#include "ServerInterface.h"

#define TimerServerID 0x0B00B001

int timerServer (void *);

class TimerListener : public ServerListener,
                      public DefaultArrayListBaseNode<TimerListener> {
	private :
		uint32_t counter;
		uint32_t top;
		uint32_t bottom;
		TimerListener () {}
		TimerListener (ServerListener_t listener, uint32_t top, uint32_t bottom = 0) : ServerListener(listener)
		{
			counter = 0;
			this->top = top;
			this->bottom = bottom;
		}
		void operator () (ServerListener_t listener, uint32_t top, uint32_t bottom = 0)
		{
			counter = 0;
			this->top = top;
			this->bottom = bottom;
			setListener(listener);
		}

	public :
        
        int run (void *arg)
		{
			if (counter < top) {
				counter++;
				return 1;
			}
			counter = bottom;	
			return (* getListener() )(arg);
		}
		
        friend class TimerInterface;
};


class TimerInterface : public ServerInterface<TimerListener> , 
                       public virtual Allocator<void *> ,
                       private virtual  ThreadFactory  {
    private :
        TimerListener *newListener ()
        {
            return New<TimerListener>();
        }

    public :
        TimerInterface () {}
            TimerInterface (Runnable_t caller, uint32_t channelsCount, int32_t id, void *o = nullptr)
            {
                ArrayListBase<TimerListener> *l = NewArray<ArrayListBase<TimerListener> >(channelsCount);
                if (l == nullptr) {
                    return;
                }
                Runnable *r = newServer(caller, 0, id, (char *)"Timer Server");
                if (r == nullptr) {
                    Delete(l);
                    return;
                }
                this->ServerInterface::operator () (r, l, channelsCount, id, o);
            }
            void operator () (Runnable_t caller, uint32_t channelsCount, int32_t id, void *o = nullptr)
            {
                ArrayListBase<TimerListener> *l = NewArray<ArrayListBase<TimerListener> >(channelsCount);
                if (l == nullptr) {
                    return;
                }
                Runnable *r = newServer(caller, 0, id, (char *)"Timer Server");
                if (r == nullptr) {
                    Delete(l);
                    return;
                }
                this->ServerInterface::operator () (r, l, channelsCount, id, o);
            }
            
            ArrayListBase<TimerListener> *getChannel (uint32_t id)
            {
                if (id >= channelsCount) {
                    return nullptr;
                }
                return &listeners[id];
            }
            TimerListener *newTimerListener (ServerListener_t listener, uint32_t top, uint32_t bottom)
            {
                TimerListener *l = New<TimerListener>();
                (*l)(listener, top, bottom);
                return l;
            }
	
    
};

#endif


/*End of file*/

