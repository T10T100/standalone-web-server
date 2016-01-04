#include "web_interface.h"
#include "device.h"

WebServerInterface *__wFace = nullptr;

void WebServerInterface::applySimpleCommand (int32_t command)
{
    Device *device = (Device *)this->device;
    
    device->coilSet(command <= 0 ? false : true);
}