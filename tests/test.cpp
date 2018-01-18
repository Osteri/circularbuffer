#include <iostream>
#include "../circularbuffer.hpp"
#include <type_traits>

/* Tests for circular buffer. */

int main() {

    /* Test circular buffer with 'char' type. */
    CircularBuffer<char, 10> cb_ch;
    /* Write */
    while (!cb_ch.is_full())
        cb_ch.put('o');
    cb_ch.put('a');
    cb_ch.put('b');
    /* Read */
    while (cb_ch.has_unread_data())
        std::cout << cb_ch.get();
    std::cout << '\n';
    std::cout << cb_ch << '\n';

    /* Test circular buffer with 'std::string' type. */
    CircularBuffer<std::string, 5> cb_str;
    /* Write */
    cb_str.put(std::string("First!"));
    cb_str.put(std::string("Second!"));
    cb_str.put(std::string("Third!"));
    cb_str.put(std::string("Fourth!"));
    cb_str.put(std::string("Fifth!"));
    cb_str.put(std::string("Sixth!"));
    cb_str.put(std::string("Seventh!"));
    cb_str.put(std::string("Ninth!"));
    cb_str.put(std::string("Tenth!"));
    cb_str.put(std::string("***"));
    /* Read */
    std::cout << cb_str << '\n';

    /* Test if copy-constructible. It should not be,
     * since the memory is allocated from the stack. */
    std::cout << "Is CircularBuffer<T, N> copy-constructable? ";
    std::string boolean_str =
            std::is_copy_constructible<CircularBuffer<std::string, 1>>::value ? "true" : "false";
    std::cout << boolean_str << '\n';
    // auto cb_str2 = cb_str; // this should not compile
    /* Clear() does not call destructors of the objects.
     * Possibly implement calling destructors in the clear() in the future. */
    cb_str.clear();
    std::cout << cb_str << '\n'; // data is not emptied (yet)

    return 0;
}
