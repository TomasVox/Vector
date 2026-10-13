# std::Vector implementation
This is not a complex or complete std::Vector implementation. It is an implementation with the essential functions of memory management. Since std::Vector has to manage a lot of exceptions, it would be ridiculous to recreate something similar to it. So, I created this with learning and demonstration purposes. Thus, everything seen in this repository it is far from being something as good as the std::Vector. But it encapsulates the functioning of std::Vector.

## Problem std::Vector solves
The STL Vector is more than a dynamic array. In sophisticated words, it is a container for contiguos memory, resources management, operators and an interface.
It abstracts a whole process to manage memory, so you do not have to manage more responsability.

Cpp is a languaje that gives you the responsability to manage the memory you use. So, tools like STL Vector are a game changer.

### How does it solves it?
As I said before, it encapsulates a whole process of memory management. This implementation is an example of it. But lets take a look into how this works low level.
We are going to talk in a operating system level. First of all, in a logical way, RAM is not totally contiguos. RAM is divided in sections, from an X addres to a Y addres. Every section of RAM have its purpose. Think it like boxes with tags. These are the sections: Text, data segment, BSS, the heap and the stack. We are going to focus our interest in the stack and the heap. 
When we are programming, usually we are interacting with the heap and the stack. 

## Problems I had during this implementation
As said before, this is an implementation with learning purposes. Thus, problems and errors are common. The main problem I had while implementing std::Vector was doing something complex, but readable. Something elegant. The first structure was planned as follows:
- Vector.h (Definitions and declarations) -> Vector.cpp (Implementation)
- Allocator.h -> Allocator.cpp

The problem here is: Vector class is a template. It makes the code in the implementation too cumbersome to read. I searched alternative methods to keep that structure and keep the code readable and clean. I found out, in this particular case (There could be more cases, lot more cases) it's better the simplicity rather a complex structure that keeps the code readable. So I had to change Vector.cpp to a template implementation file: Vector.tpp. It is actually still pretty readable.

The 90% of the time I was figuring out how to code.
Also, I would not consider this as a limitation, but it was surprising that std::Vector is not exception safety in every part of his structure. Sometimes, it prefers optimization over security. It is actually, beneficial. It delegates responsability to the programmer who is going to use the library, but you get more speed. In functions like: at() or the access operator. Wraping the code in a try-catch block forces to use more clock cicles.

I really struggled implementing all the insert overloads. It was a challenge, cause it is actually a demanding function. It swaps objects (in my implementation) to create space, also, constructs and destroys objects. It is demanding. I could not find a way to apply the DRY strategy (Don't Repeat Yourself). I could not, even though I did it many times in the implementation. Thus, many code lines just to overload the insert function.

You see the paragraph above? Well, Actually, I could. I could apply the DRY strategy to insert.

This was really challenging and, actually, really educative. You learn a lot of memory management and how to create a class with operators.

### Preferences and decisions while coding
Due my interest in learning how to manage memory, I implemented everything manual. Thus, there are functions, like the copy assigment operator that OVER COMPLICATES the implementation. In that particular function I could have done just:
-> swap(*this, other);
But that would not be educative for me.

## Limitations
This std::Vector implementation does not include all std::Vector functions. Nonetheless, it includes most of them. Here is a list of all the funcions implemented: 
- Access features:
    - at()
    - front()
    - back()
    - data()
    - begin()
    - cbegin()
    - end()
    - cend()
    - get_allocator()
- Capacity features:
    - empty()
    - size()
    - max_size()
    - capacity()
    - shrink_to_fit()
    - reserve()
- Modifiers features:
    - push_back()
    - pop_back()
    - clear()
    - emplace()
    - emplace_back()
    - resize()
    - swap()
    - insert()
    - erase()
You can see the overloads and the complete definition in include/vector.h or for more information, all the definitions are from: https://en.cppreference.com/cpp/container/vector

I must mention, the exeption safety implemented is not the most acurrate. In a std::Vector, if something happens, you keep your original vector. I could replicate that, but in this implementation I decided just for simplicity and to keep it manual. 
