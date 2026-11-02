#include <iostream>

#include "play.hpp"
#include "room.hpp"
#include "scheduleedEvent.hpp"
#include "screning.hpp"

int main() {
    Play hamlet("Hamlet", 120, "Shakespeare");
    Screening film("Inception", 148, "Christopher Nolan");
    Room salleA("Salle A", 200);
    ScheduledEvent premiere("Premier", salleA, hamlet.getDurationMinutes());

    std::cout << premiere.summary() << std::endl;
    hamlet.play();
    film.play();

    return 0;
}
