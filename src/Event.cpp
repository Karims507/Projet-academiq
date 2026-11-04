#include "Event.hpp"

#include <cmath>
#include <stdexcept>

#include "Util.hpp"

EventRegistry& EventRegistry::instance() {
    static EventRegistry registry;
    return registry;
}

bool EventRegistry::add(EventType type) {
    const std::string key = type.key;
    types_.emplace(key, std::move(type));
    return true;
}

namespace {
void checkText(const std::string& text, const std::string& what) {
    if (text.find('|') != std::string::npos || text.find('\n') != std::string::npos ||
        text.find('\r') != std::string::npos)
        throw std::invalid_argument(what + " : les caracteres '|' et les retours à la ligne sont interdits");
}
}  // namespace

std::unique_ptr<Event> EventRegistry::create(const std::string& key, const std::string& title, int durationMinutes,
                                             double price, const std::vector<std::string>& fields) const {
    const auto it = types_.find(key);
    if (it == types_.end()) {
        std::string known;
        for (const auto& kv : types_) known += (known.empty() ? "" : ", ") + kv.first;
        throw std::invalid_argument("type d'evenement inconnu '" + key + "' (types connus : " + known + ")");
    }
    const EventType& type = it->second;

    const std::string cleanTitle = util::trim(title);
    if (cleanTitle.empty()) throw std::invalid_argument("le titre ne peut pas etre vide");
    checkText(cleanTitle, "titre");
    if (durationMinutes < 1 || durationMinutes > 1440)
        throw std::invalid_argument("la duree doit etre comprise entre 1 et 1440 minutes");
    if (!(price >= 0.0)) throw std::invalid_argument("le prix doit etre positif ou nul");
    if (fields.size() != type.fieldPrompts.size())
        throw std::invalid_argument("le type " + key + " attend " + std::to_string(type.fieldPrompts.size()) +
                                    " champ(s) specifique(s), " + std::to_string(fields.size()) + " reçu(s)");
    for (const auto& f : fields) checkText(f, "champ spécifique");

    const double cents = std::round(price * 100.0) / 100.0;  // on conserve les centimes
    return type.create(cleanTitle, durationMinutes, cents, fields);
}
