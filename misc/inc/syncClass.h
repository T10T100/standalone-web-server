#ifndef SYNCHRONIZER
#define SYNCHRONIZER

template <typename Object>
class Synchronize {
    private :
        Object *o;
        bool syncl;
    public :
        
        Synchronize (Object &o)
        {
           this->o = &o;
            if (o.synchro == true) {
                syncl = false;
            }
            o.synchro = true;
            syncl = true; 
        }
        Synchronize (Object *o)
        {
           this->o = o;
            if (o->synchro == true) {
                syncl = false;
            }
            o->synchro = true;
            syncl = true; 
        }
        bool test ()
        {
            return syncl;
        }
        void force ()
        {
            o->synchro = true;
            syncl = true; 
        }
        ~Synchronize ()
        {
            if (syncl == true) {
                this->o->synchro = false;
            }
        }
    
    
};

template <typename Object>
/*public*/ class Synchronizer {
    public :
    
    friend class Synchronize<Object>;
    
        Synchronizer ()
        {
            
        }
    //protected :
        virtual bool syncFetch (Object &o)
        {
            if (synchro == true) {
                return false;
            } else {
                synchro = true;
                return true;
            }
        }
        virtual bool syncFetch (Object *o)
        {
            if (synchro == true) {
                return false;
            } else {
                synchro = true;
                return true;
            }
        }
        virtual void syncRelease (Object &o)
        {
            synchro = false;
        }
        virtual void syncRelease (Object *o)
        {
            synchro = false;
        }
        bool synchro;
    
};

class CriticalObject {
    private :
        int isr;
    public :
       CriticalObject ()
       {
          __asm ("CPSID    I   \n\t");
       }  
       ~CriticalObject ()
       {
          __asm ("CPSIE    I   \n\t");
       }         
};


#endif

/*End of file*/



