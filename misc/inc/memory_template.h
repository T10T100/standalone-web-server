#ifndef MEMORY_TEMPLATE_CPP
#define MEMORY_TEMPLATE_CPP

#include <stdint.h>

void *Alloc (uint32_t);
int32_t Free (void *); 

template <typename native>
class Allocator {
  public:
    /*
    Allocator ();
    ~Allocator ();
    */
  template <typename Obj>
    Obj *New ();

  template <typename Obj>
    Obj *New (uint32_t); 
	
	template <typename Obj>
    Obj *NewArray (uint32_t);
	
	template <typename Obj>
    Obj **NewlArray (uint32_t);

  template <typename Obj>
    int32_t Delete (Obj *); 
	
    native *New ();
    native *New (uint32_t); 
	native *NewArray (uint32_t);
	native **NewlArray (uint32_t);
    virtual void *alloc (uint32_t size)
    {
        return Alloc (size);
    }
    virtual void free (void *p)
    {
        Free(p);
    }
  
  private:     
};


/*
Allocator::Allocator () {}
Allocator::~Allocator() {}
*/

template <typename native>
template <typename Obj>
  Obj *Allocator<native>::New ()
{
  return (Obj *)this->alloc (sizeof(Obj));
}

template <typename native>
template <typename Obj>
  Obj *Allocator<native>::New (uint32_t size)
{
  return (Obj *)this->alloc (sizeof(Obj) + size);
}

template <typename native>
template <typename Obj>
  Obj *Allocator<native>::NewArray (uint32_t size)
{
  return (Obj *)this->alloc (sizeof(Obj) * size);
}

template <typename native>
template <typename Obj>
  Obj **Allocator<native>::NewlArray (uint32_t size)
{
  return (Obj **)this->alloc (sizeof(Obj *) * size);
}

template <typename native>
template <typename Obj>
  int32_t Allocator<native>::Delete (Obj *o)
{
  this->free(static_cast<void *>(o)); 
  return 0;
}

template <typename native>
  native *Allocator<native>::New ()
{
  return (native *)this->alloc (sizeof(native));
}

template <typename native>
  native *Allocator<native>::New (uint32_t size)
{
  return (native *)this->alloc (sizeof(native) + size);
}

template <typename native>
  native *Allocator<native>::NewArray (uint32_t size)
{
  return (native *)this->alloc (sizeof(native) * size);
}


template <typename native>
  native **Allocator<native>::NewlArray (uint32_t size)
{
  return (native **)this->alloc (sizeof(native *) * size);
}

void *operator new[] (uint32_t size);
    /*
{
    return Alloc(size);
}
*/
void operator delete[] (void *p);
    /*
{
    Free(p);
}
*/
#endif /*MEMORY_TEMPLATE_CPP*/

//extern Allocator RAM;

/*End Of File*/

