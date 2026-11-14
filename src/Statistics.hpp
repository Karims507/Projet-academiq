#pragma once
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <optional>
#include <vector>

// Shared calculation helper, reused for prices (double), attendance and durations (int).
template <typename T>
struct Summary {
    std::size_t count;
    T min;
    T max;
    double mean;  // always decimal, even for integers
};

// std::nullopt if no value is available.
template <typename T>
std::optional<Summary<T>> summarize(const std::vector<T>& values) {
    if (values.empty()) return std::nullopt;
    const auto [lo, hi] = std::minmax_element(values.begin(), values.end());
    const double sum = std::accumulate(values.begin(), values.end(), 0.0,
                                       [](double acc, const T& v) { return acc + static_cast<double>(v); });
    return Summary<T>{values.size(), *lo, *hi, sum / static_cast<double>(values.size())};
}
