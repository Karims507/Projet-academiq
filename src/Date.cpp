#include "Date.hpp"

#include <cstdio>

namespace {

bool isLeap(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

int daysInMonth(int y, int m) {
    static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (m == 2 && isLeap(y)) ? 29 : d[m - 1];
}

// Algorithme de H. Hinnant (calendrier gregorien proleptique).
long long daysFromCivil(int y, int m, int d) {
    y -= m <= 2;
    const long long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = static_cast<unsigned>((153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1);
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

int digits(const std::string& s, std::size_t pos, std::size_t len) {
    int v = 0;
    for (std::size_t i = pos; i < pos + len; ++i) {
        if (s[i] < '0' || s[i] > '9') return -1;
        v = v * 10 + (s[i] - '0');
    }
    return v;
}

}  // namespace

std::optional<Date> Date::parse(const std::string& t) {
    if (t.size() != 10 || t[4] != '-' || t[7] != '-') return std::nullopt;
    const int y = digits(t, 0, 4), m = digits(t, 5, 2), d = digits(t, 8, 2);
    if (y < 1970 || m < 1 || m > 12 || d < 1) return std::nullopt;
    if (d > daysInMonth(y, m)) return std::nullopt;
    return Date(y, m, d);
}

long long Date::toDays() const { return daysFromCivil(y_, m_, d_); }

std::string Date::toString() const {
    char buf[16];
    std::snprintf(buf, sizeof buf, "%04d-%02d-%02d", y_, m_, d_);
    return buf;
}

namespace timeofday {

std::optional<int> parse(const std::string& t) {
    if (t.size() != 5 || t[2] != ':') return std::nullopt;
    const int h = digits(t, 0, 2), m = digits(t, 3, 2);
    if (h < 0 || h > 23 || m < 0 || m > 59) return std::nullopt;
    return h * 60 + m;
}

std::string toString(int minutes) {
    char buf[16];
    std::snprintf(buf, sizeof buf, "%02d:%02d", minutes / 60, minutes % 60);
    return buf;
}

}  // namespace timeofday
