#pragma once
#include "Date.hpp"

// Seance (diagramme du cours : ScheduledEvent) : garde des liens vers l'evenement et la salle,
// une date + une heure de debut, un quota de places et le nombre de reservations.
// Les liens se font par identifiant (eventId, roomId) : sauvegarde simple, pas de pointeur pendant.
struct ScheduledEvent {
    int id = 0;
    int eventId = 0;
    int roomId = 0;
    Date date;
    int startMinutes = 0;  // minutes depuis minuit
    int quota = 0;         // places mises en vente
    int reservations = 0;  // places reservees

    int remaining() const { return quota - reservations; }
    long long startAbsolute() const { return date.toDays() * 1440LL + startMinutes; }  // minutes depuis 1970
};
