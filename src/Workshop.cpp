#include "Workshop.hpp"

#include <algorithm>
#include <stdexcept>

#include "Util.hpp"

namespace {

// The type registers itself at startup: neither the input, file format, nor the display change.
[[maybe_unused]] const bool registered = EventRegistry::instance().add(
    {"WORKSHOP", "Workshop", {"Instructor", "Level (BEGINNER, INTERMEDIATE, ADVANCED)"},
     [](const std::string& title, int duration, double price,
        const std::vector<std::string>& f) -> std::unique_ptr<Event> {
         static const std::vector<std::string> levels = {"BEGINNER", "INTERMEDIATE", "ADVANCED"};
         const std::string who = util::trim(f[0]);
         const std::string level = util::trim(f[1]);
         if (who.empty()) throw std::invalid_argument("instructor cannot be empty");
         if (std::find(levels.begin(), levels.end(), level) == levels.end())
             throw std::invalid_argument("unknown level '" + level + "' (BEGINNER, INTERMEDIATE, ADVANCED)");
         return std::make_unique<Workshop>(title, duration, price, who, level);
     }});

}  // namespace
