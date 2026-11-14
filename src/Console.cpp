#include "Console.hpp"

#include <iomanip>
#include <iostream>

#include "Config.hpp"
#include "Statistics.hpp"
#include "Storage.hpp"
#include "Util.hpp"

namespace {

template <typename T>
void printSummary(const std::string& label, const std::optional<Summary<T>>& s, const std::string& unit) {
    std::cout << "  " << label << " : ";
    if (!s) {
        std::cout << "no data available\n";
        return;
    }
    std::cout << std::fixed << std::setprecision(2) << "min " << s->min << unit << ", max " << s->max << unit
              << ", average " << s->mean << unit << " (across " << s->count << " session(s))\n"
              << std::defaultfloat;
}

}  // namespace

Console::Console(Venue& venue, std::string dataPath) : venue_(venue), path_(std::move(dataPath)) {}

std::string Console::ask(const std::string& prompt) const {
    std::cout << prompt << " " << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) throw EndOfInput{};
    return util::trim(line);
}

int Console::askInt(const std::string& prompt) const {
    while (true) {
        if (const auto v = util::parseInt(ask(prompt))) return *v;
        std::cout << "  Invalid input: an integer is required.\n";
    }
}

double Console::askDouble(const std::string& prompt) const {
    while (true) {
        if (const auto v = util::parseDouble(ask(prompt))) return *v;
        std::cout << "  Invalid input: a number is required.\n";
    }
}

Date Console::askDate(const std::string& prompt) const {
    while (true) {
        if (const auto v = Date::parse(ask(prompt))) return *v;
        std::cout << "  Invalid date: YYYY-MM-DD format expected (e.g. 2026-11-20).\n";
    }
}

int Console::askTime(const std::string& prompt) const {
    while (true) {
        if (const auto v = timeofday::parse(ask(prompt))) return *v;
        std::cout << "  Invalid time: HH:MM format expected (e.g. 20:30).\n";
    }
}

void Console::persist() const {
    try {
        storage::save(venue_, path_);
    } catch (const std::exception& e) {
        std::cout << "  WARNING: save failed: " << e.what() << "\n";
    }
}

void Console::run() {
    try {
        while (true) {
            std::cout << "\n=== Cultural venue ===\n"
                         " 1. Display the program (by date)\n"
                         " 2. Search for an event\n"
                         " 3. Book seats\n"
                         " 4. Statistics and revenue\n"
                         " 5. Administration\n"
                         " 0. Exit\n";
            const std::string choice = ask("Choice:");
            if (choice == "1") showSchedule();
            else if (choice == "2") searchEvents();
            else if (choice == "3") bookSeats();
            else if (choice == "4") showStatistics();
            else if (choice == "5") administration();
            else if (choice == "0") break;
            else std::cout << "Unknown choice.\n";
        }
    } catch (const EndOfInput&) {
        std::cout << "\n(end of input)\n";
    }
    std::cout << "Goodbye.\n";
}

void Console::printScheduled(const ScheduledEvent& s) const {
    const Event* ev = venue_.findEvent(s.eventId);
    const Room* room = venue_.findRoom(s.roomId);
    std::cout << "  [" << s.id << "] " << s.date.toString() << " " << timeofday::toString(s.startMinutes) << " ("
              << ev->durationMinutes() << " min) | " << ev->title() << " (" << ev->typeLabel() << ") | " << room->name
              << " | " << s.remaining() << "/" << s.quota << " seats left | " << util::formatPrice(ev->price())
              << "\n        " << ev->details() << "\n";
}

void Console::showSchedule() const {
    const auto list = venue_.scheduleByDate();
    if (list.empty()) {
        std::cout << "No scheduled session.\n";
        return;
    }
    std::cout << "\nProgram:\n";
    for (const ScheduledEvent* s : list) printScheduled(*s);
}

void Console::searchEvents() const {
    const std::string query = ask("Title (or part of the title):");
    if (query.empty()) {
        std::cout << "Empty search: enter at least one character.\n";
        return;
    }
    const auto ids = venue_.searchEventsByTitle(query);
    if (ids.empty()) {
        std::cout << "No event matches \"" << query << "\".\n";
        return;
    }
    for (int id : ids) {
        const Event* ev = venue_.findEvent(id);
        std::cout << "\n- " << ev->title() << " (" << ev->typeLabel() << ")\n";
        for (const ScheduledEvent* s : venue_.scheduledEventsOf(id)) printScheduled(*s);
    }
}

void Console::bookSeats() {
    showSchedule();
    if (venue_.scheduledEvents().empty()) return;
    const int id = askInt("Session number:");
    const int qty = askInt("Number of seats requested:");
    const Result r = venue_.bookSeats(id, qty);
    std::cout << (r.ok ? "OK - " : "") << r.message << "\n";
    if (r.ok) persist();
}

void Console::showStatistics() const {
    std::cout << "\nTotal revenue: " << util::formatPrice(venue_.totalRevenue()) << "\n";
    const auto rooms = venue_.mostUsedRooms();
    if (rooms.empty()) {
        std::cout << "Most used room(s): no scheduled session.\n";
    } else {
        std::cout << "Most used room(s):";
        for (const auto& r : rooms) std::cout << " \"" << r.first->name << "\" (" << r.second << " session(s))";
        std::cout << "\n";
    }
    std::cout << "Statistics by session:\n";
    printSummary("Ticket price", venue_.priceSummary(), " EUR");
    printSummary("Attendance  ", venue_.attendanceSummary(), " seat(s)");
    printSummary("Duration    ", venue_.durationSummary(), " min");
}

void Console::administration() {
    const std::string code = ask("Admin code (4 letters):");
    if (code != kAdminCode) {
        std::cout << "Incorrect code: access to administration denied.\n";
        return;
    }
    while (true) {
        std::cout << "\n--- Administration ---\n"
                     " 1. Add an event (with its first session)\n"
                     " 2. Add a session for an existing event\n"
                     " 3. Add a room\n"
                     " 0. Back\n";
        const std::string choice = ask("Choice:");
        if (choice == "1") addEvent();
        else if (choice == "2") scheduleAnotherShow();
        else if (choice == "3") addRoom();
        else if (choice == "0") return;
        else std::cout << "Unknown choice.\n";
    }
}

Console::Slot Console::askSlot() const {
    std::cout << "Rooms:\n";
    for (const auto& r : venue_.rooms())
        std::cout << "  [" << r.first << "] " << r.second.name << " (capacity " << r.second.capacity << ")\n";
    Slot slot;
    slot.roomId = askInt("Room number:");
    slot.date = askDate("Date (YYYY-MM-DD):");
    slot.startMinutes = askTime("Start time (HH:MM):");
    slot.quota = askInt("Number of seats for sale (quota):");
    return slot;
}

void Console::addEvent() {
    if (venue_.rooms().empty()) {
        std::cout << "No room exists: add a room first.\n";
        return;
    }
    std::vector<const EventType*> types;
    for (const auto& kv : EventRegistry::instance().all()) types.push_back(&kv.second);
    std::cout << "Event types:\n";
    for (std::size_t i = 0; i < types.size(); ++i) std::cout << "  " << i + 1 << ". " << types[i]->label << "\n";
    const int choice = askInt("Type (number):");
    if (choice < 1 || choice > static_cast<int>(types.size())) {
        std::cout << "Unknown type.\n";
        return;
    }
    const EventType& type = *types[static_cast<std::size_t>(choice - 1)];

    std::unique_ptr<Event> event;
    while (!event) {
        const std::string title = ask("Title:");
        const int duration = askInt("Duration in minutes:");
        const double price = askDouble("Price per ticket (EUR):");
        std::vector<std::string> fields;
        for (const auto& prompt : type.fieldPrompts) fields.push_back(ask(prompt + " :"));
        try {
            event = EventRegistry::instance().create(type.key, title, duration, price, fields);
        } catch (const std::invalid_argument& e) {
            std::cout << "  Input rejected: " << e.what() << ". Please try again.\n";
        }
    }
    const Slot s = askSlot();
    const Result r = venue_.scheduleNewEvent(std::move(event), s.roomId, s.date, s.startMinutes, s.quota);
    std::cout << (r.ok ? "OK - " : "ADD REJECTED - ") << r.message << "\n";
    if (r.ok) persist();
}

void Console::scheduleAnotherShow() {
    if (venue_.events().empty() || venue_.rooms().empty()) {
        std::cout << "At least one event and one room are required.\n";
        return;
    }
    std::cout << "Events:\n";
    for (const auto& e : venue_.events()) std::cout << "  [" << e.first << "] " << e.second->title() << "\n";
    const int eventId = askInt("Event number:");
    const Slot s = askSlot();
    const Result r = venue_.scheduleExistingEvent(eventId, s.roomId, s.date, s.startMinutes, s.quota);
    std::cout << (r.ok ? "OK - " : "ADD REJECTED - ") << r.message << "\n";
    if (r.ok) persist();
}

void Console::addRoom() {
    const std::string name = ask("Room name:");
    const int capacity = askInt("Capacity:");
    const Result r = venue_.addRoom(Room{venue_.nextRoomId(), name, capacity});
    std::cout << (r.ok ? "OK - " : "ADD REJECTED - ") << r.message << "\n";
    if (r.ok) persist();
}
