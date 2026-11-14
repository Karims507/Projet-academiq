#pragma once
#include <optional>
#include <string>

// Calendar date in the format "YYYY-MM-DD".
class Date {
public:
    Date() = default;
    static std::optional<Date> parse(const std::string& text);  // strict validation (days/months/leap years)

    long long toDays() const;  // days since 1970-01-01
    std::string toString() const;

    bool operator<(const Date& o) const { return toDays() < o.toDays(); }
    bool operator==(const Date& o) const { return toDays() == o.toDays(); }

private:
    Date(int y, int m, int d) : y_(y), m_(m), d_(d) {}
    int y_ = 1970, m_ = 1, d_ = 1;
};

// Start time = number of minutes since midnight (0..1439), entered/displayed as "HH:MM".
namespace timeofday {
std::optional<int> parse(const std::string& text);
std::string toString(int minutesSinceMidnight);
}  // namespace timeofday
