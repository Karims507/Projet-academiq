#pragma once
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Date.hpp"
#include "Event.hpp"
#include "Room.hpp"
#include "ScheduledEvent.hpp"
#include "Statistics.hpp"

// Result of a business operation: accepted/rejected + understandable message.
struct Result {
    bool ok;
    std::string message;
    static Result success(std::string m = "") { return {true, std::move(m)}; }
    static Result failure(std::string m) { return {false, std::move(m)}; }
};

// The cultural venue: rooms, events, sessions. All business rules are kept here
// (no input/output), making them testable and independent from the interface.
class Venue {
public:
    // Low-level operations (also used by file loading).
    Result addRoom(const Room& room);
    Result addEvent(int id, std::unique_ptr<Event> event);
    Result addScheduledEvent(const ScheduledEvent& s);  // validates room, quota, overlap

    // High-level operations (administration).
    Result scheduleNewEvent(std::unique_ptr<Event> event, int roomId, const Date& date, int startMinutes,
                            int quota);  // all or nothing
    Result scheduleExistingEvent(int eventId, int roomId, const Date& date, int startMinutes, int quota);

    // Public.
    Result bookSeats(int scheduledEventId, int quantity);

    // Access.
    int nextRoomId() const;
    int nextEventId() const;
    int nextScheduledEventId() const;
    const Room* findRoom(int id) const;
    const Event* findEvent(int id) const;
    const ScheduledEvent* findScheduledEvent(int id) const;
    const std::map<int, Room>& rooms() const { return rooms_; }
    const std::map<int, std::unique_ptr<Event>>& events() const { return events_; }
    const std::map<int, ScheduledEvent>& scheduledEvents() const { return scheduled_; }

    // Consultation.
    std::vector<int> searchEventsByTitle(const std::string& query) const;  // case insensitive
    std::vector<const ScheduledEvent*> scheduledEventsOf(int eventId) const;
    std::vector<const ScheduledEvent*> scheduleByDate() const;
    double totalRevenue() const;
    std::vector<std::pair<const Room*, int>> mostUsedRooms() const;  // ties included; empty if no session
    std::optional<Summary<double>> priceSummary() const;       // ticket price, per session
    std::optional<Summary<int>> attendanceSummary() const;     // reserved seats, per session
    std::optional<Summary<int>> durationSummary() const;       // duration in minutes, per session

private:
    Result checkSlot(const Event& event, int roomId, const Date& date, int startMinutes, int quota,
                     int reservations) const;

    std::map<int, Room> rooms_;
    std::map<int, std::unique_ptr<Event>> events_;
    std::map<int, ScheduledEvent> scheduled_;
};
