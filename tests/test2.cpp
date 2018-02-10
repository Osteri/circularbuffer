#include <iostream>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <typeinfo>

#include "../circularbuffer.hpp"

std::string bool_to_str(bool val) {
    return val ? "true" : "false";
}

int main() {
    {
        CircularBuffer<char, 3> cb;
        cb.put('a'); cb.put('b'); cb.put('c');
//        while(cb.has_unread_data())
//            std::cout << cb.get();
//        std::cout << '\n';
    }

    CircularBuffer<char, 4> cb;
    char letter = 'a';
    for(auto& i : cb) {
        i.data = letter++;
    }


    std::cout << cb;
    /************************************************************
     * Test compatibility with InputIterator.
     *
     * http://en.cppreference.com/w/cpp/concept/InputIterator
     ************************************************************/
//    {
//        /* Expression: i != j
//         * Return: contextually convertible to bool
//         * */
//        {
//            CircularBuffer<char, 3> cb;     cb.put('a'); cb.put('b'); cb.put('c');
//            auto i = cb.begin();
//            auto j = cb.end();
//            std::cout << "bool (i != j) = " << (i != j) << '\n';
//            std::cout << '\n';
//        }

//        /* Expression: *i
//         * Return: reference, convertible to value_type
//         * */
//        {
//            CircularBuffer<char, 3> cb;     cb.put('a'); cb.put('b'); cb.put('c');
//            auto i = cb.begin();
//            std::cout << *i << '\n';
//            std::cout << '\n';
//        }

//        /* Expression: i->m
//         * Return:
//         * */
//        {
//            CircularBuffer<char, 3> cb;     cb.put('a'); cb.put('b'); cb.put('c');
//            auto i = cb.begin();
//            auto* j = &i;
//            /* Can not do this, because there aren't memebers. */
//            //std::cout << j->difference_type << '\n';
//            std::cout << '\n';
//        }

//        /* Expression: ++i
//         * Return: It&
//         * */
//        {
//            CircularBuffer<char, 3> cb;     cb.put('a'); cb.put('b'); cb.put('c');
//            auto i = cb.begin();
//            std::cout << &i << '\n';
//            std::cout << ++i << '\n';
//            std::cout << '\n';
//        }

//        /* Expression: (void)i++
//         * Return: (void)++i
//         * */
//        {
//            CircularBuffer<char, 3> cb;     cb.put('a'); cb.put('b'); cb.put('c');
//            auto i = cb.begin();
//            (void)++i;
//        }

//        /* Expression: *i++
//         * Return: convertible to value_type
//         * */
//        {
//            CircularBuffer<char, 3> cb;     cb.put('a'); cb.put('b'); cb.put('c');
//            auto i = cb.begin();
//            std::cout << *i++ << '\n';
//            std::cout << '\n';
//        }
//    }
    /************************************************************
     * Test compatibility with OutputIterator.
     *
     * http://en.cppreference.com/w/cpp/concept/OutputIterator
     ************************************************************/

    /* ... */

    /* For loops */
//    {
//        {
//            CircularBuffer<char, 3> cb;
//            cb.put('a'); cb.put('b'); cb.put('c');
//            std::cout << "for(;;;) loop: ";
//            for (auto it = cb.begin(); it != cb.end(); ++it)
//                std::cout << *it;
//            std::cout << "\n";
//        }
//        {
//            CircularBuffer<char, 3> cb;
//            cb.put('a'); cb.put('b'); cb.put('c');
//            std::cout << "for(const auto&) loop: ";
//            for (auto& it : cb) {
//                it = 'a';
//                std::cout << it;
//            }

//            for (auto it : cb) {
//                std::cout << it;
//            }
//            std::cout << "\n";
//        }
//        {
//            CircularBuffer<char, 3> cb;
//            cb.put('a'); cb.put('b'); cb.put('c');
//            std::cout << "for(auto) loop: ";
//            for (auto it : cb)
//                std::cout << it;
//            std::cout << "\n";
//        }

//        CircularBuffer<char, 3> cb;
//        cb.put('a'); cb.put('b'); cb.put('c');
//        std::cout << cb.get();
//        std::cout << cb.get();
//        std::cout << cb.get() << '\n';


//    }



    return 0;
}
