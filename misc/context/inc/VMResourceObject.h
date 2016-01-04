#ifndef VM_RESOURCE_OBJECT
#define VM_RESOURCE_OBJECT
#include <stdint.h>
#include "ArrayListStatic.h"

template <typename Owner>
    class VMResource : public DefaultArrayListBaseNode<VMResource<Owner> > {
    private :
        Owner *owner;
        bool hold;
    public :
        VMResource ()
        {
            this->owner = nullptr;
            hold = false;
        }
        bool test ()
        {
            returun hold;
        }
        bool forceHold (Owner *newOwner)
        {
            if (newOwner != null) {
                    owner = newOwner;
                    hold = true;
                    return true;
            }
        }
        bool testHold (Owner *newOwner)
        {
            if (hold == false) {
                if (newOwner != nullptr) {
                    owner = newOwner;
                    hold = true;
                    return true;
                }
            }
            return false;
        }
        void forceRelease ()
        {
            hold = false;
            owner = nullptr;
        }
        Owner *getOwner ()
        {
            return owner;
        }
    
    
};
        

    class VMOwner  {
        private :
            VMResource<VMOwner> *resource;
            bool hold;
        
        public :
            VMOwner ()
            {
                
            }
            bool collectHold (VMResource<VMOwner> *r)
            {
                return r->testHold(this);
            }
            
        
        
        
    };






#endif