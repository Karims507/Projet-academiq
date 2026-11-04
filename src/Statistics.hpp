#pragma once
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <optional>
#include <vector>

// Outil de calcul unique, réutilisé pour les prix (double), les fréquentations et les durées (int).
template <typename T>
struct Summary {
    std::size_t count;
    T min;
    T max;
    double mean;  // toujours décimale, même pour des entiers
};

// std::nullopt si aucune valeur n'est disponible.
template <typename T>
std::optional<Summary<T>> summarize(const std::vector<T>& values) {
    if (values.empty()) return std::nullopt;
    const auto [lo, hi] = std::minmax_element(values.begin(), values.end());
    const double sum = std::accumulate(values.begin(), values.end(), 0.0,
                                       [](double acc, const T& v) { return acc + static_cast<double>(v); });
    return Summary<T>{values.size(), *lo, *hi, sum / static_cast<double>(values.size())};
}
