#include "Play.hpp"

#include <stdexcept>

#include "Util.hpp"

namespace {

[[maybe_unused]] const bool registered = EventRegistry::instance().add(
    {"PLAY", "Piece de theatre", {"Duree de l'entracte en minutes (1 a 120)"},
     [](const std::string& title, int duration, double price,
        const std::vector<std::string>& f) -> std::unique_ptr<Event> {
         const auto minutes = util::parseInt(f[0]);
         if (!minutes) throw std::invalid_argument("duree de l'entracte invalide : '" + f[0] + "' (entier attendu)");
         if (*minutes < 1 || *minutes > 120)
             throw std::invalid_argument("duree de l'entracte hors limites (1 a 120) : " + f[0]);
         return std::make_unique<Play>(title, duration, price, *minutes);
     }});

}  // namespace
