#include <iostream>
#include <type_traits>
#include <iterator>

/* Singly Linked List (SLL) data structure. Synonyms: link or node,
 * which also carries the actual data. */
template<typename T>
class SLL {
public:
    SLL<T>* forward;
    bool read = true;
    T data;

    /* Helper classes for iterating through CircularBuffer.
     * CircularBuffer is a container-alike type; linked in-between with
     * SLL's. Raw pointers offer only random access, which could seg fault.
     * With own iterator class, we can abstract raw memory access away.
     * This *should* be compatible with C++ standard 'ForwardIterator'. */
    class cb_it
    {
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef SLL<T>& reference;
        typedef SLL<T>* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef cb_it self_type;

        cb_it() {}
        cb_it(pointer ptr) : raw{ptr} { }
        self_type operator++() { self_type it = *this; raw = raw->forward; return it; }
        self_type operator++(int) { ++raw; return *this; }
        reference operator*() { return *raw; }
        reference operator=(const value_type& d) { raw->data = d; return *raw; } //
        reference operator=(reference rhs) { data = rhs.data; return *raw; } //
        pointer operator->() { return raw; }
        bool operator==(const self_type& rhs) { return raw == rhs.raw; }
        bool operator!=(const self_type& rhs) { return raw != rhs.raw; }
    private:
        pointer raw;
    };

    /* CircularBuffer const iterator. */
    class const_cb_it
    {
    public:
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef SLL<T>& reference;
        typedef SLL<T>* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef const_cb_it self_type;

        const_cb_it() {}
        const_cb_it(pointer ptr) : raw{ptr} { }
        self_type operator++() { self_type it = *this; raw = raw->forward; return it; }
        self_type operator++(int) { ++raw; return *this; }
        const reference operator*() { return *raw; }
        const reference operator=(const value_type& d) { raw->data = d; return *raw; } //
        const reference operator=(const reference rhs) { data = rhs.data; return *raw; } //
        const pointer operator->() { return raw; }
        bool operator==(const self_type& rhs) { return raw == rhs.raw; }
        bool operator!=(const self_type& rhs) { return raw != rhs.raw; }
    private:
        pointer raw;
    };
};

template<typename T>
struct CB_it {
    CB_it (SLL<T>* sll) : r{sll}, w{sll} {}

    CB_it (const CB_it&) = default;
    CB_it (CB_it&&) = default;

    CB_it (const T& d) { w->data = d; }

    /* Iterator traits for std algorithms. */
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::forward_iterator_tag iterator_category;
    //typedef CB_it<value_type> iterator;

    CB_it& operator ++() { /* ++pre */
        r = r->forward;
        return *this;
    }
    CB_it operator ++(int) { /* post++ */
        CB_it it(r);
        r = r->forward;
        return it;
    }
    CB_it& operator =(const T& data) { /* data assign */
        w->read = false;
        w->data = data;
        w = w->forward;
    }
    CB_it& operator =(const CB_it& other) { /* iterator assign */
        return other;
    }
    T& operator *() { /* de-reference */
        r->read = true;
        return r->data;
    }
    CB_it* operator ->() { /* ... */
        return this;
    }
    inline bool operator ==(const CB_it& rhs) const noexcept {
        return (this->r == rhs.r && this->w == rhs.w);
    }
    inline bool operator !=(const CB_it& rhs) const noexcept {
        return (this->r != rhs.r && this->w != rhs.w);
    }
    //    inline bool is_full() const noexcept { return w->read == false; }
    //    inline bool has_unread_data() const noexcept { return r->read == true; }

#ifndef NDEBUG /* there is no std::cout in embedded systems. */
    friend std::ostream& operator<<(std::ostream& os, CB_it& it) {
        os << &it;
        return os;
    }
#endif

private: /* read and write pointers are separated in CB */
    SLL<T>* r;
    SLL<T>* w;
};

/* Actual implementation. */
template<typename T, const size_t N>
class CircularBuffer
{
public:
    CircularBuffer() : r{elements}, w{&elements[N-1]} { init_links(); }
    CircularBuffer(const CircularBuffer&) = delete;
    CircularBuffer& operator=(const CircularBuffer&) = delete;

    /* Use these accessors if you want to keep track of when the buffer comes full.
     * Poll with is_full() and has_unread_data(). */
    void put(const T& data) noexcept { w->read = false; ++w = data; }
    T get() noexcept { r->read = true; ++r; return r->data; }
//    inline bool is_full() const noexcept { return *w->read == true; }
//    inline bool has_unread_data() const noexcept { return *r->read == false; }

    inline decltype(N) size() const noexcept { return N; }

    /* Basically circular buffer has no begin/end, but we can cheat a little, and pass
     * the iterators, since we know the memory locations. If you use these, CB loses
     * track of r/w pointers, unless you manually control the flags in the iteration
     * loop like the methods put() and get() do. By doing this, you lose ForwardIterator
     * benefits vs. RandomAccessIterator. */
    constexpr SLL<T>* begin() { return elements; }
    constexpr SLL<T>* end() { return &elements[N]; }
    constexpr const SLL<T>* cbegin() const { return elements; }
    constexpr const SLL<T>* cend() const { return &elements[N]; }

    /* clear() does not call destructors of the elements -- it only marks the elements
     * as being read; in other words: overwrite is now possible. The data still lies
     * there, until it is overwritten. */
    //    void clear() {
    //        for (int i = 0; i < N; i++) {
    //            ++it;
    //        }
    //    }
#ifndef NDEBUG /* there is no std::cout in embedded systems. */
    friend std::ostream& operator<<(std::ostream& os, CircularBuffer& cb) {
        for (size_t i = 0; i < N; ++i)
            os << '[' << cb.get() << "]->";
        return os;
    }
#endif
private:
    SLL<T> elements[N];
    typename SLL<T>::cb_it r;
    typename SLL<T>::cb_it w;

    void init_links() noexcept {
        for (size_t i = 0, j = 1; i < N-1; i++, j++)
            elements[i].forward = &elements[j];

        elements[N-1].forward = elements;
    }
};
