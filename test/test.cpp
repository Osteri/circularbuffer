#include <iostream>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <memory>

#include "gtest/gtest.h"

#include "circularbuffer.hpp"

TEST(general, put_and_get_ints) {
  CircularBuffer<int, 2> cb;
  int i = 1;
  cb.put(i); cb.put(2);
  EXPECT_EQ(cb.get(), 1);
  EXPECT_EQ(cb.get(), 2);
}

TEST(general, full_or_empty) {
  CircularBuffer<std::string, 5> cb;
  EXPECT_TRUE(cb.empty());
  cb.put("new-string");
  EXPECT_FALSE(cb.empty());
  EXPECT_FALSE(cb.full());
  cb.put("not empty anymore");
  cb.put("not empty anymore");
  cb.put("not empty anymore");
  cb.put("not empty anymore");
  EXPECT_TRUE(cb.full());
}

/* Test circular structure with 'std::string' type. */
TEST(general, circular_structure) {
  CircularBuffer<std::string, 5> cb;
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
  EXPECT_EQ(cb.get(), "six");
  EXPECT_EQ(cb.get(), "seven");
  EXPECT_EQ(cb.get(), "eight");
  EXPECT_EQ(cb.get(), "nine");
  EXPECT_EQ(cb.get(), "ten");
}

/* Test if copy-constructible. It shouldn't be,
 * since the memory is allocated in stack. */
TEST(general, copy_constructible) {
  bool val = std::is_copy_constructible<CircularBuffer<std::string,1>>::value;
  EXPECT_FALSE(val);
}

/* Mutate element state with reference. */
TEST(general, mutate_elements) {
  CircularBuffer<char, 5> cb;
  cb.put('H'); cb.put('E'); cb.put('L'); cb.put('L'); cb.put('O');

  // mutate elements
  for(auto& it : cb) {
    it = it+1;
  }
  // print mutated elements
  std::string str;
  for(auto it : cb) {
    str += it;
  }
  EXPECT_EQ(str, "IFMMP"); // HELLO -> IFJMMP; each char is increased by 1
}

/* Test circular buffer with 'char' type. */
TEST(general, capacity_and_full) {
  CircularBuffer<char, 10> cb;
  std::string str;

  EXPECT_TRUE(cb.empty());
  EXPECT_FALSE(cb.full());
  while (!cb.full())
    cb.put('o');
  cb.put('a');
  cb.put('b');

  for (size_t i = 0; i < cb.capacity(); ++i) {
    str += cb.get();
  }
  EXPECT_EQ(str, "aboooooooo");
  EXPECT_EQ(cb.capacity(), 10);
}

TEST(general, has_unread_data) {
  CircularBuffer<char, 5> cb;
  std::string str;

  char ch = 'a';
  for(uint8_t i = 0; i < 3; ch++, i++) {
    cb.put(ch);
  }
  while (cb.has_unread_data()) {
    str += cb.get();
  }
  EXPECT_EQ(str, "abc");
}

/* Sanity checks for iterator post and pre increment. */
TEST(iterator, pre_and_post) {
  CircularBuffer<char, 2> cb;
  cb.put('a'); cb.put('b');
  auto it = cb.begin();
  EXPECT_EQ(*it++, 'a');
  EXPECT_EQ(*++it, 'a');
  EXPECT_EQ(*++it, 'b');
}

/* Check begin() and end() iterator functionality. */
TEST(iterator, begin_and_end) {
  std::string str;
  CircularBuffer<char, 5> cb;
  cb.put('H'); cb.put('E'); cb.put('L'); cb.put('L'); cb.put('O');

  for(auto it = cb.begin(); it != cb.end(); it++) {
    str += *it;
  }
  EXPECT_EQ(str, "HELLO");

  // iterate twice
  std::string str2;
  for(auto it = cb.begin(); it != cb.end(); it++) {
    str2 += *it;
  }
  EXPECT_EQ(str2, "HELLO");
}

/* Iterators point to same memory but are not equal. */
TEST(iterator, begin_and_end_equal) {
  CircularBuffer<char, 5> cb;

  auto b = cb.begin();
  auto e = cb.end();
  EXPECT_FALSE(b == e);
  EXPECT_TRUE(b != e);
}

/* Iterators are different instances but point to the same memory location. */
TEST(iterator, equality) {
  CircularBuffer<char, 5> cb;

  auto b = cb.begin();
  auto e = cb.end();
  auto b2 = cb.begin();
  auto e2 = cb.end();
  EXPECT_TRUE(b == b2);
  EXPECT_TRUE(e == e2);
  EXPECT_FALSE(b == e2);
}

/* OutputIterator functionality. */
TEST(iterator, output) {
  CircularBuffer<char, 2> cb;

  auto it = cb.begin();
  *it = 'g';
  it++;
  *it = 'h';
  it++;
  EXPECT_EQ(*it++,'g');
  EXPECT_EQ(*it++, 'h');
}

//  /* These shouldn't compile. Only include compile errors as comments. */
//  std::cout << '\n';
//  {
//    CircularBuffer<char, 2> cb;
//    auto it = cb.begin();
//    std::cout << '\n';
//    //        auto cb_2 = cb; // cb shouldn't be copyable, since it's in stack
//    //        it--; // forward iterator - reverse not possible
//    //        --it; // same as above
//  }

/* Iterator comparison. */
TEST(iterator, comparison) {
  CircularBuffer<char, 2> a;
  CircularBuffer<char, 2> b;

  auto a_it = a.begin();
  auto b_it = b.begin();
  EXPECT_FALSE(a_it == b_it);
  EXPECT_TRUE(a_it == a_it);
}

/* Validate OutputIterator category operations.
   * http://en.cppreference.com/w/cpp/concept/OutputIterator */
TEST(iterator, output_operations)
{
  CircularBuffer<char, 2> cb;
  cb.put('a'); cb.put('b');

  auto it = cb.begin();
  *it = 'a';
  *++it = 'b';
  EXPECT_EQ(*++it, 'a');
  EXPECT_EQ(*++it, 'b');
}

/* Test std::advance. */
TEST(STL, advance) {
  CircularBuffer<char, 3> cb;
  cb.put('a'); cb.put('b'); cb.put('c');
  auto it = cb.begin();
  std::advance(it, 2);
  EXPECT_EQ(*it, 'c');

}

/* Test std::fill. */
TEST(STL, fill) {
  CircularBuffer<char, 3> cb;
  std::string str;
  std::fill(std::begin(cb), std::end(cb), 'a');
  for(auto el : cb) {
    str += el;
  }
  EXPECT_EQ(str, "aaa");
}

TEST(general, initializer_list) {
  CircularBuffer<char, 3> cb = {'H','E','Y'};
  std::string str;
  for(auto el : cb) {
    str += el;
  }
  EXPECT_EQ(str, "HEY");
}

//  /* Test const iterators. */
//  std::cout << '\n';
//  {
//    CircularBuffer<int, 6> cb = {44, 24, 900, 23, 10, 2};
//    auto it = cb.cbegin();
//    it++;
////    *it = 2; // doesn't compile because const iterator
//    std::cout << *it << '\n';
//  }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
