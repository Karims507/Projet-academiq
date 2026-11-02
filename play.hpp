#pragma once

#include <string>

#include "concert.hpp"

class Play : public Concert {
public:
    Play(std::string title = "Play", int durationMinutes = 90, std::string author = "");

    void play() const override;
    const std::string& getAuthor() const noexcept;

private:
    std::string author_;
};
