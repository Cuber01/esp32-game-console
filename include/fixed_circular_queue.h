#ifndef ESP32_GAME_CONSOLE_FIXED_CIRCULAR_QUEUE_H
#define ESP32_GAME_CONSOLE_FIXED_CIRCULAR_QUEUE_H
#include <array>

template <typename T, std::size_t N>
class FixedCircularQueue {
private:
    std::array<T, N> data{};
    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t count = 0;

public:
    FixedCircularQueue() {
        data.fill(T{});
        head = 0;
        tail = 0;
        count = 0;
    }

    void PushHead(const T& value) {
        assert(count < N && "Snake overflow");
        head = (head + 1) % N;
        data[head] = value;
        count++;
    }

    void PopTail() noexcept {
        assert(count > 0 && "Snake underflow");
        tail = (tail + 1) % N;
        count--;
    }

    T& GetHead() noexcept {
        return data[head];
    }

    T& GetTail() noexcept {
        return data[tail];
    }

    size_t GetHeadIndex() {
        return head;
    }

    size_t GetTailIndex() {
        return tail;
    }

    T& operator[](std::size_t index) noexcept {
        assert(index < count && "Index out of bounds");
        return data[(tail + index) % N];
    }

    std::size_t Size() const noexcept {
        return count;
    }
};

#endif