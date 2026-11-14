#pragma once
#include "Event.hpp"

// Concert: list of artists + soundchecks that occupy the room just before the concert.
class Concert : public Event {
public:
    Concert(std::string title, int durationMinutes, double price, std::vector<std::string> artists,
            int soundcheckMinutes)
        : Event(std::move(title), durationMinutes, price), artists_(std::move(artists)), soundcheckMinutes_(soundcheckMinutes) {}

    std::string typeKey() const override { return "CONCERT"; }
    std::string typeLabel() const override { return "Concert"; }
    std::string details() const override;
    std::vector<std::string> extraFields() const override;
    int preparationMinutes() const override { return soundcheckMinutes_; }

    const std::vector<std::string>& artists() const { return artists_; }
    int soundcheckMinutes() const { return soundcheckMinutes_; }

private:
    std::vector<std::string> artists_;
    int soundcheckMinutes_;
};
