#include "Util.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace util {

std::string trim(const std::string& s) {
    std::size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == sep) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    out.push_back(cur);
    return out;
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

std::optional<int> parseInt(const std::string& text) {
    const std::string s = trim(text);
    if (s.empty()) return std::nullopt;
    errno = 0;
    char* end = nullptr;
    const long v = std::strtol(s.c_str(), &end, 10);
    if (errno != 0 || *end != '\0' || v < INT_MIN || v > INT_MAX) return std::nullopt;
    return static_cast<int>(v);
}

std::optional<double> parseDouble(const std::string& text) {
    std::string s = trim(text);
    if (s.empty()) return std::nullopt;
    std::replace(s.begin(), s.end(), ',', '.');  // "8,5" accepte
    errno = 0;
    char* end = nullptr;
    const double v = std::strtod(s.c_str(), &end);
    if (errno != 0 || *end != '\0' || !std::isfinite(v)) return std::nullopt;
    return v;
}

std::string formatPrice(double price) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(2) << price << " EUR";
    return os.str();
}

}  // namespace util
