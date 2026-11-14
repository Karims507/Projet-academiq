#include <iostream>

#include "Console.hpp"
#include "Storage.hpp"
int main(int argc, char** argv) {
    const std::string path = argc > 1 ? argv[1] : "lieu_data.txt";
    Venue venue;
    try {
        if (storage::exists(path)) {
            venue = storage::load(path);
            std::cout << "Data loaded from " << path << ": " << venue.rooms().size() << " room(s), "
                      << venue.events().size() << " event(s), " << venue.scheduledEvents().size() << " session(s).\n";
        } else {
            std::cout << "File " << path << " is missing: starting with an empty schedule.\n";
        }
    } catch (const DataError& e) {
        std::cerr << "ERROR IN THE DATA FILE - loading cancelled.\n  " << e.what()
                  << "\nFix the file and restart the application. Exit.\n";
        return 2;  // no partial schedule is used
    }
    Console console(venue, path);
    console.run();
    return 0;
}
