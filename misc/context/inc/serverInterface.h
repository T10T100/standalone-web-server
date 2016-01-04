#ifndef SERVER_INTERFACE
#define SERVER_INTERFACE

#include "memory_template.h"
#include "vmapi.h"
#include "Thread.h"
#include "ArrayListStatic.h"
#include "syncClass.h" 

#define DEFAULT_SERVER_STACK_SIZE 320 * 4

typedef int (*ServerListener_t) (void *server);

int stubServerCaller (void *anyServer);
    
        class ServerListener {
            private :
                ServerListener_t listener;

            public :
                ServerListener () {}
                    ServerListener (ServerListener_t l)
                    {
                        listener = l;
                    }
                    void operator () (ServerListener_t l)
                    {
                        listener = l;
                    }
                    int run (void *arg)
                    {
                        return (*listener)(arg);
                    }

                    
                    inline void setListener (ServerListener_t listener)
                    {
                        this->listener = listener;
                    }
                    
                    inline ServerListener_t getListener ()
                    {
                        return listener;
                    }
        };
        
        class DefaultServerListener  : public ServerListener,
                                       public DefaultArrayListBaseNode<DefaultServerListener> {
            private :
            public :
                DefaultServerListener () {}
                DefaultServerListener (ServerListener_t l) : ServerListener(l) {}
                void operator () (ServerListener_t l)
                {
                    ServerListener::setListener(l);
                }                    
        };
        
        template <typename L> 
        class ServerInterface : public DefaultArrayListBaseNode<ServerInterface<L> > {
        protected :
           Runnable *caller;
           ArrayListBase<L> *listeners;
           uint32_t channelsCount;
           int32_t ID;
           int32_t busy; 
           void *userObject; /*can be 0*/

        
        
        public :
            ServerInterface () {}
            ServerInterface (Runnable *caller, ArrayListBase<L> *listeners, uint32_t channelsCount, int32_t id, void *o = nullptr)
            {
                this->listeners = listeners;
                this->channelsCount = channelsCount;
                this->caller = caller;
                userObject = o;
                ID = id;
                busy = false;
            }
            void operator () (Runnable *caller, ArrayListBase<L> *listeners, uint32_t channelsCount, int32_t id, void *o = nullptr)
            {
                this->listeners = listeners;
                this->channelsCount = channelsCount;
                this->caller = caller;
                userObject = o;
                ID = id;
                busy = false;
            }
           void fireChannel (int32_t channel)
           {
               if (channel >= channelsCount || channel < 0) {
                   return;
               }
               L *it = listeners[channel].getLast();
               while (it != nullptr) {
                   it->run(userObject);
                   it = it->prevLink;
               }
           }
           void fireAll ()
           {
               for (int i = 0; i < channelsCount; i++) {
                   fireChannel(i);
               }
           }
            Runnable *getCaller ()
            {
                return caller;
            }
            int32_t getID ()
            {
                return ID;
            }
            void addListener (L *listener, uint32_t channel)
            {
                if (channelsCount <= channel || listener == nullptr) {
                   return;
                }
                listeners[channel].addFirst(listener);
            }
            
            Runnable *invoke ()
            {
                if (busy == true) {
                    return nullptr;
                }
                busy = true; 
                Runnable *r;
                r = getCaller();
                r->reset((void *) this); 
                return r;
            }
            void close ()
            {
                busy = false;
            }
            
            void *getUserObject ()
            {
                return userObject;
            }
           
            /*
            Remove listener
            Remove all listeners 
            */
        
    };
    
    template <typename L>
    class ServerFactory : protected virtual Allocator<void *> ,
                          protected virtual ThreadFactory {
        private :
            ArrayListBase<ServerInterface<L> > servers;
            ServerInterface<L> *serverRuntime;
        
        public :
            ServerFactory () {}
            void operator () ()
            {
                serverRuntime = nullptr;
            }
                
            void installServer (Runnable_t caller, int32_t id, int32_t listenersCount, char *name, void *o = nullptr)
            {
                ServerInterface<L> *s = New<ServerInterface<L> >();
                if (s == nullptr) {
                    /*throw*/
                    return;
                }
                ArrayListBase<L> *listeners = NewArray<ArrayListBase<L> >(listenersCount);
                if (listeners == nullptr) {
                    /*throw*/
                    Delete(s);
                    return;
                }
                Runnable *thread = newServer(caller, 0, id, name);
                if (thread == nullptr) {
                    Delete(s);
                    Delete(listeners);
                    return;
                }
                (*s) (thread, listeners, listenersCount, id, o);
                
                servers.addLast(s);
            }
            
            Runnable *invokeServer (int32_t id)
            {
                Runnable *r = nullptr;
                serverRuntime = servers.getFirst();
                while (serverRuntime != nullptr) {
                    if (serverRuntime->getID() == id) {
                        return serverRuntime->invoke();
                    }
                    serverRuntime = serverRuntime->nextLink;
                }
                return r;
            }
            
            void addServerListener (ServerListener_t listener, int32_t id, int32_t channel)
            {
                serverRuntime = servers.getFirst();
                while (serverRuntime != nullptr) {
                    if (serverRuntime->getID() == id) {
                        serverRuntime->addListener( newListener(listener), channel );
                        break;
                    }
                    serverRuntime = serverRuntime->nextLink;
                }
            }
            
            virtual L *newListener (ServerListener_t l_t)
            {
                L *l = New<L>();
                (*l)(l_t);
                return l;
            }
            
            bool closeServer (Runnable *r)
            {
                int32_t id = r->getId();
                serverRuntime = servers.getFirst();
                while (serverRuntime != nullptr) {
                    if (serverRuntime->getID() == id) {
                        serverRuntime->close();
                        return true;
                    }
                    serverRuntime = serverRuntime->nextLink;
                }
                return false;
            }
    };



#endif

/*End of file*/

