#pragma once
#include "Date.hpp"

// Session (course diagram: ScheduledEvent): keeps links to the event and room,
// a date + start time, a seat quota and the number of reservations.
// Links are made by ID (eventId, roomId): simple persistence, no pointer during runtime.
struct ScheduledEvent {
    int id = 0;
    int eventId = 0;
    int roomId = 0;
    Date date;
    int startMinutes = 0;  // minutes since midnight
    int quota = 0;         // seats put on sale
    int reservations = 0;  // reserved seats

    int remaining() const { return quota - reservations; }
    long long startAbsolute() const { return date.toDays() * 1440LL + startMinutes; }  // minutes since 1970
};
