#ifndef QUEUE_CLASS
#define QUEUE_CLASS
#include "memory_template.h"


#define QUEUE_DEFAULT_CHUNK_SIZE 48

template <typename Object>
    class Queue : private Allocator<Object> {
    private :
        Object *array;
        int head;
        int tail;
        int capacity;
        int elements;
    
    
    public :
        Queue ()
        {
            array = this->NewlArray(QUEUE_DEFAULT_CHUNK_SIZE);
            this->capacity = QUEUE_DEFAULT_CHUNK_SIZE;
            this->elements = 0;
            this->tail = 0;
            this->head = 0;
        }
    
    
    
};



template <typename Object, int Capacity>
    class QueueBase {
    private :
        
        Object array[Capacity];
        int head;
        int tail;
        int capacity;
        int elements;
    
     public :
         
        QueueBase ()
        {
            this->capacity = Capacity;
            this->elements = 0;
            this->tail = 0;
            this->head = 0;
        }   
        
        void push (Object &o)
        {
            if (head + 1 == tail || (head + 1 == capacity && tail == 0)) {
                return;
            }
            array[head] = o;
            elements++;
            head++;
            if (head == capacity) {
                head = 0;
            }
            
        }
        void push (Object *o)
        {
            push(*o);
        }
        
        Object pop ()
        {
            if (tail == capacity) {
                tail = 0;
            }
            if (tail == head) {
                return array[0];
            }  
            tail++;
            elements--;
            return array[tail - 1];
            
        }
        
        bool isEmpty ()
        {
           if (elements == 0) {
               return true;
           }
           return false;
        }
        bool isFull ()
        {
            if (elements >= capacity) {
                return true;
            }
            return false;
        }
        
        uint32_t size ()
        {
            return elements;
        }
        
        Object *getArray ()
        {
            return array;
        }
        
        
        bool equate (Queue<Object> q)
        {
            return false;
        }
        
    };


#endif

/*End of file*/


