#pragma once

#include "event.hpp"

class Concert : public Event {
public:
    Concert(std::string title = "", int durationMinutes = 0);
    ~Concert() override = default;

    virtual void play() const = 0;
};
