#include <iostream>

#include "Console.hpp"
#include "Storage.hpp"

int main(int argc, char** argv) {
    const std::string path = argc > 1 ? argv[1] : "lieu_data.txt";
    Venue venue;
    try {
        if (storage::exists(path)) {
            venue = storage::load(path);
            std::cout << "Donnees chargees depuis " << path << " : " << venue.rooms().size() << " salle(s), "
                      << venue.events().size() << " evenement(s), " << venue.scheduledEvents().size() << " seance(s).\n";
        } else {
            std::cout << "Fichier " << path << " absent : demarrage avec une programmation vide.\n";
        }
    } catch (const DataError& e) {
        std::cerr << "ERREUR DANS LE FICHIER DE DONNEES - chargement annuler.\n  " << e.what()
                  << "\nCorrigez le fichier puis relancez l'application. Arrêt.\n";
        return 2;  // aucune programmation partielle n'est utilisée
    }
    Console console(venue, path);
    console.run();
    return 0;
}
