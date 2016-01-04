#ifndef STRING_TEMPLATE
#define STRING_TEMPLATE
#include "memory_template.h"
#include "arrayList.h"
#include <string.h>
#include <math.h>

#define DEFAULT_STRING_ALLOC 48
#define MAX_STRING_LENGTH 48
#define MAX_FLOAT_LITERAL_LENGTH 16


class String : public Allocator<char> {
    private :
        char *string;
        int length;
        int bufferRemain; 
        
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
    
        String *stringifyInt (int32_t value, int32_t base)
        {
              int32_t i, sign;
              char *c  = this->NewArray(MAX_FLOAT_LITERAL_LENGTH);
              if ((sign = value) < 0)              /* record sign */
                  value = -value;                    /* make n positive */
              i = 0;
              do {                               /* generate digits in reverse order */
                  c[i++] = value % base + '0';   /* get next digit */
              } while ((value /= base) > 0);       /* delete it */
              if (sign < 0)
                  c[i++] = '-';
              c[i] = '\0';
              return (String *) new String(reverseChars(c));
        }
        
    
    public :
      template <typename C>
        String (C *string)
        {
            this->string = this->NewArray(DEFAULT_STRING_ALLOC);
            int i = 0;
            for (; string[i] != 0 && i < MAX_STRING_LENGTH; i++) {
                this->string[i] = string[i];
            }
            this->length = i;
            bufferRemain = DEFAULT_STRING_ALLOC - i;
        }
        String (String &string)
        {
            this->string = this->New(string.length + 8);
            int i = 0;
            for (; this->string[i] != 0 && i < MAX_STRING_LENGTH; i++) {
                this->string[i] = string.string[i];
            }
            this->length = string.length;
            this->bufferRemain = 8;
        }
        String (char c)
        {
            this->string = this->NewArray(9);
            this->string[0] = c;
            this->length = 1;
            bufferRemain = 8;
        }
        ~String ()
        {
            delete(this->string);
            delete(this); 
        }
        char *operator = (String &string)
        {
            return this->string;
        }
        String *operator + (String &string)
        {
            int length = this->length + string.length;
            char *c = this->New(length + 8);
            int i = 0;
            for (; i < this->length; i++) {
                c[i] = this->string[i];
            }
            int n = 0;
            for (; i < length; i++, n++) {
                c[i] = string.string[n];
            }
            this->length = length;
            this->bufferRemain = 8;
            delete(this->string);
            //delete(string.string);
            this->string = c;
            return this;
        }
        String *operator + (String *string)
        {
            return (*this) + *string;
        }
        String *operator + (char *string)
        {
            (*this) + ((String *)new String(string));
            return this;
        }
        String *operator + (char c)
        {
            if (this->bufferRemain < 1) {
                (*this) + ((String *)new String(c));
                return this;
            } else {
                this->string[++this->length] = c;
                this->bufferRemain--;
                return this;
            }
        }
        String *operator + (int32_t n)
        {
            (*this) + this->stringifyInt(n, 10);
            return this;
        }
        String *operator + (float f)
        {
            union {
                float    fl;
                uint32_t dw;
            } uf;
            int s = ( uf.dw >> 31 ) ? -1 : 1;   /* sign */
            int e = ( uf.dw >> 23 ) & 0xFF;     /* range */
            int m =                            /* mantisa */
              e ?
                ( uf.dw & 0x7FFFFF ) | 0x800000 :
                ( uf.dw & 0x7FFFFF ) << 1;
            e -= 127;
            
            (*this) + this->stringifyInt(s * m * (pow(2.0f, (e-23)) ), 10);
            return this;
        }
        
        
        
        template <typename Object>
            String *operator + (Object &o)
            {
                (*this) + o.toString();
                return this;
            }
            
        template <typename Object>
            String *operator + (Object *o)
            {
                (*this) + o->toString();
                return this;
            }
        

};



#endif

/*End od file*/

