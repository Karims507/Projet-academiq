#include "Screening.hpp"

#include <stdexcept>

#include "Util.hpp"

namespace {

[[maybe_unused]] const bool registered = EventRegistry::instance().add(
    {"SCREENING", "Screening", {"Minimum audience age (0 = all ages, otherwise 1 to 18)"},
     [](const std::string& title, int duration, double price,
        const std::vector<std::string>& f) -> std::unique_ptr<Event> {
         const auto age = util::parseInt(f[0]);
         if (!age) throw std::invalid_argument("invalid minimum age: '" + f[0] + "' (integer expected)");
         if (*age < 0 || *age > 18) throw std::invalid_argument("minimum age out of range (0 to 18): " + f[0]);
         return std::make_unique<Screening>(title, duration, price, *age);
     }});

}  // namespace
