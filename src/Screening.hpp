#pragma once
#include "Event.hpp"

// Screening: minimum audience age (minimumAge_).
class Screening : public Event {
public:
    Screening(std::string title, int durationMinutes, double price, int minimumAge)
        : Event(std::move(title), durationMinutes, price), minimumAge_(minimumAge) {}

    std::string typeKey() const override { return "SCREENING"; }
    std::string typeLabel() const override { return "Screening"; }
    std::string details() const override {
        return minimumAge_ == 0 ? "All ages" : "Minimum age: " + std::to_string(minimumAge_) + " years";
    }
    std::vector<std::string> extraFields() const override { return {std::to_string(minimumAge_)}; }

    int minimumAge() const { return minimumAge_; }

private:
    int minimumAge_;
};
