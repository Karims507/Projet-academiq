#include "Workshop.hpp"

#include <algorithm>
#include <stdexcept>

#include "Util.hpp"

namespace {

// Le type s'enregistre tout seul au demarrage : ni la saisie, ni le fichier, ni l'affichage ne changent.
[[maybe_unused]] const bool registered = EventRegistry::instance().add(
    {"WORKSHOP", "Atelier", {"Intervenant", "Niveau (DEBUTANT, INTERMEDIAIRE, AVANCE)"},
     [](const std::string& title, int duration, double price,
        const std::vector<std::string>& f) -> std::unique_ptr<Event> {
         static const std::vector<std::string> levels = {"DEBUTANT", "INTERMEDIAIRE", "AVANCE"};
         const std::string who = util::trim(f[0]);
         const std::string level = util::trim(f[1]);
         if (who.empty()) throw std::invalid_argument("l'intervenant ne peut pas être vide");
         if (std::find(levels.begin(), levels.end(), level) == levels.end())
             throw std::invalid_argument("niveau inconnu '" + level + "' (DEBUTANT, INTERMEDIAIRE, AVANCE)");
         return std::make_unique<Workshop>(title, duration, price, who, level);
     }});

}  // namespace
