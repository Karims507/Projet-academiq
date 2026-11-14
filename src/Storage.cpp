#include "Storage.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>

#include "Util.hpp"

namespace {

using Fields = std::vector<std::string>;

void requireCount(const Fields& f, std::size_t expected, const std::string& usage) {
    if (f.size() != expected)
        throw std::invalid_argument("line " + f[0] + " must have " + std::to_string(expected) +
                                    " fields separated by '|' (" + usage + "), found " + std::to_string(f.size()));
}

int needInt(const Fields& f, std::size_t i, const std::string& name) {
    const auto v = util::parseInt(f[i]);
    if (!v)
        throw std::invalid_argument("field n°" + std::to_string(i + 1) + " (" + name + ") is invalid: '" + f[i] +
                                    "' (integer expected)");
    return *v;
}

void check(const Result& r) {
    if (!r.ok) throw std::invalid_argument(r.message);
}

void parseRecord(Venue& venue, const Fields& f) {
    const std::string& kind = f[0];
    if (kind == "ROOM") {
        requireCount(f, 4, "ROOM|id|name|capacity");
        check(venue.addRoom(Room{needInt(f, 1, "id"), f[2], needInt(f, 3, "capacity")}));
    } else if (kind == "EVENT") {
        if (f.size() < 6)
            throw std::invalid_argument(
                "EVENT line must contain at least 6 fields (EVENT|id|type|title|duration|price|...)");
        const int id = needInt(f, 1, "id");
        const int duration = needInt(f, 4, "duration");
        const auto price = util::parseDouble(f[5]);
        if (!price) throw std::invalid_argument("field n°6 (price) is invalid: '" + f[5] + "' (number expected)");
        std::unique_ptr<Event> ev =
            EventRegistry::instance().create(f[2], f[3], duration, *price, Fields(f.begin() + 6, f.end()));
        check(venue.addEvent(id, std::move(ev)));
    } else if (kind == "SCHEDULED") {
        requireCount(f, 8, "SCHEDULED|id|event|room|date|time|quota|reservations");
        ScheduledEvent s;
        s.id = needInt(f, 1, "id");
        s.eventId = needInt(f, 2, "event");
        s.roomId = needInt(f, 3, "room");
        const auto date = Date::parse(f[4]);
        if (!date) throw std::invalid_argument("field n°5 (date) is invalid: '" + f[4] + "' (format YYYY-MM-DD)");
        s.date = *date;
        const auto time = timeofday::parse(f[5]);
        if (!time) throw std::invalid_argument("field n°6 (time) is invalid: '" + f[5] + "' (format HH:MM)");
        s.startMinutes = *time;
        s.quota = needInt(f, 6, "quota");
        s.reservations = needInt(f, 7, "reservations");
        if (!venue.findEvent(s.eventId))
            throw std::invalid_argument("event " + std::to_string(s.eventId) +
                                        " is unknown (EVENT lines must come before SCHEDULED lines)");
        if (!venue.findRoom(s.roomId))
            throw std::invalid_argument("room " + std::to_string(s.roomId) +
                                        " is unknown (ROOM lines must come before SCHEDULED lines)");
        check(venue.addScheduledEvent(s));
    } else {
        throw std::invalid_argument("unknown record type '" + kind + "' (expected: ROOM, EVENT or SCHEDULED)");
    }
}

}  // namespace

namespace storage {

bool exists(const std::string& path) { return std::filesystem::exists(path); }

Venue load(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw DataError(path, 0, "unable to open the file");
    Venue venue;
    std::string raw;
    int lineNo = 0;
    while (std::getline(in, raw)) {
        ++lineNo;
        const std::string line = util::trim(raw);  // removes a possible '\r' too
        if (line.empty() || line[0] == '#') continue;
        Fields fields = util::split(line, '|');
        for (auto& x : fields) x = util::trim(x);
        try {
            parseRecord(venue, fields);
        } catch (const std::invalid_argument& e) {
            throw DataError(path, lineNo, e.what());
        }
    }
    if (in.bad()) throw DataError(path, lineNo, "file read error");
    return venue;
}

void save(const Venue& venue, const std::string& path) {
    const std::string tmp = path + ".tmp";
    {
        std::ofstream out(tmp, std::ios::trunc);
        if (!out) throw std::runtime_error("unable to write " + tmp);
        out << "# Cultural venue data (ROOM, EVENT, SCHEDULED; separator '|')\n";
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
        if (!out) throw std::runtime_error("error while writing " + tmp);
    }
    std::error_code ec;
    std::filesystem::rename(tmp, path, ec);
    if (ec) throw std::runtime_error("unable to replace " + path + ": " + ec.message());
}

}  // namespace storage
