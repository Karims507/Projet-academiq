#pragma once

#include "Event.hpp"

class Play : public Event
{
private:
    bool hasInterval_;

public:
    Play(std::string title,
         int durationMinutes,
         double price,
         bool hasInterval);

    void display() const override;
};