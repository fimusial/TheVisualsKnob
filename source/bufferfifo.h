#pragma once

#include <array>
#include <atomic>
#include <vector>

template<typename T>
struct BufferFifo
{
    bool tryPush(const std::vector<T>& buffer)
    {
        const int write = writeIndex.load(std::memory_order_relaxed);
        const int next = increment(write);

        if (next == readIndex.load(std::memory_order_acquire))
        {
            return false; // full
        }

        buffers[write] = buffer;
        writeIndex.store(next, std::memory_order_release);
        return true;
    }

    bool tryPull(std::vector<T>& buffer)
    {
        const int read = readIndex.load(std::memory_order_relaxed);

        if (read == writeIndex.load(std::memory_order_acquire))
        {
            return false; // empty
        }

        buffer = buffers[read];
        readIndex.store(increment(read), std::memory_order_release);
        return true;
    }

    int getAvailablePullCount() const
    {
        const int read = readIndex.load(std::memory_order_acquire);
        const int write = writeIndex.load(std::memory_order_acquire);

        return write >= read
            ? write - read
            : Capacity - read + write;
    }

private:
    static constexpr int Capacity = 32;
    std::array<std::vector<T>, Capacity> buffers;
    std::atomic<int> readIndex { 0 };
    std::atomic<int> writeIndex { 0 };

    static constexpr int increment(int index) noexcept
    {
        return (index + 1) % Capacity;
    }
};
