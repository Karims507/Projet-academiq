#include "Concert.hpp"
#include <iostream>

Concert::Concert(std::string title,
                 int durationMinutes,
                 double price,
                 std::vector<std::string> artists)
    : Event(title, durationMinutes, price),
      artists_(artists)
{
}

void Concert::display() const
{
    std::cout << "Concert: " << title_ << "\n";
    std::cout << "Duration: " << durationMinutes_ << " minutes\n";
    std::cout << "Price: " << price_ << "\n";

    std::cout << "Artists: ";

    for (const auto& artist : artists_)
    {
        std::cout << artist << " ";
    }

    std::cout << "\n";
}