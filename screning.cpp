#include "screning.hpp"

#include <iostream>
#include <utility>

Screening::Screening(std::string title, int durationMinutes, std::string film)
    : Concert(std::move(title), durationMinutes), film_(std::move(film)) {}

void Screening::play() const {
    std::cout << "Screening: " << getTitle() << " - " << film_
              << " (" << getDurationMinutes() << " min)" << std::endl;
}

const std::string& Screening::getFilm() const noexcept {
    return film_;
}
