
#include "vmapi.h"

extern "C" __value_in_regs Word upcall (SVC_arg a);

extern int vminit (uint32_t heapStart, uint32_t heapSize);

extern "C"  __value_in_regs DwArg vmstart();

__value_in_regs __svc(2) int __upcall (int a, int b, int c);

__value_in_regs int vm::init (uint32_t heapStart, uint32_t heapSize)
{
    return vminit(heapStart, heapSize);
}
__value_in_regs int vm::start ()
{
    return vmstart().a0;
}
__value_in_regs int vm::create (Runnable_t runnable, char *name = nullptr)
{
    return __upcall (__create, (Word)runnable, (Word)name);
}
__value_in_regs int vm::kill (int cause)
{
    return __upcall (__kill, (Word)cause, 0);
}
__value_in_regs int vm::sleep (int sleep)
{
    return __upcall (__sleep, (Word)sleep, 0);
}
__value_in_regs int vm::runtimeDispatchEvent ( int (*listener) (void *))
{
    return __upcall (__runtimeDispatchEvent, (Word)listener, 0);
}
__value_in_regs int vm::pushEvent (int (*event) (void *))
{
    return __upcall (__pushEvent, (Word)event, 0);
}
__value_in_regs int vm::close ()
{
    return __upcall (__close, 1, 0);
}


__value_in_regs int vm::addTimer ( int (*t) (void *), uint32_t period)
{
    return __upcall (__addTimer, (Word)t, period);
}

__value_in_regs int vm::addSensorListener ( int (*t) (void *), int32_t id)
{
    return __upcall (__addSensorListener, (Word)t, id);
}

__value_in_regs int vm::invokeServer (int32_t id)
{
    return __upcall (__invokeServer, (Word)id, 0);
}



__value_in_regs int vm::startCritical ()
{
    return __upcall (__startCritical, 0, 0);
}

__value_in_regs int vm::endCritical ()
{
    return __upcall (__endCritical, 0, 0);
}


/*End of file*/

