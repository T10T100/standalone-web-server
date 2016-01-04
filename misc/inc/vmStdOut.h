#ifndef VM_STD_OUT
#define VM_STD_OUT
#include "stdint.h"

typedef struct {
        char *stream;
        uint32_t capacity;     
} DefaultCharStreamStruct;

enum DefaultStreamModes {
    StreamClosed = 0,
    StreamOpened = 1,
    
    
    StreamError  =-1,
  
};

template<typename Stream>
class VmOut {
    private :
        Stream *outStream;
        uint32_t elementsCount;
        uint32_t capacity;
        int8_t status;
    
        char *reverseChars (char *charSequence)
            {
                  char *j;
                  int32_t c;
                 
                  j = charSequence + strlen(charSequence) - 1;
                  while(charSequence < j) {
                    c = *charSequence;
                    *charSequence++ = *j;
                    *j-- = c;
                  }	
                  return j;	
            }    
            char *stringifyInt (char *buffer, int32_t value, int32_t base)
            {
                  int32_t i, sign;
                  if ((sign = value) < 0)              /* record sign */
                      value = -value;                    /* make n positive */
                  i = 0;
                  do {                               /* generate digits in reverse order */
                      buffer[i++] = value % base + '0';   /* get next digit */
                  } while ((value /= base) > 0);       /* delete it */
                  if (sign < 0)
                      buffer[i++] = '-';
                  //buffer[i] = '\0';
                  return reverseChars(buffer);
            }
            char *stringifyInt (char *buffer, uint32_t value, int32_t base)
            {
                  int32_t i;                   /* make n positive */
                  i = 0;
                  do {                               /* generate digits in reverse order */
                      buffer[i++] = value % base + '0';   /* get next digit */
                  } while ((value /= base) > 0);       /* delete it */
                  //buffer[i] = '\0';
                  return reverseChars(buffer);
            }
            int32_t printChar(char c)
            {
                if (status == StreamClosed) { return -1; }
                if (this->elementsCount + 1 > this->capacity)  {
                    return -1;
                } else {}
                this->outStream[this->elementsCount++] = c;
                return 0;                    
            }
    public :
        VmOut (Stream *out, uint32_t capacity)
        {
           outStream = out;
           elementsCount = 0;
           this->capacity = capacity;
           status = StreamOpened;
           for (int i = capacity; i >= 0; i--) {
               out[i] = 0;
           }
        }

        void print (char *str)
        {
            int i = 0;
            while (str[i] != 0) {
                printChar(str[i]);
                i++;
            }
        }
        void println (char *str)
        {
            if (status == StreamClosed) { return; }
            printChar('\n');
            print(str);
        }
        void print (const char *str)
        {
            int i = 0;
            while (str[i] != 0) {
                printChar(str[i]);
                i++;
            }
        }
        void println (const char *str)
        {
            if (status == StreamClosed) { return; }
            printChar('\n');
            print(str);
        }
        void print (int32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            print ( stringifyInt(buf, value, 10) );
        }
        void println (int32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            println ( stringifyInt(buf, value, 10) );
        }
        void printHex (int32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            print ( "0x" );
            print ( stringifyInt(buf, value, 16) );
        }
        void printlnHex (int32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            println ( "0x" );
            print ( stringifyInt(buf, value, 16) );
        }
        
        void print (uint32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            print ( stringifyInt(buf, value, 10) );
        }
        void println (uint32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            println ( stringifyInt(buf, value, 10) );
        }
        void printHex (uint32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            print ( "0x" );
            print ( stringifyInt(buf, value, 16) );
        }
        void printlnHex (uint32_t value)
        {
            char  buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
            println ( "0x" );
            print ( stringifyInt(buf, value, 16) );
        }
        
        
        
        
        
        
        Stream *getStream ()
        {
            return outStream;
        }
        
        void clear ()
        {
            for (int i = capacity; i >= 0; i--) {
                outStream[i] = 0;
            }
            elementsCount = 0;
        }
        void close ()
        {
           outStream = nullptr;
           elementsCount = 0;
           status = StreamClosed;
        }
    
    
};


#endif


/*End of file*/

