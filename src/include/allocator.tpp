#include "allocator.h"

#include <cstddef>

template <typename T>
T* Allocator<T>::allocate(std::size_t count)
{
    /*
        Not finished.
    */

    T* p = static_cast<T*>(
        ::operator new(count * sizeof(T))
    );
    return p;
}

template <typename T>
void Allocator<T>::deallocate(T* p, size_type count)
{
    /*
        Not finished.
    */
    ::operator delete(p);
}