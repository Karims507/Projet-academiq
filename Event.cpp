#include "Event.hpp"

Event::Event(std::string title, int durationMinutes, double price)
    : title_(title),
      durationMinutes_(durationMinutes),
      price_(price)
{
}

std::string Event::getTitle() const
{
    return title_;
}

int Event::getDuration() const
{
    return durationMinutes_;
}

double Event::getPrice() const
{
    return price_;
}

void Event::setTitle(std::string title)
{
    title_ = title;
}

void Event::setDuration(int duration)
{
    durationMinutes_ = duration;
}

void Event::setPrice(double price)
{
    price_ = price;
}