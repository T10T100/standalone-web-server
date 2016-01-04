#ifndef STEP_MOTOR_DEVICE_CLASS
#define STEP_MOTOR_DEVICE_CLASS
#include "stdint.h"
#include "syncClass.h"


extern void __delay_ns(int ns);

class StepMotorAB : public Synchronizer<StepMotorAB> {
    
    private :
        int errCode;

        virtual int HLInit ();
        virtual int HLDeInit ();
        
        virtual int setCoilAPhase (int f); /*if f < 0 : electron current streams from A1* to A2* , if f == 0 : do nothing, f > 0 : A2 -> A1*/
        virtual int setCoilBPhase (int f);
    
        void delay_us (int us)
        {
            __delay_ns(us * 1000);
        }
        
        void step (int a, int b)
        {
            setCoilAPhase(a);
            delay_us(20);
            setCoilBPhase(b);
            delay_us(20);
        }
  
        int makeStep (int h)
        {
            /*if step < 0 -> make step Left, and so forth*/
            if (h < 0) {
                step(h, -h);
            } else {
                step(-h, h);
            }
            return 0;
        }
    public :
        StepMotorAB () : Synchronizer<StepMotorAB>()
        {
            
        }
        
        int mount (char *args)
        {
            errCode = HLInit();
            
            
            /*if errCode < 0 -> mount fault*/
            return errCode;
        }
        
        int unMount (char *args)
        {
            Synchronize<StepMotorAB> sync(this);
            if (sync.test() == false) {
                /*return*/
                return -1;
            }
            
            errCode = HLDeInit();
            
            
            /*if errCode < 0 -> unMount fault*/
            return errCode;
        }
        
        void tick ()
        {
            Synchronize<StepMotorAB> sync(this);
            sync.force();
            
            makeStep(1);
            makeStep(-1);
        }
        
        void goIddle ()
        {
            setCoilAPhase(1);
            setCoilBPhase(-1);
        }
        
        
    
};





#endif



/*End of file*/

