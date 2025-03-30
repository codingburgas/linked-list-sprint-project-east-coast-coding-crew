#include "cli_manager.hpp"
#include "utilities.hpp"
#include <iostream>
#include <format>
#include <ctime>
#include <iomanip>

namespace Eccc {
namespace Core {

CliManager::CliManager(std::shared_ptr<Database> db) : dbManager(db) {
}

void CliManager::displayLogo() {
    std::cout << "\n";
    std::cout << CYAN << " _____          _   _____      _       _            " << RESET << "\n";
    std::cout << CYAN << "|  __ \\        | | |  __ \\    (_)     | |           " << RESET << "\n";
    std::cout << CYAN << "| |__) |_ _ ___| |_| |__) |___ _ _ __ | |_ ___ _ __ " << RESET << "\n";
    std::cout << GREEN << "|  ___/ _` / __| __|  ___/ _ \\ | '_ \\| __/ _ \\ '__|" << RESET << "\n";
    std::cout << GREEN << "| |  | (_| \\__ \\ |_| |   | (_) | | | | ||  __/ |   " << RESET << "\n";
    std::cout << GREEN << "|_|   \\__,_|___/\\__|_|    \\___/|_| |_|\\__\\___|_|   " << RESET << "\n";
    std::cout << "\n";
    std::cout << MAGENTA << "               Historical Events Database" << RESET << "\n";
    std::cout << YELLOW << "                     v1.0.0" << RESET << "\n\n";
}

void CliManager::displayCommandHelp() {
    std::cout << BOLD << "\nCommands:" << RESET << "\n";
    std::cout << GREEN << "> event add" << RESET << "            Add a new historical event\n";
    std::cout << GREEN << "> event list" << RESET << "           View all events\n";
    std::cout << GREEN << "> event find <id>" << RESET << "      Find event by ID\n";
    std::cout << GREEN << "> event category <n>" << RESET << " Find events by category\n";
    std::cout << GREEN << "> event location <n>" << RESET << " Find events by location\n";
    std::cout << GREEN << "> event sort-date" << RESET << "      Sort events by date\n";
    std::cout << GREEN << "> event sort-title" << RESET << "     Sort events by title\n";
    std::cout << GREEN << "> event delete <id>" << RESET << "    Delete an event\n";
    std::cout << GREEN << "> event update <id>" << RESET << "    Update an event\n";
    std::cout << GREEN << "> sample" << RESET << "               Add sample data\n";
    std::cout << GREEN << "> help" << RESET << "                 View all commands\n";
    std::cout << GREEN << "> exit" << RESET << "                 Exit program\n";
}

void CliManager::run() {
    std::cout << "\033[2J\033[1;1H";
    displayLogo();

    auto schemaResult = dbManager.setupSchema();
    if (!schemaResult) {
        std::cerr << RED << "✗ Failed to setup database schema: " << schemaResult.error() << RESET << "\n";
        return;
    }

    std::cout << GREEN << "✓ " << RESET << "Database schema initialized\n";

    displayCommandHelp();
    bool running = true;

    while (running) {
        std::cout << "\n" << BOLD << YELLOW << "hist-cli" << RESET << " " << BLUE << "$ " << RESET;
        std::string input;
        std::getline(std::cin, input);

        auto [command, args] = Eccc::Core::parseCommand(input);

        if (command == "exit" || command == "quit") {
            std::cout << "Exiting program.\n";
            running = false;
        }
        else if (command == "help") {
            displayCommandHelp();
        }
        else if (command == "sample") {
            addSampleData();
        }
        else if (command == "event") {
            handleEventCommand(args);
        }
        else if (!command.empty()) {
            std::cout << RED << "✗ " << RESET << "Unknown command: " << command << ". Try 'help' for a list of commands.\n";
        }
    }
}

void CliManager::addSampleData() {
    HistoricalEvent event1;
    event1.title = "Declaration of Independence";
    event1.description = "The United States Declaration of Independence was adopted by the Second Continental Congress.";
    event1.location = "Philadelphia, Pennsylvania";
    event1.date = createDate(1776, 7, 4);
    event1.category = "Political";
    event1.significance = 10;

    HistoricalEvent event2;
    event2.title = "Moon Landing";
    event2.description = "Apollo 11 was the spaceflight that first landed humans on the Moon.";
    event2.location = "Moon";
    event2.date = createDate(1969, 7, 20);
    event2.category = "Scientific";
    event2.significance = 9;

    HistoricalEvent event3;
    event3.title = "World Wide Web Invention";
    event3.description = "Tim Berners-Lee invented the World Wide Web while working at CERN.";
    event3.location = "CERN, Switzerland";
    event3.date = createDate(1989, 3, 12);
    event3.category = "Technology";
    event3.significance = 10;

    HistoricalEvent event4;
    event4.title = "Fall of the Berlin Wall";
    event4.description = "The fall of the Berlin Wall, which had separated East and West Berlin since 1961.";
    event4.location = "Berlin, Germany";
    event4.date = createDate(1989, 11, 9);
    event4.category = "Political";
    event4.significance = 8;

    HistoricalEvent event5;
    event5.title = "First Powered Flight";
    event5.description = "The Wright brothers made the first controlled, sustained flight of a powered, heavier-than-air aircraft.";
    event5.location = "Kitty Hawk, North Carolina";
    event5.date = createDate(1903, 12, 17);
    event5.category = "Technology";
    event5.significance = 9;

    auto result1 = dbManager.createEvent(event1);
    auto result2 = dbManager.createEvent(event2);
    auto result3 = dbManager.createEvent(event3);
    auto result4 = dbManager.createEvent(event4);
    auto result5 = dbManager.createEvent(event5);

    if (result1 && result2 && result3 && result4 && result5) {
        std::cout << GREEN << "✓ " << RESET << "Sample data added successfully.\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error adding sample data.\n";
    }
}

HistoricalEvent CliManager::inputEventDetails() {
    HistoricalEvent event;

    std::cout << CYAN << "Enter event title: " << RESET;
    std::getline(std::cin, event.title);

    std::cout << CYAN << "Enter event description: " << RESET;
    std::getline(std::cin, event.description);

    std::cout << CYAN << "Enter event location: " << RESET;
    std::getline(std::cin, event.location);

    int year, month, day;
    std::cout << CYAN << "Enter event date (YYYY MM DD): " << RESET;
    std::cin >> year >> month >> day;
    std::cin.ignore();
    event.date = createDate(year, month, day);

    std::cout << CYAN << "Enter event category: " << RESET;
    std::getline(std::cin, event.category);

    std::cout << CYAN << "Enter event significance (1-10): " << RESET;
    std::cin >> event.significance;
    std::cin.ignore();

    return event;
}

void CliManager::handleEventCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "✗ " << RESET << "Missing event subcommand. Try 'help' for a list of commands.\n";
        return;
    }

    std::string subCommand = args[0];

    if (subCommand == "add") {
        handleAddEvent();
    }
    else if (subCommand == "list") {
        handleListEvents();
    }
    else if (subCommand == "find") {
        handleFindEvent(args);
    }
    else if (subCommand == "category") {
        handleCategorySearch(args);
    }
    else if (subCommand == "location") {
        handleLocationSearch(args);
    }
    else if (subCommand == "sort-date") {
        handleSortByDate();
    }
    else if (subCommand == "sort-title") {
        handleSortByTitle();
    }
    else if (subCommand == "delete") {
        handleDeleteEvent(args);
    }
    else if (subCommand == "update") {
        handleUpdateEvent(args);
    }
    else {
        std::cout << RED << "✗ " << RESET << "Unknown event subcommand: " << subCommand << ". Try 'help' for a list of commands.\n";
    }
}

void CliManager::handleAddEvent() {
    HistoricalEvent newEvent = inputEventDetails();
    auto result = dbManager.createEvent(newEvent);

    if (result) {
        std::cout << GREEN << "✓ " << RESET << "Event added successfully with ID: " << result.value() << "\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error adding event: " << result.error() << "\n";
    }
}

void CliManager::handleListEvents() {
    auto result = dbManager.loadEventsToLinkedList();

    if (result) {
        HistoricalLinkedList& list = result.value();
        if (list.isEmpty()) {
            std::cout << YELLOW << "! " << RESET << "No events found in the database.\n";
        } else {
            std::cout << CYAN << "Total events: " << list.getSize() << RESET << "\n";
            list.display();
        }
    } else {
        std::cout << RED << "✗ " << RESET << "Error loading events: " << result.error() << "\n";
    }
}

void CliManager::handleFindEvent(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "✗ " << RESET << "Missing event ID. Usage: event find <id>\n";
        return;
    }

    int id;
    try {
        id = std::stoi(args[1]);
    } catch (...) {
        std::cout << RED << "✗ " << RESET << "Invalid ID. Please enter a number.\n";
        return;
    }

    auto result = dbManager.readEvent(id);

    if (result) {
        const HistoricalEvent& event = result.value();

        std::cout << BOLD << "ID: " << RESET << event.id << "\n";
        std::cout << BOLD << "Title: " << RESET << event.title << "\n";
        std::cout << BOLD << "Description: " << RESET << event.description << "\n";
        std::cout << BOLD << "Location: " << RESET << event.location << "\n";

        char timeBuffer[80];
        struct tm* timeInfo = localtime(&event.date);
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d", timeInfo);

        std::cout << BOLD << "Date: " << RESET << timeBuffer << "\n";
        std::cout << BOLD << "Category: " << RESET << event.category << "\n";
        std::cout << BOLD << "Significance: " << RESET << event.significance << "\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error finding event: " << result.error() << "\n";
    }
}

void CliManager::handleCategorySearch(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "✗ " << RESET << "Missing category name. Usage: event category <n>\n";
        return;
    }

    std::string category = args[1];
    auto result = dbManager.getEventsByCategory(category);

    if (result) {
        const std::vector<HistoricalEvent>& events = result.value();

        if (events.empty()) {
            std::cout << YELLOW << "! " << RESET << "No events found in the category '" << category << "'.\n";
        } else {
            std::cout << CYAN << "Found " << events.size() << " events in category '" << category << "':" << RESET << "\n";

            for (const auto& event : events) {
                std::cout << "  " << BOLD << event.id << RESET << " - " << event.title << "\n";
            }
        }
    } else {
        std::cout << RED << "✗ " << RESET << "Error finding events: " << result.error() << "\n";
    }
}

void CliManager::handleLocationSearch(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "✗ " << RESET << "Missing location name. Usage: event location <n>\n";
        return;
    }

    std::string location = args[1];
    auto result = dbManager.getEventsByLocation(location);

    if (result) {
        const std::vector<HistoricalEvent>& events = result.value();

        if (events.empty()) {
            std::cout << YELLOW << "! " << RESET << "No events found at location '" << location << "'.\n";
        } else {
            std::cout << CYAN << "Found " << events.size() << " events at location '" << location << "':" << RESET << "\n";

            for (const auto& event : events) {
                std::cout << "  " << BOLD << event.id << RESET << " - " << event.title << "\n";
            }
        }
    } else {
        std::cout << RED << "✗ " << RESET << "Error finding events: " << result.error() << "\n";
    }
}

void CliManager::handleSortByDate() {
    auto result = dbManager.loadEventsToLinkedList();

    if (result) {
        HistoricalLinkedList& list = result.value();

        if (list.isEmpty()) {
            std::cout << YELLOW << "! " << RESET << "No events found in the database.\n";
        } else {
            list.sortByDate();
            std::cout << GREEN << "✓ " << RESET << "Events sorted by date:\n";
            list.display();

            auto saveResult = dbManager.saveLinkedListToDb(list);
            if (!saveResult) {
                std::cout << RED << "✗ " << RESET << "Error saving sorted list: " << saveResult.error() << "\n";
            }
        }
    } else {
        std::cout << RED << "✗ " << RESET << "Error loading events: " << result.error() << "\n";
    }
}

void CliManager::handleSortByTitle() {
    auto result = dbManager.loadEventsToLinkedList();

    if (result) {
        HistoricalLinkedList& list = result.value();

        if (list.isEmpty()) {
            std::cout << YELLOW << "! " << RESET << "No events found in the database.\n";
        } else {
            list.sortByTitle();
            std::cout << GREEN << "✓ " << RESET << "Events sorted by title:\n";
            list.display();

            auto saveResult = dbManager.saveLinkedListToDb(list);
            if (!saveResult) {
                std::cout << RED << "✗ " << RESET << "Error saving sorted list: " << saveResult.error() << "\n";
            }
        }
    } else {
        std::cout << RED << "✗ " << RESET << "Error loading events: " << result.error() << "\n";
    }
}

void CliManager::handleDeleteEvent(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "✗ " << RESET << "Missing event ID. Usage: event delete <id>\n";
        return;
    }

    int id;
    try {
        id = std::stoi(args[1]);
    } catch (...) {
        std::cout << RED << "✗ " << RESET << "Invalid ID. Please enter a number.\n";
        return;
    }

    auto result = dbManager.deleteEvent(id);

    if (result) {
        std::cout << GREEN << "✓ " << RESET << "Event deleted successfully.\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error deleting event: " << result.error() << "\n";
    }
}

void CliManager::handleUpdateEvent(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "✗ " << RESET << "Missing event ID. Usage: event update <id>\n";
        return;
    }

    int id;
    try {
        id = std::stoi(args[1]);
    } catch (...) {
        std::cout << RED << "✗ " << RESET << "Invalid ID. Please enter a number.\n";
        return;
    }

    auto existingEventResult = dbManager.readEvent(id);

    if (!existingEventResult) {
        std::cout << RED << "✗ " << RESET << "Error: " << existingEventResult.error() << "\n";
        return;
    }

    std::cout << CYAN << "Updating event with ID " << id << ":" << RESET << "\n";
    HistoricalEvent updatedEvent = inputEventDetails();
    updatedEvent.id = id;

    auto result = dbManager.updateEvent(updatedEvent);

    if (result) {
        std::cout << GREEN << "✓ " << RESET << "Event updated successfully.\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error updating event: " << result.error() << "\n";
    }
}

}
}
