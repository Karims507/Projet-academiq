#include "Screening.hpp"

#include <stdexcept>

#include "Util.hpp"

namespace {

[[maybe_unused]] const bool registered = EventRegistry::instance().add(
    {"SCREENING", "Projection", {"Âge minimum du public (0 = tout public, sinon 1 a 18)"},
     [](const std::string& title, int duration, double price,
        const std::vector<std::string>& f) -> std::unique_ptr<Event> {
         const auto age = util::parseInt(f[0]);
         if (!age) throw std::invalid_argument("âge minimum invalide : '" + f[0] + "' (entier attendu)");
         if (*age < 0 || *age > 18) throw std::invalid_argument("âge minimum hors limites (0 a 18) : " + f[0]);
         return std::make_unique<Screening>(title, duration, price, *age);
     }});

}  // namespace
