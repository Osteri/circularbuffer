#ifndef NDEBUG
  #include <iostream>
#endif
#include <utility>
#include <cassert>

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

    iterator() { }
    iterator(pointer ptr) : raw{ptr} { }

    self_type operator++(int) { cnt++; self_type it = *this; raw = raw->forward; return it; }
    self_type operator++() { cnt++; raw = raw->forward; return *this; }
    value_type& operator*() { return raw->data; }
    pointer operator->() { return raw; }
    // iterators are equal if their counters (size_t) are same and they are same instance (iterator) OR they point to same node (SLL)
    bool operator==(const self_type& rhs) { return (cnt == rhs.cnt) && ((this == &rhs) || (raw == rhs.raw)); }
    bool operator!=(const self_type& rhs) { return (cnt != rhs.cnt) && ((this != &rhs) || (raw != rhs.raw)); }

  private:
    pointer raw;
    size_t cnt = 0; // Cyclic iterator must have internal state.
                    // Otherwise it's impossible to use range based
                    // for loops in C++11 and beyond.
    template<typename U, const size_t N>
    friend class CircularBuffer; // access 'raw' and 'cnt' from CircularBuffer
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
    const_iterator(pointer ptr) : raw{ptr} {}

    self_type operator++(int) { cnt++; self_type it = *this; raw = raw->forward; return it; }
    self_type operator++() { cnt++; raw = raw->forward; return *this; }
    const value_type operator*() { return raw->data; }
    pointer operator->() { return raw; }
    // iterators are equal if their counters (size_t) are same and they are same instance (iterator) OR they point to same node (SLL)
    bool operator==(const self_type& rhs) { return raw == rhs.raw; }
    bool operator!=(const self_type& rhs) { return raw != rhs.raw; }
  private:
    pointer raw;
    size_t cnt = 0; // Cyclic iterator must have internal state.
                    // Otherwise it's impossible to use range based
                    // for loops in C++11 and beyond.
    template<typename U, const size_t N>
    friend class CircularBuffer; // access 'raw' and 'cnt' from CircularBuffer
  };
};

/* Actual implementation. */
template<typename T, const size_t N>
class CircularBuffer
{
public:
  CircularBuffer(std::initializer_list<T> il) { // ilist ctor
    init_links();
    assert(il.size() == N);
    std::copy(std::begin(il), std::end(il), std::begin(*this));
  }
  CircularBuffer() { init_links(); } // def ctor
  CircularBuffer(CircularBuffer&&) = delete;                    // mov ctor
  CircularBuffer& operator=(const CircularBuffer&) = delete;    // cpy assign ctor

  void put(const T& data) noexcept { w->read = false; w->data = data; ++w; }
  void put(T&& data) noexcept { w->read = false; w->data = std::move(data); ++w; }
  T get() noexcept { r->read = true; return *r++; }

  typedef typename::SLL<T>::iterator it;
  it begin() noexcept { r.cnt = 0; return r; }
  it end() noexcept { w.cnt = N; return w; }

  typedef typename::SLL<T>::const_iterator c_it;
  c_it cbegin() noexcept { r.cnt = 0; return c_it(r.raw); }
  c_it cend() noexcept { w.cnt = N; return c_it(w.raw); }

  bool has_unread_data() noexcept { return r->read == false; }
  bool empty() noexcept { return r.raw == w.raw && r->read == true; }
  bool full() noexcept { return r.raw == w.raw && r->read == false; }

  constexpr decltype(N) capacity() const noexcept { return N; }

#ifndef NDEBUG /* there is no std::cout in embedded systems. */
  friend std::ostream& operator<<(std::ostream& os, CircularBuffer& cb) {
    for (size_t i = 0; i < N; ++i)
      os << '[' << cb.get() << "]->";
    return os;
  }
#endif
private:
  SLL<T> elements[N];
  typename SLL<T>::iterator r{elements};
  typename SLL<T>::iterator w{elements};

  void init_links() noexcept {
    for (size_t i = 0, j = 1; i < N-1; i++, j++) {
      elements[i].forward = &elements[j];
    }
    elements[N-1].forward = elements; // tail points to head
  }
};
