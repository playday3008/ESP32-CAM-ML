#pragma once

#include <cstddef>
#include <cstdint>

#include <array>

constexpr size_t RA_FILTER_DEFAULT_SIZE = 20;

template <typename T = int32_t, size_t N = RA_FILTER_DEFAULT_SIZE>
class RaFilter {
    size_t           index = 0;
    size_t           count = 0;
    T                sum   = 0;
    std::array<T, N> values{};

  public:
    T operator()(const T value) {
        this->sum                -= this->values[this->index];
        this->values[this->index] = value;
        this->sum                += this->values[this->index];
        this->index++;
        this->index = this->index % values.size();
        if (this->count < values.size()) {
            this->count++;
        }
        return this->sum / this->count;
    }
};
