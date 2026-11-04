#pragma once
#include <string>

#include "Venue.hpp"

// Lancee quand l'entree standard est fermee (fin de fichier) : permet de scripter l'application.
struct EndOfInput {};

class Console {
public:
    Console(Venue& venue, std::string dataPath);
    void run();

private:
    struct Slot {
        int roomId;
        Date date;
        int startMinutes;
        int quota;
    };

    void showSchedule() const;
    void searchEvents() const;
    void bookSeats();
    void showStatistics() const;
    void administration();
    void addEvent();
    void scheduleAnotherShow();
    void addRoom();

    void persist() const;
    void printScheduled(const ScheduledEvent& s) const;
    Slot askSlot() const;
    std::string ask(const std::string& prompt) const;
    int askInt(const std::string& prompt) const;
    double askDouble(const std::string& prompt) const;
    Date askDate(const std::string& prompt) const;
    int askTime(const std::string& prompt) const;

    Venue& venue_;
    std::string path_;
};
