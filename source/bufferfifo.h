#pragma once

#include <array>
#include <atomic>
#include <vector>

template<typename T>
struct BufferFifo
{
    void prepare(std::size_t bufferCapacity)
    {
        for (auto& buffer : buffers)
        {
            buffer.clear();
            buffer.resize(bufferCapacity, 0.0f);
        }
    }

    bool tryPush(const std::vector<T>& buffer)
    {
        const size_t write = writeIndex.load(std::memory_order_relaxed);
        const size_t next = increment(write);

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
        const size_t read = readIndex.load(std::memory_order_relaxed);

        if (read == writeIndex.load(std::memory_order_acquire))
        {
            return false; // empty
        }

        buffer = buffers[read];
        readIndex.store(increment(read), std::memory_order_release);
        return true;
    }

    size_t getAvailableReads() const
    {
        const size_t read = readIndex.load(std::memory_order_acquire);
        const size_t write = writeIndex.load(std::memory_order_acquire);

        return write >= read
            ? write - read
            : Capacity - read + write;
    }

private:
    static constexpr size_t Capacity = 32;
    std::array<std::vector<T>, Capacity> buffers;
    std::atomic<size_t> readIndex { 0 };
    std::atomic<size_t> writeIndex { 0 };

    static constexpr size_t increment(size_t index) noexcept
    {
        return (index + 1) % Capacity;
    }
};
