#pragma once
#include "Event.hpp"

// Play: includes an intermission.
class Play : public Event {
public:
    Play(std::string title, int durationMinutes, double price, int intermissionMinutes)
        : Event(std::move(title), durationMinutes, price), intermissionMinutes_(intermissionMinutes) {}

    std::string typeKey() const override { return "PLAY"; }
    std::string typeLabel() const override { return "Play"; }
    std::string details() const override { return "Intermission: " + std::to_string(intermissionMinutes_) + " min"; }
    std::vector<std::string> extraFields() const override { return {std::to_string(intermissionMinutes_)}; }

    int intermissionMinutes() const { return intermissionMinutes_; }

private:
    int intermissionMinutes_;
};
