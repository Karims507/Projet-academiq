#pragma once

#include "Event.hpp"
#include <vector>
#include <string>

class Concert : public Event
{
private:
    std::vector<std::string> artists_;

public:
    Concert(std::string title,
            int durationMinutes,
            double price,
            std::vector<std::string> artists);

    void display() const override;
};