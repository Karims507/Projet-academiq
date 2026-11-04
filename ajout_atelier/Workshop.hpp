#pragma once
#include "Event.hpp"

// Atelier : un intervenant et un niveau. Aucun fichier existant n'est modifie pour l'ajouter.
class Workshop : public Event {
public:
    Workshop(std::string title, int durationMinutes, double price, std::string instructor, std::string level)
        : Event(std::move(title), durationMinutes, price), instructor_(std::move(instructor)), level_(std::move(level)) {}

    std::string typeKey() const override { return "WORKSHOP"; }
    std::string typeLabel() const override { return "Atelier"; }
    std::string details() const override { return "Intervenant : " + instructor_ + " | niveau : " + level_; }
    std::vector<std::string> extraFields() const override { return {instructor_, level_}; }

private:
    std::string instructor_, level_;
};
