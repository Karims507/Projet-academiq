#pragma once

#include <string>
#include "Event.hpp"
#include "Room.hpp"

class ScheduledEvent
{
private:
    Event* event_;
    Room* room_;
    std::string date_;
    std::string startTime_;
    int quota_;
    int reservations_;

public:
    ScheduledEvent(Event* event,
                   Room* room,
                   std::string date,
                   std::string startTime,
                   int quota);

    bool bookSeats(int quantity);
    void display() const;
    int getReservations() const;
};