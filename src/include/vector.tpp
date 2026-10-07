#include "vector.h"
#include "allocator.h"

#include <cstddef>

template <typename T>
Vector<T>::Vector() noexcept(noexcept(Allocator<value_type>())) : Vector(Allocator<value_type>())
{
    Vector::_size = 0;
    Vector::_capacity = 8;
    Vector::_data = Vector::_alloc.allocate(_capacity);
}