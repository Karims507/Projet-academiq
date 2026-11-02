#include "scheduleedEvent.hpp"

#include <string>
#include <utility>

ScheduledEvent::ScheduledEvent(std::string name, const Room& room, int durationMinutes)
    : name_(std::move(name)), room_(room), durationMinutes_(durationMinutes) {}

const std::string& ScheduledEvent::getName() const noexcept {
    return name_;
}

const Room& ScheduledEvent::getRoom() const noexcept {
    return room_;
}

int ScheduledEvent::getDurationMinutes() const noexcept {
    return durationMinutes_;
}

std::string ScheduledEvent::summary() const {
    return name_ + " in " + room_.getName() + " (" + std::to_string(durationMinutes_) + " min)";
}
