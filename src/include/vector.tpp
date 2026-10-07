#include "vector.h"
#include "allocator.h"

#include <cstddef>

/*
    Constructors and destructor
*/
template <typename T>
Vector<T>::Vector() noexcept(noexcept(Allocator<T>())) : Vector(0, T(), Allocator<T>()) {}

template <typename T>
Vector<T>::Vector(const Allocator<T>& alloc) : Vector(0, T(), alloc) {}

template <typename T>
Vector<T>::Vector(std::size_t count, const Allocator<T>& alloc) : Vector(count, T(), alloc) {}

template <typename T>
Vector<T>::Vector(std::size_t count, const T& value, const Allocator<T>& alloc)
: _alloc(alloc),
  _data (count > 0 ? _alloc.allocate(count) : nullptr),
  _capacity(count),
  _size(count)
{
    for (std::size_t i = 0; i<_size; i++)
        new (_data + i) T(value);
}

template <typename T>
Vector<T>::Vector(const Vector& other) 
: _alloc(other._alloc),
  _capacity(other._capacity),
  _size(other._size),
  _data(_capacity > 0 ? _alloc.allocate(_capacity) : nullptr)
{
    for (std::size_t i = 0; i<_size;i++)
        new (_data + i) T(other._data[i]); // Using _data[i] is equals as going to an address and seeing what is inside.
}

template <typename T>
Vector<T>::Vector(Vector&& other, const Allocator<T>& alloc) 
: _alloc(alloc),
  _capacity(other.capacity),
  _size(other._size),
  _data(_capacity > 0 ? _alloc.allocate(_capacity) : nullptr)
{
    for (std::size_t i = 0; i<_size; i++)
        new (_data + i) T(other._data[i]);
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept : Vector(other, other._alloc) {}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list, const Allocator<T>& alloc) {}

template <typename T>
Vector<T>::~Vector(){}