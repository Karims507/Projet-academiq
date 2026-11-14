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
        throw std::invalid_argument(what + " : the characters '|' and newlines are forbidden");
}
}  // namespace

std::unique_ptr<Event> EventRegistry::create(const std::string& key, const std::string& title, int durationMinutes,
                                             double price, const std::vector<std::string>& fields) const {
    const auto it = types_.find(key);
    if (it == types_.end()) {
        std::string known;
        for (const auto& kv : types_) known += (known.empty() ? "" : ", ") + kv.first;
        throw std::invalid_argument("unknown event type '" + key + "' (known types: " + known + ")");
    }
    const EventType& type = it->second;

    const std::string cleanTitle = util::trim(title);
    if (cleanTitle.empty()) throw std::invalid_argument("title cannot be empty");
    checkText(cleanTitle, "title");
    if (durationMinutes < 1 || durationMinutes > 1440)
        throw std::invalid_argument("duration must be between 1 and 1440 minutes");
    if (!(price >= 0.0)) throw std::invalid_argument("price must be positive or zero");
    if (fields.size() != type.fieldPrompts.size())
        throw std::invalid_argument("type " + key + " expects " + std::to_string(type.fieldPrompts.size()) +
                                    " specific field(s), but " + std::to_string(fields.size()) + " was/were received");
    for (const auto& f : fields) checkText(f, "specific field");

    const double cents = std::round(price * 100.0) / 100.0;  // keep cent precision
    return type.create(cleanTitle, durationMinutes, cents, fields);
}
