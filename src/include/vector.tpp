#include "vector.h"
#include "allocator.h"

#include <cstddef>
#include <stdexcept>
#include <limits>
#include <iterator>

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

template <typename T>
template <class... Args>
T* Vector<T>::emplace(const T* pos, Args&&... args)
{
    std::size_t index = pos - _data;

    insert(pos, std::forward<T>(args));
    return _data + index;
}

template <typename T>
template <class... Args>
T& Vector<T>::emplace_back(Args&&... args)
{
    push_back(std::move(T(std::forward<Args>(args)...)));
    return _data[_size - 1];
}

template <typename T>
void Vector<T>::resize(std::size_t new_size, const T& value)
{
    if (new_size < _size)
    {
        for (std::size_t i = _size; i>new_size; i--)
            _data[i - 1].~T();
    }
    if (new_size > _size)
    {
        T val;

        if (new_size > _capacity)
        {
            if (&value >= _data && &value <= _data + _capacity)
                val(value);

            std::size_t capacity = (_capacity == 0) ? new_size : _capacity * 2;
            reserve(capacity < new_size ? new_size : capacity); // I decided reserve this amount of memory to avoid reserve excess of memory if it is not necessary.
        }

        for (std::size_t i = _size; i<new_size; i++)
            new (_data + i) T(value);
    }
    _size = new_size;
}

template <typename T>
void Vector<T>::resize(std::size_t new_size)
{
    resize(new_size, T());
}

template <typename T>
void Vector<T>::swap(Vector<T>& other) noexcept
{
    T* data_buffer = other._data;
    std::size_t cap_buffer = other._capacity;
    std::size_t size_buffer = other._size;

    other._data = _data;
    other._capacity = _capacity;
    other._size = _size;

    _data = data_buffer;
    _capacity = cap_buffer;
    _size = size_buffer;
}

template <typename T>
T* Vector<T>::insert(const T* pos, const T& value)
{
    std::size_t index = pos - _data;
    insert(pos, 1, value);

    return _data + index;
}

template <typename T>
T* Vector<T>::insert(const T* pos, T&& value)
{
    std::size_t index = pos - _data;
    insert(pos, 1, std::move(value));

    return _data + index;
}

template <typename T>
T* Vector<T>::insert(const T* pos, std::size_t count, const T& value)
{
    Vector<T> buffer(count, value);

    return insert(pos, buffer.begin(), buffer.end());
}

template <typename T>
T* Vector<T>::insert(const T* pos, std::initializer_list<T> list)
{
    return insert(pos, list.begin(), list.end());
}

template <typename T>
template <class InputIt>
T* Vector<T>::insert(const T* pos, InputIt first, InputIt last)
{
    if (pos < _data || pos > _data + _size)
        return nullptr;
    std::size_t elements = std::distance(first, last); // I had to use std::distance for versatility.
    std::size_t index = pos - _data;

    if (elements == 0)
        return const_cast<T*>(pos);
    
    if (_size + elements > _capacity)
        reserve((_size + elements) * 2);

    for (std::size_t i = _size; i>index; --i)
    {
        ::new (_data + i + elements- 1) T(std::move(_data[i - 1]));
        _data[i - 1].~T();
    }

    std::size_t c_index = index;
    for (InputIt it = first; it != last; ++it)
    {
        ::new (_data + c_index) T(std::move(*it));
        c_index++;
    }
    _size += elements;

    return _data + index;
}

template <typename T>
T* Vector<T>::erase(const T* pos)
{
    if (_size == 0)
        return _data;
    if (pos >= _data + _size || pos < _data)
        return nullptr;
    std::size_t index = pos - _data;
    
    for (std::size_t i = index; i<_size; i++)
        _data[i] = std::move(_data[i + 1]);
    _data[_size - 1].~T();

    --_size;

    return _data + index;
}

template <typename T>
T* Vector<T>::erase(const T* first, const T* last)
{
    if (first >= _data + _size || first < _data)
        return nullptr;
    if (last >= _data + _size || last < _data)
        return nullptr;
    if (last < first)
        return nullptr;

    std::size_t elements = last - first;
    if (elements == 0)
        return const_cast<T*>(first);
    
    std::size_t index = first - _data;
    std::size_t c_index = index;

    for (std::size_t i = _size - elements; i<_size; i++)
    {
        _data[c_index] = std::move(_data[i]);
        _data[i].~T();
        ++c_index;
    }
    _size -= elements;
    return _data + index;
}