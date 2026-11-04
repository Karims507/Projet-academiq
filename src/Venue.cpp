#include "Venue.hpp"

#include <algorithm>

#include "Util.hpp"

namespace {
template <typename Map>
int nextKey(const Map& m) {
    return m.empty() ? 1 : m.rbegin()->first + 1;
}
}  // namespace

int Venue::nextRoomId() const { return nextKey(rooms_); }
int Venue::nextEventId() const { return nextKey(events_); }
int Venue::nextScheduledEventId() const { return nextKey(scheduled_); }

const Room* Venue::findRoom(int id) const {
    const auto it = rooms_.find(id);
    return it == rooms_.end() ? nullptr : &it->second;
}
const Event* Venue::findEvent(int id) const {
    const auto it = events_.find(id);
    return it == events_.end() ? nullptr : it->second.get();
}
const ScheduledEvent* Venue::findScheduledEvent(int id) const {
    const auto it = scheduled_.find(id);
    return it == scheduled_.end() ? nullptr : &it->second;
}

Result Venue::addRoom(const Room& room) {
    if (room.id <= 0) return Result::failure("l'identifiant de salle doit être strictement positif");
    if (rooms_.count(room.id)) return Result::failure("identifiant de salle dejà utilise : " + std::to_string(room.id));
    if (util::trim(room.name).empty()) return Result::failure("le nom de la salle ne peut pas être vide");
    if (room.name.find('|') != std::string::npos) return Result::failure("le nom de la salle ne peut pas contenir '|'");
    if (room.capacity <= 0) return Result::failure("la capacite de la salle doit être strictement positive");
    rooms_.emplace(room.id, room);
    return Result::success("Salle « " + room.name + " » ajoutee (capacite " + std::to_string(room.capacity) + ").");
}

Result Venue::addEvent(int id, std::unique_ptr<Event> event) {
    if (!event) return Result::failure("evenement invalide");
    if (id <= 0) return Result::failure("l'identifiant d'evenement doit être strictement positif");
    if (events_.count(id)) return Result::failure("identifiant d'evenement deja utilise : " + std::to_string(id));
    events_.emplace(id, std::move(event));
    return Result::success();
}

Result Venue::checkSlot(const Event& event, int roomId, const Date& date, int startMinutes, int quota,
                        int reservations) const {
    const Room* room = findRoom(roomId);
    if (!room) return Result::failure("salle inconnue (identifiant " + std::to_string(roomId) + ")");
    if (startMinutes < 0 || startMinutes > 1439) return Result::failure("heure de debut invalide");
    if (quota <= 0) return Result::failure("le nombre de places mises en vente doit être strictement positif");
    if (quota > room->capacity)
        return Result::failure(std::to_string(quota) + " places demandees : cela depasse la capacite de la salle « " +
                               room->name + " » (" + std::to_string(room->capacity) + ")");
    if (reservations < 0 || reservations > quota)
        return Result::failure("le nombre de places reservees (" + std::to_string(reservations) +
                               ") doit être compris entre 0 et " + std::to_string(quota));

    // Occupation de la salle : [debut - preparation, debut + duree[ (deux seances qui se suivent sont permises).
    const long long start = date.toDays() * 1440LL + startMinutes;
    const long long begin = start - event.preparationMinutes();
    const long long end = start + event.durationMinutes();
    for (const auto& entry : scheduled_) {
        const ScheduledEvent& other = entry.second;
        if (other.roomId != roomId) continue;
        const Event* otherEvent = findEvent(other.eventId);
        const long long otherBegin = other.startAbsolute() - otherEvent->preparationMinutes();
        const long long otherEnd = other.startAbsolute() + otherEvent->durationMinutes();
        if (begin < otherEnd && otherBegin < end)
            return Result::failure("conflit de salle : « " + room->name + " » est dejà occupee par « " +
                                   otherEvent->title() + " » (" + other.date.toString() + " " +
                                   timeofday::toString(other.startMinutes) + ", " +
                                   std::to_string(otherEvent->durationMinutes()) + " min)");
    }
    return Result::success();
}

Result Venue::addScheduledEvent(const ScheduledEvent& s) {
    if (s.id <= 0) return Result::failure("l'identifiant de seance doit être strictement positif");
    if (scheduled_.count(s.id)) return Result::failure("identifiant de seance deja utilise : " + std::to_string(s.id));
    const Event* event = findEvent(s.eventId);
    if (!event) return Result::failure("evenement inconnu (identifiant " + std::to_string(s.eventId) + ")");
    const Result check = checkSlot(*event, s.roomId, s.date, s.startMinutes, s.quota, s.reservations);
    if (!check.ok) return check;
    scheduled_.emplace(s.id, s);
    return Result::success();
}

Result Venue::scheduleNewEvent(std::unique_ptr<Event> event, int roomId, const Date& date, int startMinutes,
                               int quota) {
    if (!event) return Result::failure("evenement invalide");
    const Result check = checkSlot(*event, roomId, date, startMinutes, quota, 0);
    if (!check.ok) return check;  // rien n'est modifie en cas de refus
    const int eventId = nextEventId();
    const std::string title = event->title();
    addEvent(eventId, std::move(event));
    addScheduledEvent(ScheduledEvent{nextScheduledEventId(), eventId, roomId, date, startMinutes, quota, 0});
    return Result::success("Evenement « " + title + " » programme le " + date.toString() + " à " +
                           timeofday::toString(startMinutes) + " (" + std::to_string(quota) + " places en vente).");
}

Result Venue::scheduleExistingEvent(int eventId, int roomId, const Date& date, int startMinutes, int quota) {
    if (!findEvent(eventId)) return Result::failure("evenement inconnu (identifiant " + std::to_string(eventId) + ")");
    const Result r =
        addScheduledEvent(ScheduledEvent{nextScheduledEventId(), eventId, roomId, date, startMinutes, quota, 0});
    if (!r.ok) return r;
    return Result::success("Seance ajoutee le " + date.toString() + " a " + timeofday::toString(startMinutes) + " (" +
                           std::to_string(quota) + " places en vente).");
}

Result Venue::bookSeats(int scheduledEventId, int quantity) {
    const auto it = scheduled_.find(scheduledEventId);
    if (it == scheduled_.end()) return Result::failure("Reservation refusee : seance inconnue.");
    ScheduledEvent& s = it->second;
    if (quantity <= 0) return Result::failure("Reservation refusee : le nombre de places doit être strictement positif.");
    if (s.remaining() == 0) return Result::failure("Reservation refusee : cette seance est complete.");
    if (quantity > s.remaining())
        return Result::failure("Reservation refusee : vous demandez " + std::to_string(quantity) +
                               " place(s), il n'en reste que " + std::to_string(s.remaining()) + ".");
    s.reservations += quantity;  // seul endroit ou les reservations augmentent
    const Event* ev = findEvent(s.eventId);
    return Result::success("Reservation acceptee : " + std::to_string(quantity) + " place(s) pour « " + ev->title() +
                           " » le " + s.date.toString() + " a " + timeofday::toString(s.startMinutes) + " — total " +
                           util::formatPrice(quantity * ev->price()) + " (il reste " +
                           std::to_string(s.remaining()) + " place(s)).");
}

std::vector<int> Venue::searchEventsByTitle(const std::string& query) const {
    const std::string needle = util::toLower(util::trim(query));
    std::vector<int> found;
    for (const auto& entry : events_)
        if (util::toLower(entry.second->title()).find(needle) != std::string::npos) found.push_back(entry.first);
    return found;
}

std::vector<const ScheduledEvent*> Venue::scheduledEventsOf(int eventId) const {
    std::vector<const ScheduledEvent*> out;
    for (const auto& entry : scheduled_)
        if (entry.second.eventId == eventId) out.push_back(&entry.second);
    std::sort(out.begin(), out.end(),
              [](const ScheduledEvent* a, const ScheduledEvent* b) { return a->startAbsolute() < b->startAbsolute(); });
    return out;
}

std::vector<const ScheduledEvent*> Venue::scheduleByDate() const {
    std::vector<const ScheduledEvent*> out;
    for (const auto& entry : scheduled_) out.push_back(&entry.second);
    std::sort(out.begin(), out.end(), [](const ScheduledEvent* a, const ScheduledEvent* b) {
        if (a->startAbsolute() != b->startAbsolute()) return a->startAbsolute() < b->startAbsolute();
        return a->id < b->id;
    });
    return out;
}

double Venue::totalRevenue() const {
    double total = 0.0;
    for (const auto& entry : scheduled_) total += entry.second.reservations * findEvent(entry.second.eventId)->price();
    return total;
}

std::vector<std::pair<const Room*, int>> Venue::mostUsedRooms() const {
    std::map<int, int> counts;
    for (const auto& entry : scheduled_) ++counts[entry.second.roomId];
    int best = 0;
    for (const auto& c : counts) best = std::max(best, c.second);
    std::vector<std::pair<const Room*, int>> out;
    for (const auto& c : counts)
        if (c.second == best) out.emplace_back(findRoom(c.first), c.second);
    return out;
}

std::optional<Summary<double>> Venue::priceSummary() const {
    std::vector<double> v;
    for (const auto& e : scheduled_) v.push_back(findEvent(e.second.eventId)->price());
    return summarize(v);
}
std::optional<Summary<int>> Venue::attendanceSummary() const {
    std::vector<int> v;
    for (const auto& e : scheduled_) v.push_back(e.second.reservations);
    return summarize(v);
}
std::optional<Summary<int>> Venue::durationSummary() const {
    std::vector<int> v;
    for (const auto& e : scheduled_) v.push_back(findEvent(e.second.eventId)->durationMinutes());
    return summarize(v);
}
