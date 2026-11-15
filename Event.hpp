#pragma once

#include <string>

class Event
{
protected:
    std::string title_;
    int durationMinutes_;
    double price_;

public:
    Event(std::string title, int durationMinutes, double price);

    std::string getTitle() const;
    int getDuration() const;
    double getPrice() const;

    void setTitle(std::string title);
    void setDuration(int duration);
    void setPrice(double price);

    virtual void display() const = 0;

    virtual ~Event() = default;
};