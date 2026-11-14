#pragma once
#include <stdexcept>
#include <string>

#include "Venue.hpp"

// Data error: indicates the file, line and nature of the problem.
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
// Loads ALL OR NOTHING: raises DataError at the first inconsistency, no partial Venue is returned.
Venue load(const std::string& path);
// Writes to a temporary file and then renames it (a crash does not destroy the old file).
void save(const Venue& venue, const std::string& path);
}  // namespace storage
