#ifndef FIXED_LIST_H
#define FIXED_LIST_H

#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <utility>

template <typename T, std::size_t N>
class FixedList {
private:
    std::array<T, N> data{};
    std::size_t count = 0;

public:
    // Aliases
    using iterator               = T*;
    using const_iterator         = const T*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr FixedList() noexcept = default;

    ~FixedList() {
        Clear();
    }

    // Disable copy/move or allow compiler defaults (default copy/move works if T is copyable/movable)
    FixedList(const FixedList&) = default;
    FixedList& operator=(const FixedList&) = default;
    FixedList(FixedList&&) noexcept = default;
    FixedList& operator=(FixedList&&) noexcept = default;

    // -------------------------------------------------------------------------
    // Iterators
    // -------------------------------------------------------------------------
    iterator begin() noexcept { return data.data(); }
    iterator end()   noexcept { return data.data() + count; }

    const_iterator begin()  const noexcept { return data.data(); }
    const_iterator end()    const noexcept { return data.data() + count; }
    const_iterator cbegin() const noexcept { return data.data(); }
    const_iterator cend()   const noexcept { return data.data() + count; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend()   noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()    const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(cbegin()); }

    // -------------------------------------------------------------------------
    // Core
    // -------------------------------------------------------------------------
    void Push(const T& value) {
        assert(!IsFull() && "FixedList overflow");
        data[count++] = value;
    }

    void Push(T&& value) {
        assert(!IsFull() && "FixedList overflow");
        data[count++] = std::move(value);
    }

    void Pop() noexcept {
        assert(!IsEmpty() && "FixedList underflow");
        count--;
        data[count].~T(); // Destroy popped element
    }

    T& operator[](std::size_t index) noexcept {
        assert(index < count && "FixedList index out of bounds");
        return data[index];
    }

    const T& operator[](std::size_t index) const noexcept {
        assert(index < count && "FixedList index out of bounds");
        return data[index];
    }

    void Clear() noexcept {
        while (count > 0) {
            Pop();
        }
    }

    std::size_t Size() const noexcept {
        return count;
    }

    constexpr std::size_t Capacity() const noexcept {
        return N;
    }

    bool IsEmpty() const noexcept {
        return count == 0;
    }

    bool IsFull() const noexcept {
        return count >= N;
    }
};

#endif // FIXED_LIST_H