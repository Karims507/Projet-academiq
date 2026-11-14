#pragma once
#include <optional>
#include <string>
#include <vector>

namespace util {
std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char sep);
std::string toLower(std::string s);                    // ASCII only
std::optional<int> parseInt(const std::string& s);     // rejects "12abc", "", out-of-range values
std::optional<double> parseDouble(const std::string& s);
std::string formatPrice(double price);                 // "8.50 EUR"
}  // namespace util
