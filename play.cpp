#include "play.hpp"

#include <iostream>
#include <utility>

Play::Play(std::string title, int durationMinutes, std::string author)
    : Concert(std::move(title), durationMinutes), author_(std::move(author)) {}

void Play::play() const {
    std::cout << "Play: " << getTitle() << " by " << author_
              << " (" << getDurationMinutes() << " min)" << std::endl;
}

const std::string& Play::getAuthor() const noexcept {
    return author_;
}
