#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Evenement abstrait (diagramme du cours) : title_, durationMinutes_, price_.
class Event {
public:
    Event(std::string title, int durationMinutes, double price)
        : title_(std::move(title)), durationMinutes_(durationMinutes), price_(price) {}
    virtual ~Event() = default;

    const std::string& title() const { return title_; }
    int durationMinutes() const { return durationMinutes_; }
    double price() const { return price_; }  // prix d'une place

    virtual std::string typeKey() const = 0;                       // cle stockee dans le fichier ("SCREENING")
    virtual std::string typeLabel() const = 0;                     // libelle affiche (ex. "Projection")
    virtual std::string details() const = 0;                       // presentation au public
    virtual std::vector<std::string> extraFields() const = 0;      // donnees propres, pour la sauvegarde
    virtual int preparationMinutes() const { return 0; }           // temps d'occupation de la salle avant le debut

private:
    std::string title_;
    int durationMinutes_;
    double price_;
};

// Description d'un type d'evenement : tout ce qu'il faut pour le saisir, le valider et le recharger.
struct EventType {
    std::string key;
    std::string label;
    std::vector<std::string> fieldPrompts;  // une question par donnee propre au type
    // Leve std::invalid_argument avec un message precis si les champs sont invalides.
    std::function<std::unique_ptr<Event>(const std::string& title, int durationMinutes, double price,
                                         const std::vector<std::string>& fields)>
        create;
};

// Registre des types : un nouveau type s'enregistre lui-meme, sans toucher au reste du code.
class EventRegistry {
public:
    static EventRegistry& instance();

    bool add(EventType type);
    const std::map<std::string, EventType>& all() const { return types_; }

    // Point d'entree unique de creation (saisie clavier ET lecture de fichier) : memes validations.
    std::unique_ptr<Event> create(const std::string& key, const std::string& title, int durationMinutes,
                                  double price, const std::vector<std::string>& fields) const;

private:
    std::map<std::string, EventType> types_;
};
