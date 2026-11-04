#pragma once
#include "Date.hpp"

// Séance (diagramme du cours : ScheduledEvent) : garde des liens vers l'événement et la salle,
// une date + une heure de début, un quota de places et le nombre de réservations.
// Les liens se font par identifiant (eventId, roomId) : sauvegarde simple, pas de pointeur pendant.
struct ScheduledEvent {
    int id = 0;
    int eventId = 0;
    int roomId = 0;
    Date date;
    int startMinutes = 0;  // minutes depuis minuit
    int quota = 0;         // places mises en vente
    int reservations = 0;  // places réservées

    int remaining() const { return quota - reservations; }
    long long startAbsolute() const { return date.toDays() * 1440LL + startMinutes; }  // minutes depuis 1970
};
