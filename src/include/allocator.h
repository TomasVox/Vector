#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>

template <typename T>
class Allocator 
{
    public:
    using value_type = T;
    using size_type = std::size_t;

    Allocator() noexcept = default;

    value_type* allocate(size_type count);
    void deallocate(value_type* p);
};

#endif