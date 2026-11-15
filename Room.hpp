#pragma once

#include <string>

class Room
{
private:
    std::string name_;
    int capacity_;

public:
    Room(std::string name, int capacity);

    std::string getName() const;
    int getCapacity() const;

    void display() const;
};