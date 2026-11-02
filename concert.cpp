#include "concert.hpp"

#include <utility>

Concert::Concert(std::string title, int durationMinutes)
    : Event(std::move(title), durationMinutes) {}
