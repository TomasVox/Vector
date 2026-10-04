

#ifndef VECTOR_H
#define VECTOR_H

#include "allocator.h"

template <typename T>
class Vector
{
    T* data;
    size_t size;
    Allocator<T> alloc;

    public:
        Vector() : alloc(Allocator<T>()){}

        Vector() noexcept(noexcept(Allocator<T>())) : Vector(Allocator<T>()){}

        explicit Vector(const Allocator<T>& alloc = Allocator()) : alloc(alloc) {};
        
};

#endif