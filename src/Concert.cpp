#include "Concert.hpp"

#include <stdexcept>

#include "Util.hpp"

namespace {

std::string join(const std::vector<std::string>& items, const std::string& sep) {
    std::string out;
    for (const auto& i : items) out += (out.empty() ? "" : sep) + i;
    return out;
}

[[maybe_unused]] const bool registered = EventRegistry::instance().add(
    {"CONCERT", "Concert",
     {"Artistes (separes par des ';')", "Duree des balances en minutes (0 à 240)"},
     [](const std::string& title, int duration, double price,
        const std::vector<std::string>& f) -> std::unique_ptr<Event> {
         std::vector<std::string> artists;
         for (const auto& a : util::split(f[0], ';')) {
             const std::string name = util::trim(a);
             if (name.empty()) throw std::invalid_argument("liste d'artistes invalide (nom vide) : '" + f[0] + "'");
             artists.push_back(name);
         }
         const auto minutes = util::parseInt(f[1]);
         if (!minutes) throw std::invalid_argument("duree des balances invalide : '" + f[1] + "' (entier attendu)");
         if (*minutes < 0 || *minutes > 240)
             throw std::invalid_argument("duree des balances hors limites (0 à 240) : " + f[1]);
         return std::make_unique<Concert>(title, duration, price, artists, *minutes);
     }});

}  // namespace

std::string Concert::details() const {
    return "Artistes : " + join(artists_, ", ") + " | balances : " + std::to_string(soundcheckMinutes_) + " min";
}

std::vector<std::string> Concert::extraFields() const {
    return {join(artists_, ";"), std::to_string(soundcheckMinutes_)};
}
