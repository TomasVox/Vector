#include "vector.h"
#include "allocator.h"

#include <cstddef>
#include <stdexcept>
#include <limits>

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

/*
    Access features implementation.
*/
template <typename T>
T& Vector<T>::at(std::size_t pos)
{
    if (pos >= _size)
        throw std::out_of_range("Vector::at: trying to access an index out of range.s");
    return _data[pos];
}
template <typename T>
const T& Vector<T>::at(std::size_t pos) const
{
    if (pos >= _size)
        throw std::out_of_range("Vector::at: trying to access an index out of range.s");
    return _data[pos];
}

template <typename T>
T& Vector<T>::front()
{
    return _data[0]; // While implementing this, this does not have a behaviour if there is no objects in the vector
}
template <typename T>
const T& Vector<T>::front() const
{
    return _data[0];
}

template <typename T>
T& Vector<T>::back()
{
    return _data[_size - 1];
}
template <typename T>
const T& Vector<T>::back() const
{
    return _data[_size - 1];
}

template <typename T>
T* Vector<T>::data()
{
    return _data;
}
template <typename T>
const T* Vector<T>::data() const
{
    return _data;
}

template <typename T>
T* Vector<T>::begin() noexcept
{
    return _data;
}
template <typename T>
const T* Vector<T>::begin() const noexcept
{
    return _data;
}
template <typename T>
const T* Vector<T>::cbegin() const noexcept
{
    return _data;
}

template <typename T>
T* Vector<T>::end() noexcept
{
    return _data + _size;
}
template <typename T>
const T* Vector<T>::end() const noexcept
{
    return _data + _size;
}
template <typename T>
const T* Vector<T>::cend() const noexcept
{
    return _data + _size;
}

template <typename T>
Allocator<T> Vector<T>::get_allocator() const noexcept
{
    return _alloc;
}

/*
    Capacity features implementation:
*/
template <typename T>
bool Vector<T>::empty() const
{ 
    return _size > 0;
}
template <typename T>
std::size_t Vector<T>::size() const
{
    return _size;
}
template <typename T>
std::size_t Vector<T>::max_size() const
{
    return std::numeric_limits<std::size_t>::max() / sizeof(T);
}
template <typename T>
std::size_t Vector<T>::capacity() const
{
    return _capacity;
}

template <typename T>
void Vector<T>::shrink_to_fit()
{
    if (_capacity == _size)
        return;
    Vector<T> temp(_alloc);
    temp.reserve(_size);
    for (std::size_t i = 0; i<_size; i++, ++_size)
        ::new (temp._data + i) T(_data[i]);
    *this = std::move(temp);
}

template <typename T>
void Vector<T>::reserve(std::size_t capacity)
{
    if (capacity <= _capacity)
        return;
    T* n_data = _alloc.allocate(capacity);

    std::size_t constructed = 0;
    try {
        for (std::size_t i = 0; i<_size; i++, ++constructed)
            ::new (n_data + i) T(std::move(_data[i]));
    } catch (...) {
        for (std::size_t i = 0; i<constructed; i++)
            n_data[i].~T();
        _alloc.deallocate(n_data); 
        
        throw;
    }
    for (std::size_t i = 0; i<_size; i++)
        n_data[i].~T();
    _alloc.deallocate(_data);

    _capacity = capacity;
    _data = n_data;
}

/*
    Modifiers features
*/

template <typename T>
void Vector<T>::push_back(const T& element)
{
    if (_size >= _capacity)
        reserve(_capacity > 0 ? _capacity * 2 : 8);
    
    ::new (end()) T(element);
    ++_size;
}
template <typename T>
void Vector<T>::push_back(T&& element)
{
    if (_size >= _capacity)
        reserve(_capacity > 0 ? _capacity * 2 : 8);
    
    ::new (end()) T(std::move(element));
    ++_size;
}

template <typename T>
void Vector<T>::pop_back()
{
    if (_size <= 0)
        return;
    _data[_size - 1].~T();
    --_size;
}

template <typename T>
void Vector<T>::clear()
{
    if (_size <= 0)
        return;
    for (std::size_t i = 0; i<_size; i++)
    {
        _data[i].~T();
        --_size;
    }
}

/*
STRUGGLING IMPLEMENTING THIS ONE. I'M GONNA IMPLEMENT IT LATER

template <typename T>
template <class... Args>
T* Vector<T>::emplace(const T* pos, Args&&... args)
{
    std::size_t index = _data + pos;
    push_back(T(std::forward<Args>(args)));

    if (index == _size)
        return;
    for (std::size_t i = _data + index; i>index; i--)


}
*/

template <typename T>
template <class... Args>
T& Vector<T>::emplace_back(Args&&... args)
{
    push_back(std::move(T(std::forward<Args>(args)...)));
    return _data[_size - 1];
}

template <typename T>
void Vector<T>::resize(std::size_t new_size)
{
    if (new_size < _size)
    {
        for (std::size_t i = new_size - 1; i<_size; i++)
            _data[i].~T();
    }
    if (new_size > _size)
    {
        if (new_size > _capacity)
            reserve(new_size > _capacity * 2 ? new_size * 2 : _capacity * 2);

        for (std::size_t i = _size; i<new_size; i++)
            new (data + i) T();
    }
    _size = new_size;
}