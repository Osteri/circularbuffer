#include <iostream>
#include <iterator>

/* Singly Linked List (SLL) data structure. Synonyms: link, node, etc.
 * Carries the actual data. */
template<typename T>
class SLL {
public:
  SLL<T>* forward;
  bool read = true;
  T data;

  /* iterator */
  class iterator
  {
  public:
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef SLL<T>& reference;
    typedef SLL<T>* pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef iterator self_type;

    iterator() {}
    iterator(pointer ptr) : raw{ptr} {}

    self_type operator++(int) { self_type it = *this; raw = raw->forward; return it; }
    self_type operator++() { raw = raw->forward; return *this; }
    value_type& operator*() { return raw->data; }
    pointer operator->() { return raw; }
    bool operator==(const self_type& rhs) { return raw == rhs.raw; } /* full or empty */
    bool operator!=(const self_type& rhs) { return raw != rhs.raw; }

  private:
    pointer raw;
  };

  /* const iterator. */
  class const_iterator
  {
  public:
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef SLL<T>& reference;
    typedef SLL<T>* pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef const_iterator self_type;

    const_iterator() {}
    const_iterator(pointer ptr) : raw{ptr} { }

    self_type operator++(int) { self_type it = *this; raw = raw->forward; return it; }
    self_type operator++() { raw = raw->forward; return *this; }
    const reference operator*() { return *raw; }
    const pointer operator->() { return raw; }
    bool operator==(const self_type& rhs) { return raw == rhs.raw; }
    bool operator!=(const self_type& rhs) { return raw != rhs.raw; }
  private:
    pointer raw;
  };
};

/* Actual implementation. */
template<typename T, const size_t N>
class CircularBuffer
{
public:
  CircularBuffer() : r{elements}, w{elements} { init_links(); }
  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer& operator=(const CircularBuffer&) = delete;

  void put(const T& data) noexcept { w->read = false; w->data = data; ++w; }
  T get() noexcept { r->read = true; return *r++; }

  typedef typename SLL<T>::iterator it_t;
  it_t begin() noexcept { return r; }
  it_t end() noexcept { return w; }

  bool has_unread_data() noexcept { return r->read == false; }
  bool empty() noexcept { return r == w && r->read == true; }
  bool full() noexcept { return r == w && r->read == false; }

  constexpr decltype(N) size() const noexcept { return N; }

#ifndef NDEBUG /* there is no std::cout in embedded systems. */
  friend std::ostream& operator<<(std::ostream& os, CircularBuffer& cb) {
    for (size_t i = 0; i < N; ++i)
      os << '[' << cb.get() << "]->";
    return os;
  }
#endif
private:
  SLL<T> elements[N];
  typename SLL<T>::iterator r;
  typename SLL<T>::iterator w;

  void init_links() noexcept {
    for (size_t i = 0, j = 1; i < N-1; i++, j++)
      elements[i].forward = &elements[j];

    elements[N-1].forward = elements;
  }
};
