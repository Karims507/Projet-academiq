#include "Screening.hpp"
#include <iostream>

Screening::Screening(std::string title,
                     int durationMinutes,
                     double price,
                     int minimumAge)
    : Event(title, durationMinutes, price),
      minimumAge_(minimumAge)
{
}

void Screening::display() const
{
    std::cout << "Screening: " << title_ << "\n";
    std::cout << "Duration: " << durationMinutes_ << " minutes\n";
    std::cout << "Price: " << price_ << "\n";
    std::cout << "Minimum age: " << minimumAge_ << "\n";
}