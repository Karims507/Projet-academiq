#pragma once
#include "Event.hpp"

// Workshop: an instructor and a level. No existing file is modified to add it.
class Workshop : public Event {
public:
    Workshop(std::string title, int durationMinutes, double price, std::string instructor, std::string level)
        : Event(std::move(title), durationMinutes, price), instructor_(std::move(instructor)), level_(std::move(level)) {}

    std::string typeKey() const override { return "WORKSHOP"; }
    std::string typeLabel() const override { return "Workshop"; }
    std::string details() const override { return "Instructor: " + instructor_ + " | level: " + level_; }
    std::vector<std::string> extraFields() const override { return {instructor_, level_}; }

private:
    std::string instructor_, level_;
};
