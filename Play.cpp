#include "Play.hpp"
#include <iostream>

Play::Play(std::string title,
           int durationMinutes,
           double price,
           bool hasInterval)
    : Event(title, durationMinutes, price),
      hasInterval_(hasInterval)
{
}

void Play::display() const
{
    std::cout << "Play: " << title_ << "\n";
    std::cout << "Duration: " << durationMinutes_ << " minutes\n";
    std::cout << "Price: " << price_ << "\n";

    if (hasInterval_)
    {
        std::cout << "Interval: yes\n";
    }
    else
    {
        std::cout << "Interval: no\n";
    }
}