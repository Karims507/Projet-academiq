#pragma once

#include <string>

#include "concert.hpp"

class Screening : public Concert {
public:
    Screening(std::string title = "Screening", int durationMinutes = 120, std::string film = "");

    void play() const override;
    const std::string& getFilm() const noexcept;

private:
    std::string film_;
};
