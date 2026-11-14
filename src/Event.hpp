#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Abstract event (course diagram): title_, durationMinutes_, price_.
class Event {
public:
    Event(std::string title, int durationMinutes, double price)
        : title_(std::move(title)), durationMinutes_(durationMinutes), price_(price) {}
    virtual ~Event() = default;

    const std::string& title() const { return title_; }
    int durationMinutes() const { return durationMinutes_; }
    double price() const { return price_; }  // ticket price

    virtual std::string typeKey() const = 0;                       // key stored in the file ("SCREENING")
    virtual std::string typeLabel() const = 0;                     // label displayed (e.g. "Projection")
    virtual std::string details() const = 0;                       // public presentation
    virtual std::vector<std::string> extraFields() const = 0;      // custom fields for saving
    virtual int preparationMinutes() const { return 0; }           // room occupancy time before start

private:
    std::string title_;
    int durationMinutes_;
    double price_;
};

// Description of an event type: everything needed to enter, validate and reload it.
struct EventType {
    std::string key;
    std::string label;
    std::vector<std::string> fieldPrompts;  // one question per custom field of the type
    // Throws std::invalid_argument with a precise message if fields are invalid.
    std::function<std::unique_ptr<Event>(const std::string& title, int durationMinutes, double price,
                                         const std::vector<std::string>& fields)>
        create;
};

// Registry of types: a new type registers itself without touching the rest of the code.
class EventRegistry {
public:
    static EventRegistry& instance();

    bool add(EventType type);
    const std::map<std::string, EventType>& all() const { return types_; }

    // Single entry point for creation (keyboard input AND file reading): same validations.
    std::unique_ptr<Event> create(const std::string& key, const std::string& title, int durationMinutes,
                                  double price, const std::vector<std::string>& fields) const;

private:
    std::map<std::string, EventType> types_;
};
