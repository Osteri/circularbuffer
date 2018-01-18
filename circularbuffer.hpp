#include <iostream>

template<typename T>
struct SLL {
    SLL<T>* forward;
    T data;
    bool read = true;
};

template<typename T, const size_t N>
class CircularBuffer
{
public:
    CircularBuffer() noexcept : r_pos{elements}, w_pos{elements} { init_links(); }
    CircularBuffer(const CircularBuffer&) = delete;
    CircularBuffer& operator=(const CircularBuffer&) = delete;

    void put(T data) noexcept { w_pos->data = data; w_pos->read = false; w_pos = w_pos->forward; }
    T get() noexcept { auto tmp = r_pos->data; r_pos->read = true; r_pos = r_pos->forward; return tmp; }

    inline bool is_full() const noexcept { return w_pos->read == false; }
    inline bool has_unread_data() const noexcept { return r_pos->read == false; }
    inline decltype(N) size() const noexcept { return N; }

    /* clear() does not call destructors of the elements -- it only marks the elements
     * as being read; in other words: overwrite is now possible. The data still lies
     * there, until it is overwritten. */
    void clear() {
        for (int i = 0; i < N; ++i) {
            r_pos->read = true;
            r_pos = r_pos->forward;
        }
    }
#ifndef NDEBUG /* there is no std::cout in embedded systems. */
    friend std::ostream& operator<<(std::ostream& os, CircularBuffer& cb) {
        for (size_t i = 0; i < N; ++i)
            os << '[' << cb.get() << "]->";
        return os;
    }
#endif
private:
    SLL<T> elements[N];
    /* TODO: Why these are pointers? */
    SLL<T>* r_pos;
    SLL<T>* w_pos;

    void init_links() noexcept {
        for (size_t i = 0, j = 1; i < N-1; i++, j++)
            elements[i].forward = &elements[j];

        elements[N-1].forward = elements;
    }
};
