#ifndef ALLOCATOR_H
#define ALLOCATOR_H

template <typename T>
class Allocator 
{
    public:
    Allocator() noexcept = default;

    T* allocate(std::size_t count);
    void deallocate(T* p, std::size_t count);
};

#endif