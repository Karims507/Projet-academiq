#pragma once

#include <string>

class Room {
public:
    Room(std::string name = "", int capacity = 0);

    const std::string& getName() const noexcept;
    int getCapacity() const noexcept;
    bool hasCapacity(int seats) const noexcept;

private:
    std::string name_;
    int capacity_;
};
