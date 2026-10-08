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
Vector<T>::Vector(std::initializer_list<T> list, const Allocator<T>& alloc)
: _alloc(alloc),
  _data(_alloc.allocate(list.size())),
  _capacity(list.size()),
  _size(list.size())
{
    for (std::size_t i = 0; i<_size; i++)
        new (_data + i) T(list[i]);
}

template <typename T>
Vector<T>::~Vector()
{
    for (std::size_t i = 0; i<_size; i++)
        _data[i].~T();
    _alloc.deallocate(_data);
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (this == &other) return *this;

    clear();
    if (_capacity < other._size)
        reserve(other._capacity);

    try 
    {
        for (std::size_t i = 0; i<other._size; i++, ++_size)
            ::new (_data + i) T(other._data[i]);
    } 
    catch(...)
    {
        for (std::size_t i = 0; i<_size; i++)
            _data[i].~T();
        _size = 0;
        throw;
    }

    _alloc = other._alloc;

    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
    try
    {
        for (std::size_t i = 0; i<_size; i++)
            _data[i].~T();
    } catch (...)
    {
        throw;
    }
    _alloc.deallocate(_data);

    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;
    _alloc = other._alloc;

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;

    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> list)
{
    Vector<T> temp(list);
    *this = std::move(temp);
    return *this;
    /*
    clear();
    if (_capacity < list.size8)
        reserve(list.size());

    try 
    {
        for (std::size_t i = 0; i<other._size; i++, ++_size)
            ::new (_data + i) T(other._data[i]);
    } 
    catch(...)
    {
        for (std::size_t i = 0; i<_size; i++)
            _data[i].~T();
        _size = 0;
        throw;
    }
    */
}

template <typename T>
T& Vector<T>::operator[](std::size_t index)
{
    return _data[index];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t index) const
{
    return _data[index];
}