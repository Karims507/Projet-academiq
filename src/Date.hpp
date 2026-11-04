#pragma once
#include <optional>
#include <string>

// Date du calendrier au format "AAAA-MM-JJ".
class Date {
public:
    Date() = default;
    static std::optional<Date> parse(const std::string& text);  // validation stricte (jours/mois/bissextile)

    long long toDays() const;  // jours depuis 1970-01-01
    std::string toString() const;

    bool operator<(const Date& o) const { return toDays() < o.toDays(); }
    bool operator==(const Date& o) const { return toDays() == o.toDays(); }

private:
    Date(int y, int m, int d) : y_(y), m_(m), d_(d) {}
    int y_ = 1970, m_ = 1, d_ = 1;
};

// Heure de début = nombre de minutes depuis minuit (0..1439), saisie/affichée en "HH:MM".
namespace timeofday {
std::optional<int> parse(const std::string& text);
std::string toString(int minutesSinceMidnight);
}  // namespace timeofday
