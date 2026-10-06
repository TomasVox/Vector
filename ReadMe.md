# std::Vector implementation
This is not a complex or complete std::Vector implementation. It is an implementation with the essential functions of memory management. Since std::Vector has to manage a lot of exceptions, it would be ridiculous to recreate something similar to it. So, I created this with learning and demonstration purposes. Thus, everything seen in this repository it is far from being something as good as the std::Vector. But it encapsulates the functioning of std::Vector.

## Problem std::Vector solves

### How does it solves it?

## Problems I had during this implementation
As said before, this is an implementation with learning purposes. Thus, problems and errors are common. The main problem I had while implementing std::Vector was doing something complex, but readable. Something elegant. The first structure was planned as follows:
- Vector.h (Definitions and declarations) -> Vector.cpp (Implementation)

The problem here is: Vector class is a template. It makes the code in the implementation too cumbersome to read. I searched alternative methods to keep that structure and keep the code readable and clean. I found out, in this particular case (There could be more cases, lot more cases) it's better the simplicity rather a complex structure that keeps the code readable. So I had to change Vector.cpp to a template implementation file: Vector.tpp. It is actually still pretty readable.