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

// Résultat d'une opération métier : accepté/refusé + message compréhensible.
struct Result {
    bool ok;
    std::string message;
    static Result success(std::string m = "") { return {true, std::move(m)}; }
    static Result failure(std::string m) { return {false, std::move(m)}; }
};

// Le lieu culturel : salles, événements, séances. Toutes les règles métier sont ici
// (pas d'entrée/sortie), ce qui les rend testables et indépendantes de l'interface.
class Venue {
public:
    // Opérations bas niveau (utilisées aussi par le chargement de fichier).
    Result addRoom(const Room& room);
    Result addEvent(int id, std::unique_ptr<Event> event);
    Result addScheduledEvent(const ScheduledEvent& s);  // vérifie salle, quota, chevauchement

    // Opérations de haut niveau (administration).
    Result scheduleNewEvent(std::unique_ptr<Event> event, int roomId, const Date& date, int startMinutes,
                            int quota);  // tout ou rien
    Result scheduleExistingEvent(int eventId, int roomId, const Date& date, int startMinutes, int quota);

    // Public.
    Result bookSeats(int scheduledEventId, int quantity);

    // Accès.
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
    std::vector<int> searchEventsByTitle(const std::string& query) const;  // insensible à la casse
    std::vector<const ScheduledEvent*> scheduledEventsOf(int eventId) const;
    std::vector<const ScheduledEvent*> scheduleByDate() const;
    double totalRevenue() const;
    std::vector<std::pair<const Room*, int>> mostUsedRooms() const;  // ex æquo inclus ; vide si aucune séance
    std::optional<Summary<double>> priceSummary() const;       // prix de la place, par séance
    std::optional<Summary<int>> attendanceSummary() const;     // places réservées, par séance
    std::optional<Summary<int>> durationSummary() const;       // durée en minutes, par séance

private:
    Result checkSlot(const Event& event, int roomId, const Date& date, int startMinutes, int quota,
                     int reservations) const;

    std::map<int, Room> rooms_;
    std::map<int, std::unique_ptr<Event>> events_;
    std::map<int, ScheduledEvent> scheduled_;
};
