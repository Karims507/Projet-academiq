#pragma once
#include "Event.hpp"

// Projection (Screening dans le diagramme du cours) : age minimum du public (minimumAge_).
class Screening : public Event {
public:
    Screening(std::string title, int durationMinutes, double price, int minimumAge)
        : Event(std::move(title), durationMinutes, price), minimumAge_(minimumAge) {}

    std::string typeKey() const override { return "SCREENING"; }
    std::string typeLabel() const override { return "Projection"; }
    std::string details() const override {
        return minimumAge_ == 0 ? "Tout public" : "Âge minimum : " + std::to_string(minimumAge_) + " ans";
    }
    std::vector<std::string> extraFields() const override { return {std::to_string(minimumAge_)}; }

    int minimumAge() const { return minimumAge_; }

private:
    int minimumAge_;
};
