#pragma once
#include <stdexcept>
#include <string>

#include "Venue.hpp"

// Erreur de donnees : indique le fichier, la ligne et la nature du probleme.
class DataError : public std::runtime_error {
public:
    DataError(const std::string& file, int line, const std::string& message)
        : std::runtime_error(file + (line > 0 ? ":" + std::to_string(line) : "") + ": " + message),
          file_(file), line_(line) {}
    const std::string& file() const { return file_; }
    int line() const { return line_; }

private:
    std::string file_;
    int line_;
};

namespace storage {
bool exists(const std::string& path);
// Charge TOUT ou RIEN : leve DataError a la premiere incoherence, aucun Venue partiel n'est renvoye.
Venue load(const std::string& path);
// Ecrit dans un fichier temporaire puis le renomme (un crash ne detruit pas l'ancien fichier).
void save(const Venue& venue, const std::string& path);
}  // namespace storage
