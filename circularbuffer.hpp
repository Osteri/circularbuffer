#include <iostream>
#include <type_traits>
#include <iterator>

template<typename T>
struct SLL {
public:
    SLL<T>* forward;
    bool read = true;
    T data;
};

template<typename T>
struct CB_it {
    CB_it (SLL<T>* sll) : r{sll}, w{sll} {}
    CB_it (CB_it&) = default;
    CB_it (CB_it&&) = default;
    CB_it (const CB_it&) = default;

    CB_it (T d) { w->data = d; }

    /* Iterator traits for std algorithms. */
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::forward_iterator_tag iterator_category;

    CB_it operator ++() { /* ++pre */
        //r->read = false;
        r = r->forward;
        return *this;
    }
    CB_it operator ++(int) { /* post++ */
        CB_it it(r);
        //r->read = false;
        r = r->forward;
        return it;
    }
    CB_it operator =(T data) { /* add data */
        w->read = false;
        w->data = data;
        w = w->forward;
    }
    inline T& operator *() { /* de-reference */
        r->read = true;
        return r->data;
    }
    inline bool operator ==(CB_it& rhs) {
        return this == &rhs;
    }
    inline bool operator !=(CB_it& rhs) {
        return this != &rhs;
    }
    inline bool is_full() const noexcept { return w->read == false; }
    inline bool has_unread_data() const noexcept { return r->read == true; }

private: /* read and write pointers are separated in CB */
    SLL<T>* r;
    SLL<T>* w;
};

template<typename T, const size_t N>
class CircularBuffer
{
public:
    CircularBuffer() noexcept : it{elements} { init_links(); }
    CircularBuffer(const CircularBuffer&) = delete;
    CircularBuffer& operator=(const CircularBuffer&) = delete;

    inline CB_it<T> begin() { return it; }
    inline CB_it<T> end() { return begin(); }

    void put(T data) noexcept { it = data; /* InputIterator */ }
    T get() noexcept { return *it++; } /* OutputIterator*/

    inline bool is_full() const noexcept { return it.is_full(); }
    inline bool has_unread_data() const noexcept { return it.has_unread_data(); }
    inline decltype(N) size() const noexcept { return N; }

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
    CB_it<T> it;

    void init_links() noexcept {
        for (size_t i = 0, j = 1; i < N-1; i++, j++)
            elements[i].forward = &elements[j];

        elements[N-1].forward = elements;
    }
};
