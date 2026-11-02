#include "event.hpp"

#include <string>
#include <utility>

Event::Event(std::string title, int durationMinutes)
    : title_(std::move(title)), durationMinutes_(durationMinutes) {}

const std::string& Event::getTitle() const noexcept {
    return title_;
}

int Event::getDurationMinutes() const noexcept {
    return durationMinutes_;
}

std::string Event::description() const {
    return title_ + " (" + std::to_string(durationMinutes_) + " min)";
}
