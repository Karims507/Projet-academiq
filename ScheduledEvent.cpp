#include "ScheduledEvent.hpp"
#include <iostream>

ScheduledEvent::ScheduledEvent(Event* event,
                               Room* room,
                               std::string date,
                               std::string startTime,
                               int quota)
    : event_(event),
      room_(room),
      date_(date),
      startTime_(startTime),
      quota_(quota),
      reservations_(0)
{
    if (quota_ > room_->getCapacity())
    {
        quota_ = room_->getCapacity();
    }
}

bool ScheduledEvent::bookSeats(int quantity)
{
    if (quantity <= 0)
    {
        std::cout << "Invalid quantity\n";
        return false;
    }

    if (reservations_ + quantity > quota_)
    {
        std::cout << "Not enough seats available\n";
        return false;
    }

    reservations_ += quantity;

    std::cout << "Booking accepted\n";
    return true;
}

void ScheduledEvent::display() const
{
    event_->display();

    std::cout << "Room: " << room_->getName() << "\n";
    std::cout << "Date: " << date_ << "\n";
    std::cout << "Time: " << startTime_ << "\n";
    std::cout << "Reservations: "
              << reservations_
              << "/"
              << quota_
              << "\n";
}

int ScheduledEvent::getReservations() const
{
    return reservations_;
}