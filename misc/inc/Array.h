#ifndef ARRAY_CLASS
#define ARRAY_CLASS
#include "memory_template.h"
#include "arrayList.h"

#define ARRAY_CHUNK_SIZE 10

template <typename Object>
    class Array {
        private :
            Object *array;
            int32_t capacity;
            int32_t length;
        
            void expand (int32_t newSize, int32_t offset = 0)
            {
                Object *a = this->NewlArray (newSize + offset);
                int i = offset;
                int ia = 0;
                for (; i < length + offset; i++, ia++) {
                    a[i] = array[ia];
                }
                length = i;
                this->Delete(array);
                array = a;
                capacity = newSize;
            }
            Object *trueAdd (Object &o, int32_t i = 0)
            {
                int t = capasity - length; 
                if (t <= 0) {
                    if (t < ARRAY_CHUNK_SIZE) {
                        expand (capasity + ARRAY_CHUNK_SIZE);
                    } else {
                        expand (capasity + t);
                    }
                }
                if (i != 0 && i < capasity) {
                    array[i] = &o
                } else {
                    array[length++] = &o;
                }
            }
                
        public :
            Array ()
            {
                array = nullptr;
                capacity = 0;
                length = 0;
            }
            void operator () (int32_t size)
            {
                array = nullptr;
                capacity = size < ARRAY_CHUNK_SIZE ? ARRAY_CHUNK_SIZE : size;
                length = 0; 
            }
            
            Object *add (Object &o)
            {
                return trueAdd(o);
            }
            Object *add (Object *o)
            {
                return trueAdd(*o);
            }
            Object *addHead (Object &o)
            {
                return trueAdd(o, 0);
            }
            Object *addHead (Object *o)
            {
                return trueAdd(*o, 0);
            }
            Object *addTail (Object &o)
            {
                return trueAdd(o);
            }
            Object *addTail (Object *o)
            {
                return trueAdd(*o);
            }
        
    };




#endif

/*End of file*/

