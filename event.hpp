#pragma once

#include <string>

class Event {
public:
    Event(std::string title = "", int durationMinutes = 0);
    virtual ~Event() = default;

    const std::string& getTitle() const noexcept;
    int getDurationMinutes() const noexcept;
    virtual std::string description() const;

protected:
    std::string title_;
    int durationMinutes_;
};
