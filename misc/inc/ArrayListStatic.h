#ifndef ARRAY_LIST_BASE
#define ARRAY_LIST_BASE
#include <stdint.h>



template <typename Object>
	class DefaultArrayListBaseNode {
        public :
        DefaultArrayListBaseNode ()
		{
            prevLink = nullptr;
            nextLink = nullptr;
		}    
		/*Default constructor*/
		/*Default interface to compare objects*/
		bool equals (DefaultArrayListBaseNode<Object> &node)
		{
			return this->equalsTo(node);
		}		
		Object *prevLink;
	    Object *nextLink;
		/*Object defined interface*/
        virtual bool equalsTo (DefaultArrayListBaseNode<Object> &node)
        {
            return false;
        }
        
		private:
	};

	template <typename Object>
		class ArrayListBase {
			public: 
			ArrayListBase ()
			{
				this->firstNode = nullptr;
				this->lastNode = nullptr;
                elementCount = 0;
			}
			
			/*put new item without compare - as result create an unsorted list*/
			Object *add (Object &item)
			{
				this->elementCount++;
                    Object *i,*j;
                    if (this->firstNode == nullptr) {
                        this->firstNode = &item;
                        this->lastNode = &item;
                        item.nextLink = nullptr;
                        item.prevLink = nullptr;
                        return &item;
                    }
                    i = this->firstNode;
                    j = nullptr;
                    while (i != nullptr) {
                        if (i->equals(item) == true) {
                            j = i;
                            i = i->nextLink;
                            continue;
                        }
                        if (i->prevLink != nullptr) {
                            i->prevLink->nextLink = &item;
                            item.nextLink = i;
                            item.prevLink = i->prevLink;
                            i->prevLink = &item;
                            return &item;
                        }
                        item.nextLink = i;
                        item.prevLink = nullptr;
                        i->prevLink = &item;
                        this->firstNode = &item;
                        return &item;
                    }
                    j->nextLink = &item;
                    item.nextLink = nullptr;
                    this->lastNode = &item;
                    item.prevLink= j;
                    return &item;					
			}
			
			Object *add (Object *item)
			{
				return this->add(*item);
			}
			
			Object *addFirst (Object &item)
			{
				this->elementCount++;
					Object *i;
					if (this->firstNode == nullptr) {
						this->firstNode = &item;
						this->lastNode = &item;
						item.nextLink = nullptr;
						item.prevLink = nullptr;
						return &item;
					}
					i = this->firstNode;
					item.nextLink = i;
					item.prevLink = nullptr;
					i->prevLink = &item;
					this->firstNode = &item;
					return &item;
			}
			
			Object *addFirst (Object *item)
			{
				return this->addFirst(*item);
			}
			
			Object *addLast (Object *item)
			{
				this->elementCount++;
				Object *i;
				if (this->firstNode == nullptr) {
					this->firstNode = item;
					this->lastNode = item;
					lastNode->nextLink = nullptr;
					firstNode->prevLink = nullptr;
					return item;
				}
				i = this->lastNode;
				item->prevLink = i;
				item->nextLink = nullptr;
				i->nextLink = item;
				this->lastNode = item;			
				return item;
			}		
            
            Object *getFirst ()
            {
                return this->firstNode;
            }
			Object *getLast ()
            {
                return this->lastNode;
            }
			/*in this method all items will migrate from one list to another, with
			rewriting their links;
			*/
			ArrayListBase<Object> *addAll (ArrayListBase<Object> &arrayList)
			{
				while (arrayList.isEmpty() == false) {
					this->add(arrayList.removeFirst());
				}
				return this;
			}
			
			template <typename Collection>
			ArrayListBase<Object> *addAll (Collection collection)
			{
				return this;
			}
			
			Object *remove (Object &object)
			{
				if (this->elementCount <= 0){
                    return &object;
                }
				this->elementCount--;
				Object *l = object.prevLink,*r = object.nextLink;
				if (!l&&!r) {
                    this->firstNode = nullptr;
                    this->lastNode = nullptr;
                    return &object;
                }
                if (!l) {
                    this->firstNode = r;
                    r->prevLink = nullptr;
                }   else    {
                    if (r)
                    l->nextLink = r;
                }
                if (!r) {
                    l->nextLink = nullptr;
                    this->lastNode = l;
                }   else    {
                    r->prevLink = l;
                }	
				return &object;
            }
			
			Object *remove (Object *object)
			{
				return this->remove(*object);
			}
			
			Object *removeFirst ()
			{
				if (this->elementCount <= 0){
                    return nullptr;
                }
				this->elementCount--;
                Object *o = firstNode;
                
				if (!o->nextLink) {
                    this->firstNode = nullptr;
                    this->lastNode = nullptr;
                    return o;
                }
                firstNode = o->nextLink;
                firstNode->prevLink = nullptr;
	
				return o;
			}
			
			Object *removeLast ()
			{
				if (this->elementCount <= 0){
                    return nullptr;
                }
				this->elementCount--;
                Object *o = lastNode;
                
				if (!o->prevLink) {
                    this->firstNode = nullptr;
                    this->lastNode = nullptr;
                    return o;
                }
                lastNode = o->prevLink;
                lastNode->nextLink = nullptr;
	
				return lastNode;
			}
			
			void removeAll ()
			{
				while (this->isEmpty() == false) {
                    this->removeFirst();
                }
			}
			
			
			
			
			
			bool isEmpty ()
			{
				if (this->elementCount > 0) {
					return false;
				} else {
					return true;
				}
			}
			
			uint32_t size ()
			{
				return this->elementCount;
			}
			private:
			uint32_t elementCount;
			Object *firstNode;
			Object *lastNode;
		};
#endif /*ARRAY_LIST_BASE*/

/*End of file*/

