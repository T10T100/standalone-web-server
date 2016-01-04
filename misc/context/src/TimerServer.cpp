#include "TimerServer.h"
#include "vmapi.h"
#include "main.h"

int timerServer (void *r)
{
    static TimerInterface *i = (TimerInterface *)r;
    for (;;) {
        i->fireAll();
        i->close();
        vm::close();
    }
}
/*End of file*/

