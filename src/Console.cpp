#include "Console.hpp"

#include <iomanip>
#include <iostream>

#include "Config.hpp"
#include "Statistics.hpp"
#include "Storage.hpp"
#include "Util.hpp"

namespace {

template <typename T>
void printSummary(const std::string& label, const std::optional<Summary<T>>& s, const std::string& unit) {
    std::cout << "  " << label << " : ";
    if (!s) {
        std::cout << "aucune donnee disponible\n";
        return;
    }
    std::cout << std::fixed << std::setprecision(2) << "min " << s->min << unit << ", max " << s->max << unit
              << ", moyenne " << s->mean << unit << " (sur " << s->count << " seance(s))\n"
              << std::defaultfloat;
}

}  // namespace

Console::Console(Venue& venue, std::string dataPath) : venue_(venue), path_(std::move(dataPath)) {}

std::string Console::ask(const std::string& prompt) const {
    std::cout << prompt << " " << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) throw EndOfInput{};
    return util::trim(line);
}

int Console::askInt(const std::string& prompt) const {
    while (true) {
        if (const auto v = util::parseInt(ask(prompt))) return *v;
        std::cout << "  Entree invalide : un nombre entier est attendu.\n";
    }
}

double Console::askDouble(const std::string& prompt) const {
    while (true) {
        if (const auto v = util::parseDouble(ask(prompt))) return *v;
        std::cout << "  Entree invalide : un nombre est attendu.\n";
    }
}

Date Console::askDate(const std::string& prompt) const {
    while (true) {
        if (const auto v = Date::parse(ask(prompt))) return *v;
        std::cout << "  Date invalide : format AAAA-MM-JJ attendu (ex. 2026-11-20).\n";
    }
}

int Console::askTime(const std::string& prompt) const {
    while (true) {
        if (const auto v = timeofday::parse(ask(prompt))) return *v;
        std::cout << "  Heure invalide : format HH:MM attendu (ex. 20:30).\n";
    }
}

void Console::persist() const {
    try {
        storage::save(venue_, path_);
    } catch (const std::exception& e) {
        std::cout << "  ATTENTION, sauvegarde impossible : " << e.what() << "\n";
    }
}

void Console::run() {
    try {
        while (true) {
            std::cout << "\n=== Lieu culturel ===\n"
                         " 1. Afficher la programmation (par date)\n"
                         " 2. Rechercher un evenement\n"
                         " 3. Reserver des places\n"
                         " 4. Statistiques et recettes\n"
                         " 5. Administration\n"
                         " 0. Quitter\n";
            const std::string choice = ask("Choix :");
            if (choice == "1") showSchedule();
            else if (choice == "2") searchEvents();
            else if (choice == "3") bookSeats();
            else if (choice == "4") showStatistics();
            else if (choice == "5") administration();
            else if (choice == "0") break;
            else std::cout << "Choix inconnu.\n";
        }
    } catch (const EndOfInput&) {
        std::cout << "\n(fin de l'entree)\n";
    }
    std::cout << "Au revoir.\n";
}

void Console::printScheduled(const ScheduledEvent& s) const {
    const Event* ev = venue_.findEvent(s.eventId);
    const Room* room = venue_.findRoom(s.roomId);
    std::cout << "  [" << s.id << "] " << s.date.toString() << " " << timeofday::toString(s.startMinutes) << " ("
              << ev->durationMinutes() << " min) | " << ev->title() << " (" << ev->typeLabel() << ") | " << room->name
              << " | " << s.remaining() << "/" << s.quota << " places restantes | " << util::formatPrice(ev->price())
              << "\n        " << ev->details() << "\n";
}

void Console::showSchedule() const {
    const auto list = venue_.scheduleByDate();
    if (list.empty()) {
        std::cout << "Aucune seance programmee.\n";
        return;
    }
    std::cout << "\nProgrammation :\n";
    for (const ScheduledEvent* s : list) printScheduled(*s);
}

void Console::searchEvents() const {
    const std::string query = ask("Titre (ou partie du titre) :");
    if (query.empty()) {
        std::cout << "Recherche vide : saisissez au moins un caractere.\n";
        return;
    }
    const auto ids = venue_.searchEventsByTitle(query);
    if (ids.empty()) {
        std::cout << "Aucun evenement ne correspond a \"" << query << "\".\n";
        return;
    }
    for (int id : ids) {
        const Event* ev = venue_.findEvent(id);
        std::cout << "\n- " << ev->title() << " (" << ev->typeLabel() << ")\n";
        for (const ScheduledEvent* s : venue_.scheduledEventsOf(id)) printScheduled(*s);
    }
}

void Console::bookSeats() {
    showSchedule();
    if (venue_.scheduledEvents().empty()) return;
    const int id = askInt("Numero de la seance :");
    const int qty = askInt("Nombre de places souhaite :");
    const Result r = venue_.bookSeats(id, qty);
    std::cout << (r.ok ? "OK - " : "") << r.message << "\n";
    if (r.ok) persist();
}

void Console::showStatistics() const {
    std::cout << "\nRecettes totales : " << util::formatPrice(venue_.totalRevenue()) << "\n";
    const auto rooms = venue_.mostUsedRooms();
    if (rooms.empty()) {
        std::cout << "Salles les plus utilisees : aucune seance programmee.\n";
    } else {
        std::cout << "Salle(s) la/les plus utilisee(s) :";
        for (const auto& r : rooms) std::cout << " « " << r.first->name << " » (" << r.second << " seance(s))";
        std::cout << "\n";
    }
    std::cout << "Statistiques par seance :\n";
    printSummary("Prix de la place", venue_.priceSummary(), " EUR");
    printSummary("Frequentation  ", venue_.attendanceSummary(), " place(s)");
    printSummary("Duree          ", venue_.durationSummary(), " min");
}

void Console::administration() {
    const std::string code = ask("Code administrateur (4 lettres) :");
    if (code != kAdminCode) {
        std::cout << "Code incorrect : acces à l'administration refuser.\n";
        return;
    }
    while (true) {
        std::cout << "\n--- Administration ---\n"
                     " 1. Ajouter un evenement (avec sa premiere seance)\n"
                     " 2. Ajouter une seance a un evenement existant\n"
                     " 3. Ajouter une salle\n"
                     " 0. Retour\n";
        const std::string choice = ask("Choix :");
        if (choice == "1") addEvent();
        else if (choice == "2") scheduleAnotherShow();
        else if (choice == "3") addRoom();
        else if (choice == "0") return;
        else std::cout << "Choix inconnu.\n";
    }
}

Console::Slot Console::askSlot() const {
    std::cout << "Salles :\n";
    for (const auto& r : venue_.rooms())
        std::cout << "  [" << r.first << "] " << r.second.name << " (capacite " << r.second.capacity << ")\n";
    Slot slot;
    slot.roomId = askInt("Numero de la salle :");
    slot.date = askDate("Date (AAAA-MM-JJ) :");
    slot.startMinutes = askTime("Heure de debut (HH:MM) :");
    slot.quota = askInt("Nombre de places mises en vente (quota) :");
    return slot;
}

void Console::addEvent() {
    if (venue_.rooms().empty()) {
        std::cout << "Aucune salle n'existe : ajoutez d'abord une salle.\n";
        return;
    }
    std::vector<const EventType*> types;
    for (const auto& kv : EventRegistry::instance().all()) types.push_back(&kv.second);
    std::cout << "Types d'evenement :\n";
    for (std::size_t i = 0; i < types.size(); ++i) std::cout << "  " << i + 1 << ". " << types[i]->label << "\n";
    const int choice = askInt("Type (numero) :");
    if (choice < 1 || choice > static_cast<int>(types.size())) {
        std::cout << "Type inconnu.\n";
        return;
    }
    const EventType& type = *types[static_cast<std::size_t>(choice - 1)];

    std::unique_ptr<Event> event;
    while (!event) {
        const std::string title = ask("Titre :");
        const int duration = askInt("Duree en minutes :");
        const double price = askDouble("Prix d'une place (EUR) :");
        std::vector<std::string> fields;
        for (const auto& prompt : type.fieldPrompts) fields.push_back(ask(prompt + " :"));
        try {
            event = EventRegistry::instance().create(type.key, title, duration, price, fields);
        } catch (const std::invalid_argument& e) {
            std::cout << "  Saisie refusee : " << e.what() << ". On recommence.\n";
        }
    }
    const Slot s = askSlot();
    const Result r = venue_.scheduleNewEvent(std::move(event), s.roomId, s.date, s.startMinutes, s.quota);
    std::cout << (r.ok ? "OK - " : "AJOUT REFUSE - ") << r.message << "\n";
    if (r.ok) persist();
}

void Console::scheduleAnotherShow() {
    if (venue_.events().empty() || venue_.rooms().empty()) {
        std::cout << "Il faut au moins un evenement et une salle.\n";
        return;
    }
    std::cout << "Evenements :\n";
    for (const auto& e : venue_.events()) std::cout << "  [" << e.first << "] " << e.second->title() << "\n";
    const int eventId = askInt("Numero de l'evenement :");
    const Slot s = askSlot();
    const Result r = venue_.scheduleExistingEvent(eventId, s.roomId, s.date, s.startMinutes, s.quota);
    std::cout << (r.ok ? "OK - " : "AJOUT REFUSE - ") << r.message << "\n";
    if (r.ok) persist();
}

void Console::addRoom() {
    const std::string name = ask("Nom de la salle :");
    const int capacity = askInt("Capacite :");
    const Result r = venue_.addRoom(Room{venue_.nextRoomId(), name, capacity});
    std::cout << (r.ok ? "OK - " : "AJOUT REFUSE - ") << r.message << "\n";
    if (r.ok) persist();
}
