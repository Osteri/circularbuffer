### Why another circular buffer?
[Circluar buffers](https://en.wikipedia.org/wiki/Circular_buffer) are commonly used in embedded systems. However, many development environments don't support STL by default (such as avr-gcc-g++). There is a [standard proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0059r1.pdf) for circular buffers in C++.

### Implementation
The circular buffer is implemented in C++11/templates as singly linked list on top of stack, which makes it suitable for low-overhead systems. The buffer size isn't required to be power of two (16, 32, 64, ...). The circular buffer encapsulates its memory address, so it should be safe. The elements it carries aren't limited to stack only; they could be anything, such as `std:string`s, as long as they handle their own memory.

This implementation isn't thread safe and doesn't use exceptions.

### Design requirements
- fixed size container
- don't require power of 2 sizes
- never expose raw memory address outside
- linked list structure always stack allocated
- use iterators/`put()`/`get()` to iterate through elements
- don't use modulo for element access (slow in some embedded systems)
- support STL/std::<features> as much as possible
- C++11, possibly convertible to C++03
- less code as possible
- small footprint 
- and of course: safe and fast

Right now some parts require STL, but they are trivial and will be removed in the future. They are used to ease debugging.

### How to install/use it?
Header-only inclusion is enough.

    wget https://raw.githubusercontent.com/Osteri/circularbuffer/master/circularbuffer.hpp
    #include "circularbuffer.hpp"

### Examples
**Case 1:**
Circular buffer with 4 `char` elements:

```cpp
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
```
    
![stack based singly linked list circular buffer](https://github.com/Osteri/circularbuffer/blob/master/wiki/char.png?raw=true)

---

**Case 2:**
Circular buffer with 4 `std::string` elements:

```cpp
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
```

In case 2, `std::string`s are not necessarily located in heap, due to Small Buffer Optimization (SBO). For simplicity, they are represented in heap. The actual memory representation is decided by the compiler.

![stack based singly linked list circular buffer](https://github.com/Osteri/circularbuffer/blob/master/wiki/string.png?raw=true)

---

**Case 3:**
Circular buffer with 2 `char` elements:

```cpp
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
```

Notice how we overwrite the oldest data when it isn't being controlled. The characters 'a' and 'b' are now being overwritten with 'c' and 'd', since we didn't check before writing and because the buffer size is 2. This could be used when maximal throughput is very important.

---

### Run simple tests
    cd tests/
    mkdir build && cd build
    cmake ..
    make
    ./circularbuffer-tests

### Other

There are unefficient implementations of circular buffers available on the Internet. Some of them use random access with the help of modulo in bounds checking, which is costly in embedded systems.

#### Asymptotic complexity of API calls

Size of the buffer represents n:

* `CircularBuffer::begin()` - **𝛰(1)**
* `CircularBuffer::end()` - **𝛰(1)**
* `CircularBuffer::put()` - **𝛰(1)**
* `CircularBuffer::get()` - **𝛰(1)**
* `CircularBuffer::capacity()` - **𝛰(1)**
* `CircularBuffer::full()` - **𝛰(1)**
* `CircularBuffer::has_unread_data()` - **𝛰(1)**
