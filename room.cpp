#include "room.hpp"

#include <utility>

Room::Room(std::string name, int capacity)
    : name_(std::move(name)), capacity_(capacity) {}

const std::string& Room::getName() const noexcept {
    return name_;
}

int Room::getCapacity() const noexcept {
    return capacity_;
}

bool Room::hasCapacity(int seats) const noexcept {
    return capacity_ >= seats;
}
