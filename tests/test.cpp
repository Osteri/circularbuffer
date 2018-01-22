#include <iostream>
#include <type_traits>
#include <algorithm>
#include <utility>

#include "../circularbuffer.hpp"

std::string bool_to_str(bool val) {
    return val ? "true" : "false";
}

int main() {

    /* Test circular structure with 'std::string' type. */
    {
        CircularBuffer<std::string, 5> cb;
        /* Write */
        cb.put(std::string("one"));
        cb.put(std::string("two"));
        cb.put(std::string("three"));
        cb.put(std::string("four"));
        cb.put(std::string("five"));
        cb.put(std::string("six"));
        cb.put(std::string("seven"));
        cb.put(std::string("eight"));
        cb.put(std::string("nine"));
        cb.put(std::string("ten"));

        std::cout << "Result: " << cb << '\n';
    }

    /* Test if copy-constructible. It shouldn't be,
     * since the memory is allocated in the stack. */
    std::cout << '\n';
    std::cout << "Is cb copy-constructable? "
              "\n"
              "Result: "
              <<
                 bool_to_str(std::is_copy_constructible<CircularBuffer<std::string,1>>::value)
              << '\n';
    // auto cb_str2 = cb_str; // this should not compile
    /* Clear() does not call destructors of the objects.
     * Possibly implement calling destructors in the clear() in the future. */
    //cb_str.clear();
    //std::cout << "cb_str: " << cb_str << '\n'; // data is not emptied (yet)

    /* Test circular structure with 'char' type. */
    std::cout << '\n';
    {
        CircularBuffer<char, 2> cb;
        cb.put('a'); cb.put('b'); cb.put('c'); cb.put('d');
        std::cout << "Should print 'cd':" "\n" "Result: " << cb << '\n';

        /* Sanity checks for iterator post and pre increment. */
        auto it = cb.begin();
        it++;
        ++it;

//        Improper iterator invalidation causes infinite loop(?)
//        https://stackoverflow.com/questions/16904454/what-is-iterator-invalidation */
//        for (auto it = cb.begin(); it != cb_ch.end(); ++it)
//            std::cout << *it;
    }

    /* These shouldn't compile. Only include compile errors a as comments. */
    {
        CircularBuffer<char, 2> cb;
        auto it = cb.begin();
//        auto cb_2 = cb; // cb shouldn't be copyable, since it is in stack
//        it--; // forward iterator - reverse not possible
//        --it; // same as above
    }

    /* Iterator comparison. */
    std::cout << '\n';
    {
        CircularBuffer<char, 2> a;
        CircularBuffer<char, 2> b;
        auto a_it = a.begin();
        auto b_it = b.begin();
        std::cout << "a == b? " "\n"
                     "Result: " << bool_to_str(a_it == b_it) << '\n';
        std::cout << "a == a? " "\n"
                     "Result: " << bool_to_str(a_it == a_it) << '\n';
    }

    /* Test if iterator is in OutputIterator category.
     * http://en.cppreference.com/w/cpp/concept/OutputIterator */
    std::cout << '\n';
    {
        CircularBuffer<char, 2> cb;
        cb.put('a');
        cb.put('b');
        auto it = cb.begin();
        *it = 'a';
        std::cout << "Iterator should give value 'a' from " << cb << '\n' <<
                     "Result: " << *it << '\n';
        std::cout << "Iterator should give value 'b' from " << cb << '\n' <<
                     "Result: " << *++it << '\n';
    }

    /* Test circular buffer with 'char' type. */
    std::cout << '\n';
    {
        CircularBuffer<char, 10> cb;
        /* Write */
        while (!cb.is_full())
            cb.put('o');
        cb.put('a');
        cb.put('b');

        std::cout << "Iterate cb with incrementing iterator: \n"
                     "Result: ";
        auto it = cb.begin();
        for (int i = 0; i < cb.size(); ++i) {
            std::cout << *it++;
        }
        std::cout << "\n\n";

        cb.put('a');
        cb.put('b');
        std::cout << "Iterate cb with polling cb.has_unread_data() using cb.get(): \n"
                     "Result: ";
        while (cb.has_unread_data())
            std::cout << cb.get();
        std::cout << "\n";
        std::cout << "in " << cb << '\n';
    }

    /* Test std::advance. */
    std::cout << '\n';
    {
        CircularBuffer<char, 3> cb;
        cb.put('a');
        cb.put('b');
        cb.put('c');
        auto it = cb.begin();
        std::advance(it, 2);
        std::cout << "Advance iterator 2 times in "<< cb << '\n'
                  << "Result: "<< *it 
                  << "\n";
    }

    return 0;
}
