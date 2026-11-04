#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Événement abstrait (diagramme du cours) : title_, durationMinutes_, price_.
class Event {
public:
    Event(std::string title, int durationMinutes, double price)
        : title_(std::move(title)), durationMinutes_(durationMinutes), price_(price) {}
    virtual ~Event() = default;

    const std::string& title() const { return title_; }
    int durationMinutes() const { return durationMinutes_; }
    double price() const { return price_; }  // prix d'une place

    virtual std::string typeKey() const = 0;                       // clé stockée dans le fichier ("SCREENING")
    virtual std::string typeLabel() const = 0;                     // libellé affiché (ex. "Projection")
    virtual std::string details() const = 0;                       // présentation au public
    virtual std::vector<std::string> extraFields() const = 0;      // données propres, pour la sauvegarde
    virtual int preparationMinutes() const { return 0; }           // temps d'occupation de la salle avant le début

private:
    std::string title_;
    int durationMinutes_;
    double price_;
};

// Description d'un type d'événement : tout ce qu'il faut pour le saisir, le valider et le recharger.
struct EventType {
    std::string key;
    std::string label;
    std::vector<std::string> fieldPrompts;  // une question par donnée propre au type
    // Lève std::invalid_argument avec un message précis si les champs sont invalides.
    std::function<std::unique_ptr<Event>(const std::string& title, int durationMinutes, double price,
                                         const std::vector<std::string>& fields)>
        create;
};

// Registre des types : un nouveau type s'enregistre lui-même, sans toucher au reste du code.
class EventRegistry {
public:
    static EventRegistry& instance();

    bool add(EventType type);
    const std::map<std::string, EventType>& all() const { return types_; }

    // Point d'entrée unique de création (saisie clavier ET lecture de fichier) : mêmes validations.
    std::unique_ptr<Event> create(const std::string& key, const std::string& title, int durationMinutes,
                                  double price, const std::vector<std::string>& fields) const;

private:
    std::map<std::string, EventType> types_;
};
