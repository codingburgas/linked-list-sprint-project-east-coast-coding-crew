#include "cli_manager.hpp"
#include "utilities.hpp"
#include <iostream>
#include <format>
#include <ctime>
#include <iomanip>
#include <set>
#include <fstream>
#include <nlohmann/json.hpp>

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
    std::cout << GREEN << "> event add -f" << RESET << "         Add first event as first node\n";
    std::cout << GREEN << "> event add -b" << RESET << "         Add event at beginning of list\n";
    std::cout << GREEN << "> event add -d" << RESET << "         Add event based on date order\n";
    std::cout << GREEN << "> event list" << RESET << "           View all events\n";
    std::cout << GREEN << "> event list -d MM/DD" << RESET << "  Find events by month/day\n";
    std::cout << GREEN << "> event list -y YYYY" << RESET << "   Find events by year\n";
    std::cout << GREEN << "> event list -c TEXT" << RESET << "   Find events by category (partial match)\n";
    std::cout << GREEN << "> event list -l TEXT" << RESET << "   Find events by location (partial match)\n";
    std::cout << GREEN << "> event list id ID" << RESET << "     Find event by ID\n";
    std::cout << GREEN << "> event sort-date" << RESET << "      Sort events by date\n";
    std::cout << GREEN << "> event sort-title" << RESET << "     Sort events by title\n";
    std::cout << GREEN << "> event delete <id>" << RESET << "    Delete an event\n";
    std::cout << GREEN << "> event update <id>" << RESET << "    Update an event\n";
    std::cout << GREEN << "> export" << RESET << "               Export events with options:\n";
    std::cout << GREEN << "    -t" << RESET << "                 Export titles\n";
    std::cout << GREEN << "    -c [category]" << RESET << "      Export all categories or specific category\n";
    std::cout << GREEN << "    -r [result]" << RESET << "        Export all results or specific result\n";
    std::cout << GREEN << "    -txt/-csv/-json" << RESET << "    Export format\n";
    std::cout << GREEN << "    -o <filename>" << RESET << "      Output filename\n";
    std::cout << GREEN << "> sample" << RESET << "               Add sample data\n";
    std::cout << GREEN << "> help" << RESET << "                 View all commands\n";
    std::cout << GREEN << "> exit" << RESET << "                 Exit program\n";
    std::cout << "\n" << BOLD << "Export formats:" << RESET << " txt, csv, json\n";
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
        else if (command == "export") {
            handleExportCommand(args);
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
    event1.leader = "Thomas Jefferson";
    event1.participants = "Continental Congress, 13 American colonies";
    event1.result = "Formation of the United States of America";

    HistoricalEvent event2;
    event2.title = "Moon Landing";
    event2.description = "Apollo 11 was the spaceflight that first landed humans on the Moon.";
    event2.location = "Moon";
    event2.date = createDate(1969, 7, 20);
    event2.category = "Scientific";
    event2.significance = 9;
    event2.leader = "Neil Armstrong";
    event2.participants = "NASA, Neil Armstrong, Buzz Aldrin, Michael Collins";
    event2.result = "First humans on the Moon";

    HistoricalEvent event3;
    event3.title = "World Wide Web Invention";
    event3.description = "Tim Berners-Lee invented the World Wide Web while working at CERN.";
    event3.location = "CERN, Switzerland";
    event3.date = createDate(1989, 3, 12);
    event3.category = "Technology";
    event3.significance = 10;
    event3.leader = "Tim Berners-Lee";
    event3.participants = "CERN researchers";
    event3.result = "Creation of the World Wide Web";

    HistoricalEvent event4;
    event4.title = "Fall of the Berlin Wall";
    event4.description = "The fall of the Berlin Wall, which had separated East and West Berlin since 1961.";
    event4.location = "Berlin, Germany";
    event4.date = createDate(1989, 11, 9);
    event4.category = "Political";
    event4.significance = 8;
    event4.leader = "Civil protesters";
    event4.participants = "East and West German citizens";
    event4.result = "Reunification of Germany";

    HistoricalEvent event5;
    event5.title = "First Powered Flight";
    event5.description = "The Wright brothers made the first controlled, sustained flight of a powered, heavier-than-air aircraft.";
    event5.location = "Kitty Hawk, North Carolina";
    event5.date = createDate(1903, 12, 17);
    event5.category = "Technology";
    event5.significance = 9;
    event5.leader = "Orville and Wilbur Wright";
    event5.participants = "Wright brothers";
    event5.result = "Beginning of the aviation era";

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

    std::cout << CYAN << "Enter event leader: " << RESET;
    std::getline(std::cin, event.leader);

    std::cout << CYAN << "Enter event participants: " << RESET;
    std::getline(std::cin, event.participants);

    std::cout << CYAN << "Enter event result: " << RESET;
    std::getline(std::cin, event.result);

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
        if (args.size() > 1) {
            if (args[1] == "-f") {
                handleAddFirstEvent();
            }
            else if (args[1] == "-b") {
                handleAddEventAtBeginning();
            }
            else if (args[1] == "-d") {
                handleAddEventByDate();
            }
            else {
                std::cout << RED << "✗ " << RESET << "Invalid add option. Usage: event add [-f|-b|-d]\n";
            }
        }
        else {
            handleAddEvent();
        }
    }
    else if (subCommand == "list") {
        if (args.size() > 1) {
            if (args[1] == "-d" && args.size() > 2) {
                handleListEventsByDate(args[2]);
            }
            else if (args[1] == "-y" && args.size() > 2) {
                handleListEventsByYear(args[2]);
            }
            else if (args[1] == "-c" && args.size() > 2) {
                handleCategorySearch({args[0], args[2]});
            }
            else if (args[1] == "-l" && args.size() > 2) {
                handleLocationSearch({args[0], args[2]});
            }
            else if (args[1] == "id" && args.size() > 2) {
                try {
                    int id = std::stoi(args[2]);
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
                        std::cout << BOLD << "Leader: " << RESET << event.leader << "\n";
                        std::cout << BOLD << "Participants: " << RESET << event.participants << "\n";
                        std::cout << BOLD << "Result: " << RESET << event.result << "\n";
                    } else {
                        std::cout << RED << "✗ " << RESET << "Error finding event: " << result.error() << "\n";
                    }
                } catch (...) {
                    std::cout << RED << "✗ " << RESET << "Invalid ID. Please enter a number.\n";
                }
            }
            else {
                std::cout << RED << "✗ " << RESET << "Invalid list option. Usage: event list [-d MM/DD | -y YYYY | -c CATEGORY | -l LOCATION | id ID]\n";
                std::cout << "  -c: Search for events with category containing CATEGORY\n";
                std::cout << "  -l: Search for events with location containing LOCATION\n";
            }
        }
        else {
            handleListEvents();
        }
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
        std::cout << BOLD << "Leader: " << RESET << event.leader << "\n";
        std::cout << BOLD << "Participants: " << RESET << event.participants << "\n";
        std::cout << BOLD << "Result: " << RESET << event.result << "\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error finding event: " << result.error() << "\n";
    }
}

void CliManager::handleCategorySearch(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "✗ " << RESET << "Missing category name. Usage: event category <n>\n";
        return;
    }

    std::string categoryQuery = args[1];
    auto eventsResult = dbManager.getAllEvents();

    if (!eventsResult) {
        std::cout << RED << "✗ " << RESET << "Error loading events: " << eventsResult.error() << "\n";
        return;
    }

    std::vector<HistoricalEvent> matchedEvents;
    for (const auto& event : eventsResult.value()) {
        // Case-insensitive partial match
        std::string eventCategory = event.category;
        std::string query = categoryQuery;
        
        // Convert to lowercase for case-insensitive comparison
        std::transform(eventCategory.begin(), eventCategory.end(), eventCategory.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        std::transform(query.begin(), query.end(), query.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        
        if (eventCategory.find(query) != std::string::npos) {
            matchedEvents.push_back(event);
        }
    }

    if (matchedEvents.empty()) {
        std::cout << YELLOW << "! " << RESET << "No events found matching category '" << categoryQuery << "'.\n";
    } else {
        std::cout << CYAN << "Found " << matchedEvents.size() << " events matching category '" << categoryQuery << "':" << RESET << "\n";

        for (const auto& event : matchedEvents) {
            std::cout << "  " << BOLD << event.id << RESET << " - " << event.title << " (" << event.category << ")\n";
        }
    }
}

void CliManager::handleLocationSearch(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "✗ " << RESET << "Missing location name. Usage: event location <n>\n";
        return;
    }

    std::string locationQuery = args[1];
    auto eventsResult = dbManager.getAllEvents();

    if (!eventsResult) {
        std::cout << RED << "✗ " << RESET << "Error loading events: " << eventsResult.error() << "\n";
        return;
    }

    std::vector<HistoricalEvent> matchedEvents;
    for (const auto& event : eventsResult.value()) {
        // Case-insensitive partial match
        std::string eventLocation = event.location;
        std::string query = locationQuery;
        
        // Convert to lowercase for case-insensitive comparison
        std::transform(eventLocation.begin(), eventLocation.end(), eventLocation.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        std::transform(query.begin(), query.end(), query.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        
        if (eventLocation.find(query) != std::string::npos) {
            matchedEvents.push_back(event);
        }
    }

    if (matchedEvents.empty()) {
        std::cout << YELLOW << "! " << RESET << "No events found matching location '" << locationQuery << "'.\n";
    } else {
        std::cout << CYAN << "Found " << matchedEvents.size() << " events matching location '" << locationQuery << "':" << RESET << "\n";

        for (const auto& event : matchedEvents) {
            std::cout << "  " << BOLD << event.id << RESET << " - " << event.title << " (" << event.location << ")\n";
        }
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

void CliManager::handleExportCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "✗ " << RESET << "Missing export options. Try 'help' for command syntax.\n";
        return;
    }

    bool exportTitles = false;
    bool exportCategories = false;
    bool exportResults = false;
    std::string category = "";
    std::string result = "";
    ExportFormat format = ExportFormat::TEXT;
    std::string outputFile = "export_output";
    bool formatSpecified = false;
    bool outputSpecified = false;

    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];

        if (arg == "-t") {
            exportTitles = true;
        }
        else if (arg == "-c") {
            exportCategories = true;
            if (i + 1 < args.size() && args[i + 1][0] != '-') {
                category = args[++i];
            }
        }
        else if (arg == "-r") {
            exportResults = true;
            if (i + 1 < args.size() && args[i + 1][0] != '-') {
                result = args[++i];
            }
        }
        else if (arg == "-txt") {
            format = ExportFormat::TEXT;
            formatSpecified = true;
        }
        else if (arg == "-csv") {
            format = ExportFormat::CSV;
            formatSpecified = true;
        }
        else if (arg == "-json") {
            format = ExportFormat::JSON;
            formatSpecified = true;
        }
        else if (arg == "-o" && i + 1 < args.size()) {
            outputFile = args[++i];
            outputSpecified = true;
        }
    }

    if (!outputSpecified) {
        std::cout << YELLOW << "! " << RESET << "No output file specified, using default: " << outputFile << "\n";
    }

    if (format == ExportFormat::TEXT && outputFile.find(".txt") == std::string::npos) {
        outputFile += ".txt";
    }
    else if (format == ExportFormat::CSV && outputFile.find(".csv") == std::string::npos) {
        outputFile += ".csv";
    }
    else if (format == ExportFormat::JSON && outputFile.find(".json") == std::string::npos) {
        outputFile += ".json";
    }

    bool success = false;
    std::string errorMsg;

    if (exportTitles) {
        auto result = dbManager.exportAllEventTitles(outputFile, format);
        success = result.has_value();
        if (!success) errorMsg = result.error();
    }
    else if (exportCategories) {
        if (!category.empty()) {
            auto result = dbManager.exportEventsByCategory(category, outputFile, format);
            success = result.has_value();
            if (!success) errorMsg = result.error();
        } else {
            auto events = dbManager.getAllEvents();
            if (!events) {
                success = false;
                errorMsg = events.error();
            } else {
                std::set<std::string> categories;
                for (const auto& event : events.value()) {
                    if (!event.category.empty()) {
                        categories.insert(event.category);
                    }
                }
                
                std::ofstream file(outputFile);
                if (!file.is_open()) {
                    success = false;
                    errorMsg = "Could not open file for writing";
                } else {
                    try {
                        success = true;
                        switch (format) {
                            case ExportFormat::TEXT: {
                                file << "# All Categories\n\n";
                                for (const auto& cat : categories) {
                                    file << "- " << cat << "\n";
                                }
                                break;
                            }
                            case ExportFormat::CSV: {
                                file << "Category\n";
                                for (const auto& cat : categories) {
                                    file << "\"" << cat << "\"\n";
                                }
                                break;
                            }
                            case ExportFormat::JSON: {
                                nlohmann::json jsonOutput;
                                jsonOutput["type"] = "categories";
                                nlohmann::json categoriesArray = nlohmann::json::array();
                                for (const auto& cat : categories) {
                                    categoriesArray.push_back(cat);
                                }
                                jsonOutput["categories"] = categoriesArray;
                                file << jsonOutput.dump(4);
                                break;
                            }
                        }
                    } catch (const std::exception& e) {
                        success = false;
                        errorMsg = std::string("Error writing to file: ") + e.what();
                    }
                }
            }
        }
    }
    else if (exportResults) {
        if (!result.empty()) {  
            auto exportResult = dbManager.exportEventsByResult(result, outputFile, format);
            success = exportResult.has_value();
            if (!success) errorMsg = exportResult.error();
        } else {
            auto events = dbManager.getAllEvents();
            if (!events) {
                success = false;
                errorMsg = events.error();
            } else {
                std::set<std::string> results;
                for (const auto& event : events.value()) {
                    if (!event.result.empty()) {
                        results.insert(event.result);
                    }
                }
                
                std::ofstream file(outputFile);
                if (!file.is_open()) {
                    success = false;
                    errorMsg = "Could not open file for writing";
                } else {
                    try {
                        success = true;
                        switch (format) {
                            case ExportFormat::TEXT: {
                                file << "# All Results\n\n";
                                for (const auto& res : results) {
                                    file << "- " << res << "\n";
                                }
                                break;
                            }
                            case ExportFormat::CSV: {
                                file << "Result\n";
                                for (const auto& res : results) {
                                    file << "\"" << res << "\"\n";
                                }
                                break;
                            }
                            case ExportFormat::JSON: {
                                nlohmann::json jsonOutput;
                                jsonOutput["type"] = "results";
                                nlohmann::json resultsArray = nlohmann::json::array();
                                for (const auto& res : results) {
                                    resultsArray.push_back(res);
                                }
                                jsonOutput["results"] = resultsArray;
                                file << jsonOutput.dump(4);
                                break;
                            }
                        }
                    } catch (const std::exception& e) {
                        success = false;
                        errorMsg = std::string("Error writing to file: ") + e.what();
                    }
                }
            }
        }
    }
    else {
        std::cout << YELLOW << "! " << RESET << "No export type specified, defaulting to titles export\n";
        auto result = dbManager.exportAllEventTitles(outputFile, format);
        success = result.has_value();
        if (!success) errorMsg = result.error();
    }

    if (success) {
        std::cout << GREEN << "✓ " << RESET << "Successfully exported data to " << outputFile << "\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error exporting data: " << errorMsg << "\n";
    }
}

ExportFormat CliManager::getFormatFromString(const std::string& formatStr) {
    if (formatStr == "txt" || formatStr == "text") {
        return ExportFormat::TEXT;
    }
    else if (formatStr == "csv") {
        return ExportFormat::CSV;
    }
    else if (formatStr == "json") {
        return ExportFormat::JSON;
    }
    else {
        return ExportFormat::TEXT;
    }
}

void CliManager::handleListEventsByDate(const std::string& dateStr) {
    int month, day;
    
    if (sscanf(dateStr.c_str(), "%d/%d", &month, &day) != 2) {
        std::cout << RED << "✗ " << RESET << "Invalid date format. Expected MM/DD (e.g., 08/03)\n";
        return;
    }
    
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        std::cout << RED << "✗ " << RESET << "Invalid date. Month must be 1-12 and day must be 1-31.\n";
        return;
    }
    
    auto result = dbManager.loadEventsToLinkedList();
    
    if (!result) {
        std::cout << RED << "✗ " << RESET << "Error loading events: " << result.error() << "\n";
        return;
    }
    
    HistoricalLinkedList& list = result.value();
    if (list.isEmpty()) {
        std::cout << YELLOW << "! " << RESET << "No events found in the database.\n";
        return;
    }
    
    std::cout << CYAN << "Events on " << month << "/" << day << " (any year):" << RESET << "\n";
    bool found = false;
    
    HistoricalNode* current = list.getHead();
    while (current) {
        struct tm* timeInfo = localtime(&current->data.date);
        
        if (timeInfo->tm_mon + 1 == month && timeInfo->tm_mday == day) {
            found = true;
            char timeBuffer[80];
            strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d", timeInfo);
            
            std::cout << "  " << BOLD << current->data.id << RESET 
                     << " - " << current->data.title 
                     << " (" << timeBuffer << ")\n";
        }
        
        current = current->next;
    }
    
    if (!found) {
        std::cout << YELLOW << "! " << RESET << "No events found on this date.\n";
    }
}

void CliManager::handleListEventsByYear(const std::string& yearStr) {
    int year;
    
    try {
        year = std::stoi(yearStr);
    } catch (...) {
        std::cout << RED << "✗ " << RESET << "Invalid year format. Expected YYYY (e.g., 1998)\n";
        return;
    }
    
    if (year < 1 || year > 9999) {
        std::cout << RED << "✗ " << RESET << "Invalid year. Year must be between 1 and 9999.\n";
        return;
    }
    
    auto result = dbManager.loadEventsToLinkedList();
    
    if (!result) {
        std::cout << RED << "✗ " << RESET << "Error loading events: " << result.error() << "\n";
        return;
    }
    
    HistoricalLinkedList& list = result.value();
    if (list.isEmpty()) {
        std::cout << YELLOW << "! " << RESET << "No events found in the database.\n";
        return;
    }
    
    std::cout << CYAN << "Events in year " << year << ":" << RESET << "\n";
    bool found = false;
    
    HistoricalNode* current = list.getHead();
    while (current) {
        struct tm* timeInfo = localtime(&current->data.date);
        
        if (timeInfo->tm_year + 1900 == year) {
            found = true;
            char timeBuffer[80];
            strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d", timeInfo);
            
            std::cout << "  " << BOLD << current->data.id << RESET 
                     << " - " << current->data.title 
                     << " (" << timeBuffer << ")\n";
        }
        
        current = current->next;
    }
    
    if (!found) {
        std::cout << YELLOW << "! " << RESET << "No events found in this year.\n";
    }
}

void CliManager::handleAddFirstEvent() {
    if (dbManager.getAllEvents().value_or(std::vector<HistoricalEvent>()).size() > 0) {
        std::cout << RED << "✗ " << RESET << "Database already contains events. This command can only be used on an empty database.\n";
        return;
    }
    
    HistoricalEvent newEvent = inputEventDetails();
    auto result = dbManager.createEvent(newEvent);

    if (result) {
        std::cout << GREEN << "✓ " << RESET << "First event added successfully with ID: " << result.value() << "\n";
    } else {
        std::cout << RED << "✗ " << RESET << "Error adding event: " << result.error() << "\n";
    }
}

void CliManager::handleAddEventAtBeginning() {
    HistoricalEvent newEvent = inputEventDetails();
    auto result = dbManager.createEvent(newEvent);

    if (result) {
        auto listResult = dbManager.loadEventsToLinkedList();
        if (listResult) {
            HistoricalLinkedList& list = listResult.value();
            auto eventResult = dbManager.readEvent(result.value());
            
            if (eventResult) {
                list.clear();
                list.insertAtBeginning(eventResult.value());
                
                auto eventsResult = dbManager.getAllEvents();
                if (eventsResult) {
                    for (const auto& event : eventsResult.value()) {
                        if (event.id != result.value()) {
                            list.insertAtEnd(event);
                        }
                    }
                    
                    auto saveResult = dbManager.saveLinkedListToDb(list);
                    if (saveResult) {
                        std::cout << GREEN << "✓ " << RESET << "Event added at beginning of list with ID: " << result.value() << "\n";
                    } else {
                        std::cout << RED << "✗ " << RESET << "Error saving list: " << saveResult.error() << "\n";
                    }
                }
            }
        } else {
            std::cout << RED << "✗ " << RESET << "Error loading list: " << listResult.error() << "\n";
        }
    } else {
        std::cout << RED << "✗ " << RESET << "Error adding event: " << result.error() << "\n";
    }
}

void CliManager::handleAddEventByDate() {
    HistoricalEvent newEvent = inputEventDetails();
    auto result = dbManager.createEvent(newEvent);

    if (result) {
        auto listResult = dbManager.loadEventsToLinkedList();
        if (listResult) {
            HistoricalLinkedList& list = listResult.value();
            auto eventResult = dbManager.readEvent(result.value());
            
            if (eventResult) {
                list.clear();
                
                auto eventsResult = dbManager.getAllEvents();
                if (eventsResult) {
                    std::vector<HistoricalEvent> events = eventsResult.value();
                    for (const auto& event : events) {
                        if (event.id != result.value()) {
                            list.insertByDate(event);
                        }
                    }
                    
                    list.insertByDate(eventResult.value());
                    
                    auto saveResult = dbManager.saveLinkedListToDb(list);
                    if (saveResult) {
                        std::cout << GREEN << "✓ " << RESET << "Event added in chronological order with ID: " << result.value() << "\n";
                    } else {
                        std::cout << RED << "✗ " << RESET << "Error saving list: " << saveResult.error() << "\n";
                    }
                }
            }
        } else {
            std::cout << RED << "✗ " << RESET << "Error loading list: " << listResult.error() << "\n";
        }
    } else {
        std::cout << RED << "✗ " << RESET << "Error adding event: " << result.error() << "\n";
    }
}

}
}
