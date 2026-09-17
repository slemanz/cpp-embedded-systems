#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <array>
#include <cstddef>

template <class T, std::size_t N> struct ring_buffer
{
    std::array<T, N> arr;
    std::size_t write_idx = 0;  // Index of the next element to write (push)
    std::size_t read_idx = 0;   // Index of the next element to read (pop)
    std::size_t count = 0;      // Number of elements in the buffer

    void push(T t)
    {
        arr.at(write_idx) = t;
        write_idx = (write_idx + 1) % N;
        if(count < N)
        {
            count++;
        }else
        {
            // buffer is full, move forward read_idx
            read_idx = (read_idx + 1) % N;
        }
    }

    T pop()
    {
        if(count == 0)
        {
            // buffer is empty, return a default constructed T.
            return T{};
        }

        T value = arr.at(read_idx);
        read_idx = (read_idx + 1) % N;
        --count;
        return value;
    }

    bool is_empty() const {return count == 0; }

    std::size_t get_count() const {return count; }
};

#endif /* RING_BUFFER_HPP */