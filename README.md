### Why another circular buffer?
[Circluar buffers](https://en.wikipedia.org/wiki/Circular_buffer) are commonly used in embedded systems. However, many development environments don't have support for STL by default (such as avr-gcc-g++).
If they would, STL's `std::forward_list` could be used instead.

### Implementation
The circular buffer is implemented in C++11/templates as singly linked list on stack, which makes it suitable for low-overhead systems. The buffer size isn't required to be power of two (16, 32, 64, ...). The circular buffer encapsulates its memory address, so it should be safe. The elements it carries aren't limited to stack only; they could be anything, such as `std:string`s, as long as they handle their own memory.

This implementation isn't thread safe and it doesn't use exceptions.

### How to use it?
Header-only inclusion is enough.

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

### Other

There are unefficient implementations of circular buffers available on the Internet. Some of them use random access with the help of modulo in bounds checking, which is costly in embedded systems.

**TODO**:
* support initializer lists

I wouldn't use this for anything serious.
