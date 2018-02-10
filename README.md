### Why another circular buffer?
[Circluar buffers](https://en.wikipedia.org/wiki/Circular_buffer) are commonly used in embedded systems. However, many development environments don't support STL by default (such as avr-gcc-g++). There is a [standard proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0059r1.pdf) for circular buffers in C++.

### Implementation
The circular buffer is implemented in C++11/templates as singly linked list on top of stack, which makes it suitable for low-overhead systems. The buffer size isn't required to be power of two (16, 32, 64, ...). The circular buffer encapsulates its memory address, so it should be safe. The elements it carries aren't limited to stack only; they could be anything, such as `std:string`s, as long as they handle their own memory.

This implementation isn't thread safe and doesn't use exceptions.

### How to install/use it?
Header-only inclusion is enough.

    wget https://raw.githubusercontent.com/Osteri/circularbuffer/master/circularbuffer.hpp
    #include "circularbuffer.hpp"

### Examples
**Case 1:**
Circular buffer with 4 `char` elements:

    #include "circularbuffer.hpp"
    #include <iostream>

    int main() 
    {
        CircularBuffer<char, 4> cb;

        while(!cb.full()) {
            cb.put('c');
            cb.put('h');
            cb.put('a');
            cb.put('r');
        }
        while(cb.has_unread_data())
            std::cout << cb.get(); 
    }
    
![stack based singly linked list circular buffer](https://github.com/Osteri/circularbuffer/blob/master/wiki/char.png?raw=true)

***
**Case 2:**
Circular buffer with 4 `std::string` elements:

    #include "circularbuffer.hpp"
    #include <iostream>

    int main() 
    {
        CircularBuffer<std::string, 4> cb;

        while(!cb.full()) {
            cb.put(std::string("These"));
            cb.put(std::string("are"));
            cb.put(std::string("in"));
            cb.put(std::string("heap"));
        }
        std::cout << cb << std::endl;
    }

In case 2, `std::string`s are not necessarily located in heap, due to Small Buffer Optimization (SBO). For simplicity, they are represented in heap. The actual memory representation is decided by the compiler.

![stack based singly linked list circular buffer](https://github.com/Osteri/circularbuffer/blob/master/wiki/string.png?raw=true)

***
**Case 3:**
Circular buffer with 2 `char` elements:

    #include "circularbuffer.hpp"
    #include <iostream>

    int main() 
    {
        CircularBuffer<char, 2> cb;
        cb.put('a');
        cb.put('b');
        cb.put('c');
        cb.put('d');
    }

Notice how we overwrite the oldest data when it isn't being controlled. The characters 'a' and 'b' are now being overwritten with 'c' and 'd', since we didn't check before writing and because the buffer size is 2. This could be used when maximal throughput is very important.

***

### Run simple tests
    cd tests/
    mkdir build && cd !$
    cmake ..
    make
    ./circularbuffer-tests

### Problems

The circular buffer doesn't work correctly when used with begin and end iterators, since they point to the same object when the buffer is full. They only work correctly when the buffer isn't full.

For example:

    while (!cb.full())
      cb.put('a');
    for (auto it = cb.begin(); it != cb.end(); it++) // won't iterate if cb is full

This code doesn't work if the buffer (cb) is full. When the cb is full or empty `it != cb.end()` is always true, because they point to the same object. The elements aren't iterated in this case.

    while (!cb.full())
      cb.put('a');
    while(cb.has_unread_data()) // iterates correctly

This code iterates all fields correctly.

Possibly remove `begin()` and `end()` in future impelementations, or find a more clever way to implement them. Now they are just for helping initialization. Therefore, avoid using `begin()` and `end()` iterators, use `full()` and `has_unread_data()` for flow control. For setting use `put()` and for getting use `get()`.

### Other

There are unefficient implementations of circular buffers available on the Internet. Some of them use random access with the help of modulo in bounds checking, which is costly in embedded systems.

#### Asymptotic complexity for API calls

Size of the buffer represents n:

* `CircularBuffer::begin()` - **𝛰(1)**
* `CircularBuffer::end()` - **𝛰(1)**
* `CircularBuffer::put()` - **𝛰(1)**
* `CircularBuffer::get()` - **𝛰(1)**
* `CircularBuffer::size()` - **𝛰(1)**
* `CircularBuffer::clear()` - **𝛰(n)**
* `CircularBuffer::full()` - **𝛰(1)**
* `CircularBuffer::has_unread_data()` - **𝛰(1)**


**TODO**:
* support initializer lists

I wouldn't use this for anything serious.
