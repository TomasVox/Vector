

#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <initializer_list>

#include "allocator.h"

template <typename T>
class Vector
{
    public:
        using value_type = T;
        using size_type = std::size_t;

        using reference = T&;
        using const_reference = const T&;

        using iterator = T*;
        using const_iterator = const T*;

        Vector() noexcept(noexcept(Allocator<value_type>())) : Vector(Allocator<value_type>());

        explicit Vector(const Allocator<value_type>& alloc = Allocator<value_type>()) : _alloc(alloc);

        explicit Vector(size_type count, const Allocator<value_type>& alloc = Allocator<value_type>());

        Vector(size_type count, const_reference value, const Allocator<value_type>& alloc = Allocator<value_type>());

        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;
        Vector(Vector&& other, const Allocator<value_type>& alloc);

        Vector(std::initializer_list<value_type> list, const Allocator<value_type>& alloc = Allocator<value_type>());

        ~Vector();

        Vector& operator=(const Vector& other);
        Vector& operator=(Vector&& other) noexcept ;
        Vector& operator=(std::initializer_list<value_type> list);

        reference operator[](size_type index);
        const_reference operator[](size_type index) const;

        /*
            Access features and iterators:
        */
       reference at(size_type pos); // Access element in pos with bounds checking.
       const_reference at(size_type pos) const;
       reference front(); // Access first element.
       const_reference front() const;
       reference back(); // Access last element.
       const_reference back() const;
       iterator data(); // Direct access to the contiguos data.
       const_iterator data() const;

       iterator begin() noexcept;
       const_iterator begin() const noexcept;
       const_iterator cbegin() const noexcept;
       iterator end() noexcept;
       const_iterator end() const noexcept;
       const_iterator cend() const noexcept;

       Allocator<value_type> get_allocator() const noexcept;

        /*
            Capacity features:
        */
        bool empty() const; // Checks if the vector is either empty or not.
        size_type size() const; // Returns the number of elements.
        size_type max_size() const; // Returns the max number of possible elements.
        size_type capacity() const; // Returns the number of elements that can be held in currently allocated storage.
        void shrink_to_fit(); // Reduces memory usage by freeing unused memory.
        void reserve(size_type capacity); // Returns the number of elements that can be held in currently allocated storage.
        
        /*
            Modifiers features:
        */
       void push_back(const_reference element); // Adds an element to the end.
       void push_back(T&& element);
       void pop_back(); // Removes the last element.
       void clear(); // Clears the content.

       template <class... Args>
       iterator emplace(const_iterator pos, Args&&... args); // Constructs element in-place.
       template <class... Args>
       reference emplace_back(Args&&... args); // Constructs element in-place at the end.

       void resize(size_type new_size); // Changes the number of elements stored.
       void resize(size_type new_size, const_reference value);
       void swap(Vector& other) noexcept; // Swaps the content.
       iterator insert(const_iterator pos, const_reference value); // Inserts elements.
       iterator insert(const_iterator pos, T&& value);
       iterator insert(const_iterator pos, size_type count, const_reference value);
       iterator insert(const_iterator pos, std::initializer_list<value_type> list);
       template <class InputIt>
       iterator insert(const_iterator pos, InputIt first, InputIt last);
       iterator erase(const_iterator pos); // Erases elements.
       iterator erase(const_iterator first, const_iterator last);
    private:
        value_type* _data;
        std::size_t _size;
        std::size_t _capacity;
        Allocator<T> _alloc;
};

#endif