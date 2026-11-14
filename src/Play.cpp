#include "Play.hpp"

#include <stdexcept>

#include "Util.hpp"

namespace {

[[maybe_unused]] const bool registered = EventRegistry::instance().add(
    {"PLAY", "Play", {"Intermission duration in minutes (1 to 120)"},
     [](const std::string& title, int duration, double price,
        const std::vector<std::string>& f) -> std::unique_ptr<Event> {
         const auto minutes = util::parseInt(f[0]);
         if (!minutes) throw std::invalid_argument("invalid intermission duration: '" + f[0] + "' (integer expected)");
         if (*minutes < 1 || *minutes > 120)
             throw std::invalid_argument("intermission duration out of range (1 to 120): " + f[0]);
         return std::make_unique<Play>(title, duration, price, *minutes);
     }});

}  // namespace
