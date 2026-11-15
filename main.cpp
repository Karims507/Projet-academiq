#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct Session
{
    std::string type, title, date, time;
    int room, duration, quota, booked;
    double price;
};

void save(const std::vector<Session>& sessions)
{
    std::ofstream file("events_data.txt");

    for (const auto& s : sessions)
    {
        file << s.type << " "
             << s.title << " "
             << s.date << " "
             << s.time << " "
             << s.room << " "
             << s.duration << " "
             << s.price << " "
             << s.quota << " "
             << s.booked << "\n";
    }
}

bool load(std::vector<Session>& sessions)
{
    std::ifstream file("events_data.txt");
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream data(line);
        Session s;

        if (!(data >> s.type
                   >> s.title
                   >> s.date
                   >> s.time
                   >> s.room
                   >> s.duration
                   >> s.price
                   >> s.quota
                   >> s.booked))
        {
            std::cout << "File error\n";
            return false;
        }

        sessions.push_back(s);
    }

    return true;
}

bool conflict(const std::vector<Session>& sessions, Session newSession)
{
    for (const auto& s : sessions)
    {
        if (s.room == newSession.room &&
            s.date == newSession.date &&
            s.time == newSession.time)
        {
            return true;
        }
    }

    return false;
}

void displaySessions(const std::vector<Session>& sessions)
{
    int i = 1;

    for (const auto& s : sessions)
    {
        std::cout << i << ". "
                  << s.title << " | "
                  << s.date << " | "
                  << s.time << " | Room "
                  << s.room << " | "
                  << s.booked << "/"
                  << s.quota << "\n";

        i++;
    }
}

void programming(const std::vector<Session>& sessions)
{
    std::string date;

    std::cout << "Date: ";
    std::cin >> date;

    std::cout << "\nProgramming for " << date << "\n";

    for (const auto& s : sessions)
    {
        if (s.date == date)
        {
            std::cout << s.time << " - "
                      << s.title << " - Room "
                      << s.room << "\n";
        }
    }
}

void book(std::vector<Session>& sessions)
{
    int number, quantity;

    displaySessions(sessions);

    std::cout << "Session number: ";
    std::cin >> number;

    if (number < 1 || number > sessions.size())
    {
        std::cout << "Invalid session\n";
        return;
    }

    std::cout << "Number of seats: ";
    std::cin >> quantity;

    Session& s = sessions[number - 1];

    if (quantity <= 0)
    {
        std::cout << "Invalid quantity\n";
    }

    else if (s.booked + quantity > s.quota)
    {
        std::cout << "Not enough seats\n";
    }

    else
    {
        s.booked += quantity;

        std::cout << "Booking accepted\n";

        save(sessions);
    }
}

void addEvent(std::vector<Session>& sessions)
{
    Session s;
    int type;

    std::cout << "1. Screening\n";
    std::cout << "2. Concert\n";
    std::cout << "3. Play\n";
    std::cin >> type;

    if (type == 1)
    {
        s.type = "Screening";
    }
    else if (type == 2)
    {
        s.type = "Concert";
    }
    else if (type == 3)
    {
        s.type = "Play";
    }
    else
    {
        return;
    }

    std::cout << "Title: ";
    std::cin >> s.title;

    std::cout << "Date: ";
    std::cin >> s.date;

    std::cout << "Time: ";
    std::cin >> s.time;

    std::cout << "Duration: ";
    std::cin >> s.duration;

    std::cout << "Price: ";
    std::cin >> s.price;

    std::cout << "Room: ";
    std::cin >> s.room;

    std::cout << "Places: ";
    std::cin >> s.quota;

    s.booked = 0;

    if (s.quota > 100)
    {
        std::cout << "Too many places\n";
        return;
    }

    if (conflict(sessions, s))
    {
        std::cout << "Room already occupied\n";
        return;
    }

    sessions.push_back(s);

    save(sessions);

    std::cout << "Event added\n";
}

int main()
{
    std::vector<Session> sessions;

    if (!load(sessions))
    {
        return 0;
    }

    int choice = -1;

    while (choice != 0)
    {
        std::cout << "\n1. User\n";
        std::cout << "2. Admin\n";
        std::cout << "0. Exit\n";
        std::cin >> choice;

        if (choice == 1)
        {
            int userChoice;

            std::cout << "1. Display sessions\n";
            std::cout << "2. Book seats\n";
            std::cout << "3. Programming by date\n";
            std::cin >> userChoice;

            if (userChoice == 1)
            {
                displaySessions(sessions);
            }

            else if (userChoice == 2)
            {
                book(sessions);
            }

            else if (userChoice == 3)
            {
                programming(sessions);
            }
        }

        else if (choice == 2)
        {
            std::string code;

            std::cout << "Admin code: ";
            std::cin >> code;

            if (code == "ABCD")
            {
                addEvent(sessions);
            }
            else
            {
                std::cout << "Wrong code\n";
            }
        }
    }

    return 0;
}