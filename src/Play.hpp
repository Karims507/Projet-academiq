#pragma once
#include "Event.hpp"

// Pièce de théâtre : comporte un entracte.
class Play : public Event {
public:
    Play(std::string title, int durationMinutes, double price, int intermissionMinutes)
        : Event(std::move(title), durationMinutes, price), intermissionMinutes_(intermissionMinutes) {}

    std::string typeKey() const override { return "PLAY"; }
    std::string typeLabel() const override { return "Piece de theatre"; }
    std::string details() const override { return "Entracte : " + std::to_string(intermissionMinutes_) + " min"; }
    std::vector<std::string> extraFields() const override { return {std::to_string(intermissionMinutes_)}; }

    int intermissionMinutes() const { return intermissionMinutes_; }

private:
    int intermissionMinutes_;
};
