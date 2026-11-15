#pragma once

#include "Event.hpp"

class Screening : public Event
{
private:
    int minimumAge_;

public:
    Screening(std::string title,
              int durationMinutes,
              double price,
              int minimumAge);

    void display() const override;
};