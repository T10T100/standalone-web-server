#ifndef WEB_SERVER_INTERFACE
#define WEB_SERVER_INTERFACE

#include "init.h"
#include "web_server.h"

class WebServerInterface {
    private :
        void *device;
    public :
        WebServerInterface (void *device) 
        {
            this->device = device;
        }
    
        
        void run ()
        {
            Web_Server();
        }
        
        void applySimpleCommand (int32_t command);
    
    
};



#endif

