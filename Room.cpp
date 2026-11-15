#include "Room.hpp"
#include <iostream>

Room::Room(std::string name, int capacity)
    : name_(name),
      capacity_(capacity)
{
}

std::string Room::getName() const
{
    return name_;
}

int Room::getCapacity() const
{
    return capacity_;
}

void Room::display() const
{
    std::cout << "Room: " << name_ << "\n";
    std::cout << "Capacity: " << capacity_ << "\n";
}