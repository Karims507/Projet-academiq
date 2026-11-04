#include "Storage.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>

#include "Util.hpp"

namespace {

using Fields = std::vector<std::string>;

void requireCount(const Fields& f, std::size_t expected, const std::string& usage) {
    if (f.size() != expected)
        throw std::invalid_argument("la ligne " + f[0] + " doit avoir " + std::to_string(expected) +
                                    " champs separes par '|' (" + usage + "), trouve " + std::to_string(f.size()));
}

int needInt(const Fields& f, std::size_t i, const std::string& name) {
    const auto v = util::parseInt(f[i]);
    if (!v)
        throw std::invalid_argument("champ n°" + std::to_string(i + 1) + " (" + name + ") invalide : '" + f[i] +
                                    "' (entier attendu)");
    return *v;
}

void check(const Result& r) {
    if (!r.ok) throw std::invalid_argument(r.message);
}

void parseRecord(Venue& venue, const Fields& f) {
    const std::string& kind = f[0];
    if (kind == "ROOM") {
        requireCount(f, 4, "ROOM|id|nom|capacite");
        check(venue.addRoom(Room{needInt(f, 1, "id"), f[2], needInt(f, 3, "capacite")}));
    } else if (kind == "EVENT") {
        if (f.size() < 6)
            throw std::invalid_argument(
                "la ligne EVENT doit contenir au moins 6 champs (EVENT|id|type|titre|durée|prix|...)");
        const int id = needInt(f, 1, "id");
        const int duration = needInt(f, 4, "duree");
        const auto price = util::parseDouble(f[5]);
        if (!price) throw std::invalid_argument("champ n°6 (prix) invalide : '" + f[5] + "' (nombre attendu)");
        std::unique_ptr<Event> ev =
            EventRegistry::instance().create(f[2], f[3], duration, *price, Fields(f.begin() + 6, f.end()));
        check(venue.addEvent(id, std::move(ev)));
    } else if (kind == "SCHEDULED") {
        requireCount(f, 8, "SCHEDULED|id|evenement|salle|date|heure|quota|reservations");
        ScheduledEvent s;
        s.id = needInt(f, 1, "id");
        s.eventId = needInt(f, 2, "evenement");
        s.roomId = needInt(f, 3, "salle");
        const auto date = Date::parse(f[4]);
        if (!date) throw std::invalid_argument("champ n°5 (date) invalide : '" + f[4] + "' (format AAAA-MM-JJ)");
        s.date = *date;
        const auto time = timeofday::parse(f[5]);
        if (!time) throw std::invalid_argument("champ n°6 (heure) invalide : '" + f[5] + "' (format HH:MM)");
        s.startMinutes = *time;
        s.quota = needInt(f, 6, "quota");
        s.reservations = needInt(f, 7, "reservations");
        if (!venue.findEvent(s.eventId))
            throw std::invalid_argument("evenement " + std::to_string(s.eventId) +
                                        " inconnu (les lignes EVENT doivent preceder les lignes SCHEDULED)");
        if (!venue.findRoom(s.roomId))
            throw std::invalid_argument("salle " + std::to_string(s.roomId) +
                                        " inconnue (les lignes ROOM doivent preceder les lignes SCHEDULED)");
        check(venue.addScheduledEvent(s));
    } else {
        throw std::invalid_argument("type d'enregistrement inconnu '" + kind + "' (attendu : ROOM, EVENT ou SCHEDULED)");
    }
}

}  // namespace

namespace storage {

bool exists(const std::string& path) { return std::filesystem::exists(path); }

Venue load(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw DataError(path, 0, "impossible d'ouvrir le fichier");
    Venue venue;
    std::string raw;
    int lineNo = 0;
    while (std::getline(in, raw)) {
        ++lineNo;
        const std::string line = util::trim(raw);  // retire aussi un eventuel '\r'
        if (line.empty() || line[0] == '#') continue;
        Fields fields = util::split(line, '|');
        for (auto& x : fields) x = util::trim(x);
        try {
            parseRecord(venue, fields);
        } catch (const std::invalid_argument& e) {
            throw DataError(path, lineNo, e.what());
        }
    }
    if (in.bad()) throw DataError(path, lineNo, "erreur de lecture du fichier");
    return venue;
}

void save(const Venue& venue, const std::string& path) {
    const std::string tmp = path + ".tmp";
    {
        std::ofstream out(tmp, std::ios::trunc);
        if (!out) throw std::runtime_error("impossible d'ecrire " + tmp);
        out << "# Lieu culturel - donnees (ROOM, EVENT, SCHEDULED ; separateur '|')\n";
        for (const auto& r : venue.rooms())
            out << "ROOM|" << r.first << '|' << r.second.name << '|' << r.second.capacity << '\n';
        for (const auto& e : venue.events()) {
            out << "EVENT|" << e.first << '|' << e.second->typeKey() << '|' << e.second->title() << '|'
                << e.second->durationMinutes() << '|' << std::fixed << std::setprecision(2) << e.second->price();
            for (const auto& f : e.second->extraFields()) out << '|' << f;
            out << '\n';
        }
        for (const auto& s : venue.scheduledEvents())
            out << "SCHEDULED|" << s.first << '|' << s.second.eventId << '|' << s.second.roomId << '|'
                << s.second.date.toString() << '|' << timeofday::toString(s.second.startMinutes) << '|'
                << s.second.quota << '|' << s.second.reservations << '\n';
        out.flush();
        if (!out) throw std::runtime_error("erreur pendant l'ecriture de " + tmp);
    }
    std::error_code ec;
    std::filesystem::rename(tmp, path, ec);
    if (ec) throw std::runtime_error("impossible de remplacer " + path + " : " + ec.message());
}

}  // namespace storage
