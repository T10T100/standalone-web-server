#ifndef THREAD_RUNNABLE
#define THREAD_RUNNABLE
#include "stdint.h"
#include "memory_template.h"
#include "contextSwitching.h"
#include "arrayListStatic.h"

typedef int (* Runnable_t) (void *);

#define StackPadSize 4
#define DefaultthreadStackSize 256 * 4
#define DefaultThreadNameLength 12

enum RunnableStatus {
  Stopped = 0,
  Running = 1,
  Pending = 2,
};

enum RunnablesMagic {
    IddleThreadID   = 0x0A00AAAA,
    DefaultThreadID = 0x0A00A000,
    DefaultServerID = 0x0B00B000,
};    


typedef  struct {
   Word a0;
   Word a1; 
} DwArg;

typedef /*bit-band*/ struct {
    unsigned sleepTime  :   16;
    signed   priority   :   5;
    unsigned waitNotify :   1;
    unsigned waitThread:    1;
    unsigned waitEvent:     1;
    unsigned irreevant:     1;
    unsigned waitPipe:      1;
    unsigned waitDriver:    1;
    unsigned waitResource:  1;
    unsigned stackOverhead: 1;
    unsigned unrecognized:  1;
    unsigned tokeSignal:    1;
    
} TCB;

template <typename R>
class RunnbleInterface {
    protected :
        Runnable_t runnable; 
        int8_t status;
        int8_t accessLevel;
        int32_t id;
        Word stackRoof;
        Word stackSize;
        void *inputValue;
        void *returnValue;
        RuntimeFrame *frameSuspended;
    
        void setInputValue (void *a)
        {
            inputValue = a;
        }
        void setReturnValue (void *a)
        {
            returnValue = a;
        }
    
    public :
        RunnbleInterface ()
        {
            
        }
        void operator () (Runnable_t r, Word stackRoof, Word stackSize, int32_t accessLevel, int32_t id)
        {
            status            = Stopped;
            runnable          = r;
            this->stackRoof   = stackRoof;
            this->stackSize   = stackSize;
            this->id          = id;
            this->accessLevel = accessLevel;
        }
        
        DwArg getFrame (RuntimeFrame *link = nullptr)
        {
            if (link == nullptr) {
                status = Running;
                RuntimeFrame *f = (RuntimeFrame *)( stackRoof - (sizeof(RuntimeFrame)) );
                f->R11 = (Word)runnable;
                f->R0 = (Word)inputValue;
                DwArg arg = {(Word)f, (Word)accessLevel};
                return arg;
            }
            if (status == Stopped) {
                RuntimeFrame *f = (RuntimeFrame *)( stackRoof - (sizeof(RuntimeFrame)) );
                f->init(link);
                f->PC = (Word)runnable;
                f->R0 = (Word)inputValue;
                status = Running;
                DwArg arg = {(Word)f, (Word)accessLevel};
                return arg;
            } 
            if (status  == Running) {
                DwArg arg = {(Word)frameSuspended, (Word)accessLevel};
                return arg;
            } 
            DwArg arg = {0, 0};
            return arg;
        }
        
        

        void reset (void *a)
        {
            inputValue = a;
            status = Stopped;
        }
};



class Runnable : public DefaultArrayListBaseNode<Runnable>,
                 public RunnbleInterface<Runnable>  {
    private :
        
        TCB tcb;
        ArrayListBase<Runnable> links;
        char name[DefaultThreadNameLength];
        void initTcb (uint32_t priority)
        {
            tcb.irreevant = 0;
            tcb.sleepTime = 0;
            tcb.stackOverhead = 0;
            tcb.tokeSignal = 0;
            tcb.unrecognized = 0;
            tcb.waitDriver = 0;
            tcb.waitEvent = 0;
            tcb.waitNotify = 0;
            tcb.waitPipe = 0;
            tcb.waitResource = 0;
            tcb.waitThread = 0;
            tcb.priority = priority;
        }
        void init (Runnable_t r, uint32_t stackSize, int32_t priority, int32_t access, int32_t id)
        {
            this->stackSize = stackSize;
            stackSize -= StackPadSize;
            Word floor = (Word)this + stackSize;
            
            RunnbleInterface<Runnable>::operator () (r, floor, stackSize, id, access);
            
            initTcb(priority);
        }
        template <typename Char>
        void setName (Char *name)
        {
            int i = 0;
            while (name[i] != 0 && i < DefaultThreadNameLength) {
                this->name[i] = name[i];
                i++;
            }
        }
        Runnable () {}
    public :
        
    /**/
    friend class Runtime;
    friend class ThreadFactory;
    
        

        bool equals (Runnable &)
        {
            return true;
        }
        
        
        void setFrame (RuntimeFrame *frame)
        {
            this->frameSuspended = frame;
        }
    
        
        
        
        
        
        int32_t getPriority ()
        {
            if (status == Stopped) {
                return 0;
            }
            return this->tcb.priority + 1;
        }
        uint32_t getAccessLvl ()
        {
            return accessLevel;
        }
  
        int32_t getId ()
        {
            return this->id;
        }
        
        char *getName ()
        {
            return name;
        }
    
        
        
        template <typename Linked>
        bool testSelf (Linked *l)
        {
            if (tcb.sleepTime > 0) {
                tcb.sleepTime--;
                return false;
            }
            return true;
        }
    
        void setWait (int mills)
        {
            this->tcb.sleepTime = mills;
        }
            
    
        template <typename A>
        bool assert (A a)
        {
            return false;
        }
        
        void stop ()
        {
            this->status = Stopped;
        }
        
        uint32_t getStackSize ()
        {
            return stackSize;
        }
        uint32_t getStackRoof ()
        {
            return stackRoof;
        }
        
    
};



class ThreadFactory : protected virtual Allocator<void *> {
        private :
            
        
        public :
            ThreadFactory () {}
                
            template <typename Char>    
            Runnable *newThread (Runnable_t r, uint32_t priority, uint32_t id = DefaultThreadID, Char *name = nullptr)
            {
                Runnable *thread = New<Runnable>(DefaultthreadStackSize);
                if (thread == nullptr) {
                    return nullptr;
                }
                thread->init(r, DefaultthreadStackSize, priority, ThreadAccessLevel0, id);
                thread->setName(name);
                return thread;
            }
            
            template <typename Char> 
            Runnable *newServer (Runnable_t r, uint32_t priority, uint32_t id, Char *name)
            {
                Runnable *thread = New<Runnable>(DefaultthreadStackSize);
                if (thread == nullptr) {
                    return nullptr;
                }
                thread->init(r, DefaultthreadStackSize, priority, ThreadAccessLevel1, id);
                thread->setName(name);
                return thread;
            }
    
};


#endif


/*End of file*/

