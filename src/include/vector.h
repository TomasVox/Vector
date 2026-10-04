

#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <initializer_list>

#include "allocator.h"

template <typename T>
class Vector
{
    T* _data;
    std::size_t _size;
    Allocator<T> _alloc;

    T* _begin;
    T* _end;

    public:
        Vector() : _alloc(Allocator<T>()){};

        Vector() noexcept(noexcept(Allocator<T>())) : Vector(Allocator<T>()){};

        explicit Vector(const Allocator<T>& alloc = Allocator()) : _alloc(alloc){};
        explicit Vector(const Allocator<T>& alloc){};

        explicit Vector(std::size_t count, const Allocator<T>& alloc = Allocator()){};
        explicit Vector(std::size_t count, const T& value = T(), const Allocator<T>& alloc = Allocator()){};

        Vector(std::size_t count, const T& value = T(), const Allocator<T>& alloc = Allocator()){};

        Vector(const Vector& other){};
        Vector(Vector&& other){};
        Vector(Vector&& other, const Allocator<T>& alloc){};

        Vector(const std::initializer_list<T> list, const Allocator<T>& alloc = Allocator()){};

        ~Vector(){};

        Vector& operator=(const Vector& other){};
        Vector& operator=(Vector&& other){};
        Vector& operator=(Vector&& other) noexcept {};
        Vector& operator=(std::initializer_list<T> list){};

        T& operator[](std::size_t index){};
        const T& operator[](std::size_t index) const {};

        /*
            Access features and iterators:
        */
       T& front() {}; // Access first element.
       const T& front() const {};
       T& back() {}; // Access last element.
       const T& back() const {};
       T* data() {}; // Direct access to the contiguos data.
       const T* data() const {};

       T* begin() noexcept {};
       const T* cbegin() const noexcept {};
       T* end() noexcept {};
       const T* cend() const noexcept {};

        /*
            Capacity features:
        */
        bool empty() const {}; // Checks if the vector is either empty or not.
        std::size_t size() const {}; // Returns the number of elements.
        std::size_t max_size() const {}; // Returns the max number of possible elements.
        std::size_t capacity() const {}; // Returns the number of elements that can be held in currently allocated storage.
        void shrink_to_fit() {}; // Reduces memory usage by freeing unused memory.
        void reserve(std::size_t capacity) {}; // Returns the number of elements that can be held in currently allocated storage.

        /*
            Modifiers features:
        */
       void push_back(const T& element){}; // Adds an element to the end.
       void push_back(T&& element){};
       void pop_back(){}; // Removes the last element.
       void clear(){}; // Clears the content.

       template <class... Args>
       T* emplace(T* pos, Args&&... args){}; // Constructs element in-place.
       void emplace_back(Args&&... args){}; // Constructs element in-place at the end.
       T& emplace_back(Args&&... args){};

       void resize(std::size_t new_size){}; // Changes the number of elements stored.

       void swap(Vector& other) noexcept {}; // Swaps the content.
};

#endif