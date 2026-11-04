#pragma once
#include <stdexcept>
#include <string>

#include "Venue.hpp"

// Erreur de données : indique le fichier, la ligne et la nature du problème.
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
// Charge TOUT ou RIEN : lève DataError à la première incohérence, aucun Venue partiel n'est renvoyé.
Venue load(const std::string& path);
// Écrit dans un fichier temporaire puis le renomme (un crash ne détruit pas l'ancien fichier).
void save(const Venue& venue, const std::string& path);
}  // namespace storage
