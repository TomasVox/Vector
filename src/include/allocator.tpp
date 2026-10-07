#include "allocator.h"

#include <cstddef>
#include <new>
#include <limits>

template <typename T>
T* Allocator<T>::allocate(std::size_t count)
{
    if (count > std::numeric_limits<std::size_t>::max() / sizeof(T))
        throw std::bad_array_new_length();

    T* p = static_cast<T*>(
        ::operator new(count * sizeof(T))
    );
    return p;
}

template <typename T>
void Allocator<T>::deallocate(T* p)
{
    ::operator delete(p)
}