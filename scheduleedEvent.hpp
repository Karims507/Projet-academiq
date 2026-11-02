#pragma once

#include <string>

#include "room.hpp"

class ScheduledEvent {
public:
    ScheduledEvent(std::string name = "", const Room& room = Room(), int durationMinutes = 0);

    const std::string& getName() const noexcept;
    const Room& getRoom() const noexcept;
    int getDurationMinutes() const noexcept;
    std::string summary() const;

private:
    std::string name_;
    Room room_;
    int durationMinutes_;
};
