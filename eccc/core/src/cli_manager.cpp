#include "cli_manager.hpp"
#include "utilities.hpp"
#include <iostream>
#include <format>
#include <ctime>
#include <iomanip>
#include <set>
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

namespace Eccc {
namespace Core {

CliManager::CliManager(std::shared_ptr<Database> db) : dbManager(db) {
}

void CliManager::displayLogo() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
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

void CliManager::displayMainMenu() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    displayLogo();
    
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║               " << CYAN << "MAIN MENU" << RESET << BOLD << "                     ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[1]" << RESET << " Manage Events                              " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[2]" << RESET << " Search & Filter                            " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[3]" << RESET << " Export Data                                " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[4]" << RESET << " Sample Data                                " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[5]" << RESET << " Help & Commands                            " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[6]" << RESET << " Command Mode                               " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[0]" << RESET << " Exit                                       " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
}

void CliManager::displayEventsMenu() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║               " << GREEN << "EVENTS MENU" << RESET << BOLD << "                   ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[1]" << RESET << " Add New Event                             " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[2]" << RESET << " Add First Event                           " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[3]" << RESET << " Add Event at Beginning                    " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[4]" << RESET << " Add Event by Date                         " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[5]" << RESET << " View All Events                           " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[6]" << RESET << " View Event by ID                          " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[7]" << RESET << " Update Event                              " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[8]" << RESET << " Delete Event                              " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[9]" << RESET << " Sort Events                               " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[0]" << RESET << " Back to Main Menu                         " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
}

void CliManager::displaySearchMenu() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║               " << MAGENTA << "SEARCH MENU" << RESET << BOLD << "                   ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[1]" << RESET << " Search by Date (MM/DD)                    " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[2]" << RESET << " Search by Year                            " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[3]" << RESET << " Search by Category                        " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[4]" << RESET << " Search by Location                        " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[0]" << RESET << " Back to Main Menu                         " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
}

void CliManager::displaySortMenu() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║                " << CYAN << "SORT MENU" << RESET << BOLD << "                     ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[1]" << RESET << " Sort by Date                              " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[2]" << RESET << " Sort by Title                             " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[0]" << RESET << " Back to Events Menu                       " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
}

void CliManager::displayExportMenu() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║               " << CYAN << "EXPORT MENU" << RESET << BOLD << "                   ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "0" << RESET << ". Back to Main Menu                           " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "1" << RESET << ". Export Titles                               " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "2" << RESET << ". Export by Category                          " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "3" << RESET << ". Export by Result                            " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "4" << RESET << ". Advanced Export (with flags)                " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
}

void CliManager::displayExportFormatMenu(std::string& filename, ExportFormat& format) {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║              " << CYAN << "EXPORT FORMAT" << RESET << BOLD << "                   ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[1]" << RESET << " Text Format (.txt)                        " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[2]" << RESET << " CSV Format (.csv)                         " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "[3]" << RESET << " JSON Format (.json)                       " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
    
    std::cout << "Select format [1-3]: ";
    int formatChoice;
    std::cin >> formatChoice;
    std::cin.ignore();
    
    switch (formatChoice) {
        case 1: format = ExportFormat::TEXT; break;
        case 2: format = ExportFormat::CSV; break;
        case 3: format = ExportFormat::JSON; break;
        default: format = ExportFormat::TEXT; break;
    }
    
    std::cout << "Enter filename (without extension): ";
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        filename = "export_output";
    }
    
    switch (formatChoice) {
        case 1: if (filename.find(".txt") == std::string::npos) filename += ".txt"; break;
        case 2: if (filename.find(".csv") == std::string::npos) filename += ".csv"; break;
        case 3: if (filename.find(".json") == std::string::npos) filename += ".json"; break;
    }
}

void CliManager::showLoadingAnimation(const std::string& message, int duration) {
    std::string animation = "|/-\\";
    int animIndex = 0;
    
    std::cout << message;
    for (int i = 0; i < duration; i++) {
        std::cout << "\b" << animation[animIndex++ % animation.length()] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "\b✓" << std::endl;
}

HistoricalEvent CliManager::inputEventDetails() {
    HistoricalEvent event;

    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║              " << GREEN << "EVENT DETAILS" << RESET << BOLD << "                  ║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";

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

void CliManager::waitForKeyPress() {
    std::cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void CliManager::run() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    displayLogo();

    auto schemaResult = dbManager.setupSchema();
    if (!schemaResult) {
        std::cerr << RED << "✗ Failed to setup database schema: " << schemaResult.error() << RESET << "\n";
        return;
    }

    std::cout << GREEN << "✓ " << RESET << "Database schema initialized\n";
    showLoadingAnimation("Loading database ", 10);
    
    bool running = true;
    while (running) {
        displayMainMenu();
        std::cout << BOLD << YELLOW << "Select an option [0-6]: " << RESET;
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        try {
            int choice = std::stoi(input);
            
            switch (choice) {
                case 0:
                    std::cout << "Exiting program.\n";
                    running = false;
                    break;
                    
                case 1: // Manage Events
                    handleEventsMenu();
                    break;
                    
                case 2: // Search & Filter
                    handleSearchMenu();
                    break;
                    
                case 3: // Export Data
                    handleExportMenu();
                    break;
                    
                case 4: // Sample Data
                    addSampleData();
                    waitForKeyPress();
                    break;
                    
                case 5: // Help & Commands
                    displayCommandHelp();
                    waitForKeyPress();
                    break;
                    
                case 6: // Command Mode
                    handleCommandMode();
                    break;
                    
                default:
                    std::cout << RED << "✗ " << RESET << "Invalid option. Please try again.\n";
                    waitForKeyPress();
            }
        } catch (const std::exception& e) {
            std::cout << RED << "✗ " << RESET << "Invalid input. Please enter a number.\n";
            waitForKeyPress();
        }
    }
}

void CliManager::handleEventsMenu() {
    bool back = false;
    
    while (!back) {
        displayEventsMenu();
        std::cout << BOLD << YELLOW << "Select an option [0-9]: " << RESET;
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        int choice = std::stoi(input);
        
        switch (choice) {
            case 0: // Back to Main Menu
                back = true;
                break;
                
            case 1: // Add New Event
                handleAddEvent();
                waitForKeyPress();
                break;
                
            case 2: // Add First Event
                handleAddFirstEvent();
                waitForKeyPress();
                break;
                
            case 3: // Add Event at Beginning
                handleAddEventAtBeginning();
                waitForKeyPress();
                break;
                
            case 4: // Add Event by Date
                handleAddEventByDate();
                waitForKeyPress();
                break;
                
            case 5: // View All Events
                handleListEvents();
                waitForKeyPress();
                break;
                
            case 6: { // View Event by ID
                std::cout << "Enter event ID: ";
                std::string idStr;
                std::getline(std::cin, idStr);
                try {
                    int id = std::stoi(idStr);
                    auto result = dbManager.readEvent(id);
                    if (result) {
                        const HistoricalEvent& event = result.value();
                        std::cout << "\n" << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
                        std::cout << BOLD << "║ ID: " << CYAN << std::left << std::setw(43) << event.id << RESET << BOLD << "║" << RESET << "\n";
                        std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
                        std::cout << BOLD << "║ " << RESET << "Title: " << event.title << std::string(std::max(0, 37 - static_cast<int>(event.title.length())), ' ') << BOLD << "║" << RESET << "\n";
                        std::cout << BOLD << "║ " << RESET << "Description: " << event.description.substr(0, 30) << (event.description.length() > 30 ? "..." : "") << std::string(std::max(0, 27 - static_cast<int>(std::min(event.description.length(), size_t(33)))), ' ') << BOLD << "║" << RESET << "\n";
                        std::cout << BOLD << "║ " << RESET << "Location: " << event.location << std::string(std::max(0, 34 - static_cast<int>(event.location.length())), ' ') << BOLD << "║" << RESET << "\n";
                        
                        char timeBuffer[80];
                        struct tm* timeInfo = localtime(&event.date);
                        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d", timeInfo);
                        
                        std::cout << BOLD << "║ " << RESET << "Date: " << timeBuffer << std::string(std::max(0, 38 - static_cast<int>(strlen(timeBuffer))), ' ') << BOLD << "║" << RESET << "\n";
                        std::cout << BOLD << "║ " << RESET << "Category: " << event.category << std::string(std::max(0, 34 - static_cast<int>(event.category.length())), ' ') << BOLD << "║" << RESET << "\n";
                        std::cout << BOLD << "║ " << RESET << "Significance: " << event.significance << std::string(std::max(0, 31 - static_cast<int>(std::to_string(event.significance).length())), ' ') << BOLD << "║" << RESET << "\n";
                        std::cout << BOLD << "║ " << RESET << "Leader: " << event.leader << std::string(std::max(0, 36 - static_cast<int>(event.leader.length())), ' ') << BOLD << "║" << RESET << "\n";
                        std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n";
                    } else {
                        std::cout << RED << "✗ " << RESET << "Error finding event: " << result.error() << "\n";
                    }
                } catch (...) {
                    std::cout << RED << "✗ " << RESET << "Invalid ID. Please enter a number.\n";
                }
                waitForKeyPress();
                break;
            }
                
            case 7: { // Update Event
                std::cout << "Enter event ID to update: ";
                std::string idStr;
                std::getline(std::cin, idStr);
                std::vector<std::string> args = {"update", idStr};
                handleUpdateEvent(args);
                waitForKeyPress();
                break;
            }
                
            case 8: { // Delete Event
                std::cout << "Enter event ID to delete: ";
                std::string idStr;
                std::getline(std::cin, idStr);
                std::vector<std::string> args = {"delete", idStr};
                handleDeleteEvent(args);
                waitForKeyPress();
                break;
            }
                
            case 9: // Sort Events
                handleSortMenu();
                break;
                
            default:
                std::cout << RED << "✗ " << RESET << "Invalid option. Please try again.\n";
                waitForKeyPress();
        }
    }
}

void CliManager::handleSortMenu() {
    bool back = false;
    
    while (!back) {
        displaySortMenu();
        std::cout << BOLD << YELLOW << "Select an option [0-2]: " << RESET;
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        int choice = std::stoi(input);
        
        switch (choice) {
            case 0: // Back to Events Menu
                back = true;
                break;
                
            case 1: // Sort by Date
                handleSortByDate();
                waitForKeyPress();
                break;
                
            case 2: // Sort by Title
                handleSortByTitle();
                waitForKeyPress();
                break;
                
            default:
                std::cout << RED << "✗ " << RESET << "Invalid option. Please try again.\n";
                waitForKeyPress();
        }
    }
}

void CliManager::handleSearchMenu() {
    bool back = false;
    
    while (!back) {
        displaySearchMenu();
        std::cout << BOLD << YELLOW << "Select an option [0-4]: " << RESET;
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        int choice = std::stoi(input);
        
        switch (choice) {
            case 0: // Back to Main Menu
                back = true;
                break;
                
            case 1: { // Search by Date
                std::cout << "Enter date (MM/DD): ";
                std::string dateStr;
                std::getline(std::cin, dateStr);
                handleListEventsByDate(dateStr);
                waitForKeyPress();
                break;
            }
                
            case 2: { // Search by Year
                std::cout << "Enter year (YYYY): ";
                std::string yearStr;
                std::getline(std::cin, yearStr);
                handleListEventsByYear(yearStr);
                waitForKeyPress();
                break;
            }
                
            case 3: { // Search by Category
                std::cout << "Enter category: ";
                std::string categoryStr;
                std::getline(std::cin, categoryStr);
                handleCategorySearch({"category", categoryStr});
                waitForKeyPress();
                break;
            }
                
            case 4: { // Search by Location
                std::cout << "Enter location: ";
                std::string locationStr;
                std::getline(std::cin, locationStr);
                handleLocationSearch({"location", locationStr});
                waitForKeyPress();
                break;
            }
                
            default:
                std::cout << RED << "✗ " << RESET << "Invalid option. Please try again.\n";
                waitForKeyPress();
        }
    }
}

void CliManager::handleExportMenu() {
    bool back = false;
    
    while (!back) {
        displayExportMenu();
        std::cout << BOLD << YELLOW << "Select an option [0-4]: " << RESET;
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        try {
            int choice = std::stoi(input);
            
            switch (choice) {
                case 0: // Back to Main Menu
                    back = true;
                    break;
                    
                case 1: { // Export Titles
                    std::string filename;
                    ExportFormat format;
                    displayExportFormatMenu(filename, format);
                    auto result = dbManager.exportAllEventTitles(filename, format);
                    if (result) {
                        showLoadingAnimation("Exporting data ", 5);
                        std::cout << GREEN << "✓ " << RESET << "Successfully exported titles to " << filename << "\n";
                    } else {
                        std::cout << RED << "✗ " << RESET << "Error exporting titles: " << result.error() << "\n";
                    }
                    waitForKeyPress();
                    break;
                }
                
                case 2: { // Export by Category
                    std::cout << "Enter category (leave empty for all categories): ";
                    std::string categoryStr;
                    std::getline(std::cin, categoryStr);
                    
                    std::string filename;
                    ExportFormat format;
                    displayExportFormatMenu(filename, format);
                    
                    std::expected<bool, std::string> result;
                    if (categoryStr.empty()) {
                        auto events = dbManager.getAllEvents();
                        if (!events) {
                            std::cout << RED << "✗ " << RESET << "Error loading events: " << events.error() << "\n";
                            waitForKeyPress();
                            break;
                        }
                        
                        std::set<std::string> categories;
                        for (const auto& event : events.value()) {
                            if (!event.category.empty()) {
                                categories.insert(event.category);
                            }
                        }
                        
                        std::ofstream file(filename);
                        if (!file.is_open()) {
                            std::cout << RED << "✗ " << RESET << "Could not open file for writing\n";
                            waitForKeyPress();
                            break;
                        }
                        
                        try {
                            showLoadingAnimation("Exporting categories ", 5);
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
                            std::cout << GREEN << "✓ " << RESET << "Successfully exported categories to " << filename << "\n";
                        } catch (const std::exception& e) {
                            std::cout << RED << "✗ " << RESET << "Error writing to file: " << e.what() << "\n";
                        }
                    } else {
                        result = dbManager.exportEventsByCategory(categoryStr, filename, format);
                        if (result) {
                            showLoadingAnimation("Exporting category data ", 5);
                            std::cout << GREEN << "✓ " << RESET << "Successfully exported category data to " << filename << "\n";
                        } else {
                            std::cout << RED << "✗ " << RESET << "Error exporting category data: " << result.error() << "\n";
                        }
                    }
                    waitForKeyPress();
                    break;
                }
                
                case 3: { // Export by Result
                    std::cout << "Enter result (leave empty for all results): ";
                    std::string resultStr;
                    std::getline(std::cin, resultStr);
                    
                    std::string filename;
                    ExportFormat format;
                    displayExportFormatMenu(filename, format);
                    
                    std::expected<bool, std::string> result;
                    if (resultStr.empty()) {
                        auto events = dbManager.getAllEvents();
                        if (!events) {
                            std::cout << RED << "✗ " << RESET << "Error loading events: " << events.error() << "\n";
                            waitForKeyPress();
                            break;
                        }
                        
                        std::set<std::string> results;
                        for (const auto& event : events.value()) {
                            if (!event.result.empty()) {
                                results.insert(event.result);
                            }
                        }
                        
                        std::ofstream file(filename);
                        if (!file.is_open()) {
                            std::cout << RED << "✗ " << RESET << "Could not open file for writing\n";
                            waitForKeyPress();
                            break;
                        }
                        
                        try {
                            showLoadingAnimation("Exporting results ", 5);
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
                            std::cout << GREEN << "✓ " << RESET << "Successfully exported results to " << filename << "\n";
                        } catch (const std::exception& e) {
                            std::cout << RED << "✗ " << RESET << "Error writing to file: " << e.what() << "\n";
                        }
                    } else {
                        result = dbManager.exportEventsByResult(resultStr, filename, format);
                        if (result) {
                            showLoadingAnimation("Exporting result data ", 5);
                            std::cout << GREEN << "✓ " << RESET << "Successfully exported result data to " << filename << "\n";
                        } else {
                            std::cout << RED << "✗ " << RESET << "Error exporting result data: " << result.error() << "\n";
                        }
                    }
                    waitForKeyPress();
                    break;
                }
                
                case 4: { // Advanced Export with flags
                    std::cout << "\033[2J\033[1;1H"; // Clear screen
                    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
                    std::cout << BOLD << "║             " << CYAN << "ADVANCED EXPORT" << RESET << BOLD << "                  ║" << RESET << "\n";
                    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << "Available flags:                               " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -t         : Export titles only               " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -c         : Export all categories            " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -c CATEGORY: Export specific category         " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -r         : Export all results               " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -r RESULT  : Export specific result           " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -txt       : Export as text format            " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -csv       : Export as CSV format             " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -json      : Export as JSON format            " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "║ " << RESET << " -o FILENAME: Specify output filename          " << BOLD << "║" << RESET << "\n";
                    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
                    
                    std::cout << YELLOW << "Enter export command (e.g., -t -json -o events.json): " << RESET;
                    std::string exportCommand;
                    std::getline(std::cin, exportCommand);
                    
                    // Parse the export command into arguments
                    auto [_, args] = parseCommand("export " + exportCommand);
                    
                    // Pass to the export handler
                    handleExportCommand(args);
                    waitForKeyPress();
                    break;
                }
                
                default:
                    std::cout << RED << "✗ " << RESET << "Invalid option. Please try again.\n";
                    waitForKeyPress();
            }
        } catch (const std::exception& e) {
            std::cout << RED << "✗ " << RESET << "Invalid input. Please enter a number.\n";
            waitForKeyPress();
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
            std::cout << "\033[2J\033[1;1H"; // Clear screen
            std::cout << CYAN << "Total events: " << list.getSize() << RESET << "\n\n";
            
            // Display table header
            std::cout << BOLD << "╔════╦═══════════════════════════════╦════════════════════╦══════════════╗" << RESET << "\n";
            std::cout << BOLD << "║" << CYAN << " ID " << RESET << BOLD << "║" << CYAN << " Title                          " << RESET << BOLD << "║" << CYAN << " Date             " << RESET << BOLD << "║" << CYAN << " Category      " << RESET << BOLD << "║" << RESET << "\n";
            std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
            
            // Display table rows
            HistoricalNode* current = list.getHead();
            int count = 0;
            
            while (current != nullptr) {
                char dateBuffer[20];
                struct tm* timeInfo = localtime(&current->data.date);
                strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
                
                std::string title = current->data.title;
                if (title.length() > 29) {
                    title = title.substr(0, 26) + "...";
                }
                
                std::string category = current->data.category;
                if (category.length() > 14) {
                    category = category.substr(0, 11) + "...";
                }
                
                std::cout << BOLD << "║" << RESET << " " << std::setw(2) << current->data.id << " " 
                         << BOLD << "║" << RESET << " " << std::left << std::setw(29) << title << " "
                         << BOLD << "║" << RESET << " " << std::left << std::setw(16) << dateBuffer << " "
                         << BOLD << "║" << RESET << " " << std::left << std::setw(12) << category << " "
                         << BOLD << "║" << RESET << "\n";
                
                current = current->next;
                count++;
                
                // Add a divider every 5 rows for readability
                if (count % 5 == 0 && current != nullptr) {
                    std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
                }
            }
            
            // Display table footer
            std::cout << BOLD << "╚════╩═══════════════════════════════╩════════════════════╩══════════════╝" << RESET << "\n";
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
        std::string eventCategory = event.category;
        std::string query = categoryQuery;
        
        std::transform(eventCategory.begin(), eventCategory.end(), eventCategory.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        std::transform(query.begin(), query.end(), query.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        
        if (eventCategory.find(query) != std::string::npos) {
            matchedEvents.push_back(event);
        }
    }

    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << CYAN << "Events matching category '" << categoryQuery << "':" << RESET << "\n\n";
    
    if (matchedEvents.empty()) {
        std::cout << YELLOW << "! " << RESET << "No events found matching category '" << categoryQuery << "'.\n";
        return;
    }

    // Display table header
    std::cout << BOLD << "╔════╦═══════════════════════════════╦════════════════════╦══════════════╗" << RESET << "\n";
    std::cout << BOLD << "║" << CYAN << " ID " << RESET << BOLD << "║" << CYAN << " Title                          " << RESET << BOLD << "║" << CYAN << " Date             " << RESET << BOLD << "║" << CYAN << " Category      " << RESET << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
    
    // Display table rows
    int count = 0;
    
    for (const auto& event : matchedEvents) {
        char dateBuffer[20];
        struct tm* timeInfo = localtime(&event.date);
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
        
        std::string title = event.title;
        if (title.length() > 29) {
            title = title.substr(0, 26) + "...";
        }
        
        std::string category = event.category;
        if (category.length() > 14) {
            category = category.substr(0, 11) + "...";
        }
        
        std::cout << BOLD << "║" << RESET << " " << std::setw(2) << event.id << " " 
                << BOLD << "║" << RESET << " " << std::left << std::setw(29) << title << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(16) << dateBuffer << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(12) << category << " "
                << BOLD << "║" << RESET << "\n";
        
        count++;
        
        // Add a divider every 5 rows for readability
        if (count % 5 == 0 && count < matchedEvents.size()) {
            std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
        }
    }
    
    // Display table footer
    std::cout << BOLD << "╚════╩═══════════════════════════════╩════════════════════╩══════════════╝" << RESET << "\n";
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
        std::string eventLocation = event.location;
        std::string query = locationQuery;
        
        std::transform(eventLocation.begin(), eventLocation.end(), eventLocation.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        std::transform(query.begin(), query.end(), query.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        
        if (eventLocation.find(query) != std::string::npos) {
            matchedEvents.push_back(event);
        }
    }

    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << CYAN << "Events matching location '" << locationQuery << "':" << RESET << "\n\n";
    
    if (matchedEvents.empty()) {
        std::cout << YELLOW << "! " << RESET << "No events found matching location '" << locationQuery << "'.\n";
        return;
    }

    // Display table header
    std::cout << BOLD << "╔════╦═══════════════════════════════╦════════════════════╦══════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║" << CYAN << " ID " << RESET << BOLD << "║" << CYAN << " Title                          " << RESET << BOLD << "║" << CYAN << " Date             " << RESET << BOLD << "║" << CYAN << " Location        " << RESET << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════════╣" << RESET << "\n";
    
    // Display table rows
    int count = 0;
    
    for (const auto& event : matchedEvents) {
        char dateBuffer[20];
        struct tm* timeInfo = localtime(&event.date);
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
        
        std::string title = event.title;
        if (title.length() > 29) {
            title = title.substr(0, 26) + "...";
        }
        
        std::string location = event.location;
        if (location.length() > 16) {
            location = location.substr(0, 13) + "...";
        }
        
        std::cout << BOLD << "║" << RESET << " " << std::setw(2) << event.id << " " 
                << BOLD << "║" << RESET << " " << std::left << std::setw(29) << title << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(16) << dateBuffer << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(14) << location << " "
                << BOLD << "║" << RESET << "\n";
        
        count++;
        
        // Add a divider every 5 rows for readability
        if (count % 5 == 0 && count < matchedEvents.size()) {
            std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════════╣" << RESET << "\n";
        }
    }
    
    // Display table footer
    std::cout << BOLD << "╚════╩═══════════════════════════════╩════════════════════╩══════════════════╝" << RESET << "\n";
}

void CliManager::handleSortByDate() {
    auto result = dbManager.loadEventsToLinkedList();

    if (result) {
        HistoricalLinkedList& list = result.value();

        if (list.isEmpty()) {
            std::cout << YELLOW << "! " << RESET << "No events found in the database.\n";
        } else {
            showLoadingAnimation("Sorting events by date ", 5);
            list.sortByDate();
            std::cout << GREEN << "✓ " << RESET << "Events sorted by date:\n";
            
            // Display the sorted events
            std::cout << "\033[2J\033[1;1H"; // Clear screen
            std::cout << CYAN << "Events sorted by date:" << RESET << "\n\n";
            
            // Display table header
            std::cout << BOLD << "╔════╦═══════════════════════════════╦════════════════════╦══════════════╗" << RESET << "\n";
            std::cout << BOLD << "║" << CYAN << " ID " << RESET << BOLD << "║" << CYAN << " Title                          " << RESET << BOLD << "║" << CYAN << " Date             " << RESET << BOLD << "║" << CYAN << " Category      " << RESET << BOLD << "║" << RESET << "\n";
            std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
            
            // Display table rows
            HistoricalNode* current = list.getHead();
            int count = 0;
            
            while (current != nullptr) {
                char dateBuffer[20];
                struct tm* timeInfo = localtime(&current->data.date);
                strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
                
                std::string title = current->data.title;
                if (title.length() > 29) {
                    title = title.substr(0, 26) + "...";
                }
                
                std::string category = current->data.category;
                if (category.length() > 14) {
                    category = category.substr(0, 11) + "...";
                }
                
                std::cout << BOLD << "║" << RESET << " " << std::setw(2) << current->data.id << " " 
                         << BOLD << "║" << RESET << " " << std::left << std::setw(29) << title << " "
                         << BOLD << "║" << RESET << " " << std::left << std::setw(16) << dateBuffer << " "
                         << BOLD << "║" << RESET << " " << std::left << std::setw(12) << category << " "
                         << BOLD << "║" << RESET << "\n";
                
                current = current->next;
                count++;
                
                // Add a divider every 5 rows for readability
                if (count % 5 == 0 && current != nullptr) {
                    std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
                }
            }
            
            // Display table footer
            std::cout << BOLD << "╚════╩═══════════════════════════════╩════════════════════╩══════════════╝" << RESET << "\n";

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
            showLoadingAnimation("Sorting events by title ", 5);
            list.sortByTitle();
            std::cout << GREEN << "✓ " << RESET << "Events sorted by title:\n";
            
            // Display the sorted events
            std::cout << "\033[2J\033[1;1H"; // Clear screen
            std::cout << CYAN << "Events sorted by title:" << RESET << "\n\n";
            
            // Display table header
            std::cout << BOLD << "╔════╦═══════════════════════════════╦════════════════════╦══════════════╗" << RESET << "\n";
            std::cout << BOLD << "║" << CYAN << " ID " << RESET << BOLD << "║" << CYAN << " Title                          " << RESET << BOLD << "║" << CYAN << " Date             " << RESET << BOLD << "║" << CYAN << " Category      " << RESET << BOLD << "║" << RESET << "\n";
            std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
            
            // Display table rows
            HistoricalNode* current = list.getHead();
            int count = 0;
            
            while (current != nullptr) {
                char dateBuffer[20];
                struct tm* timeInfo = localtime(&current->data.date);
                strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
                
                std::string title = current->data.title;
                if (title.length() > 29) {
                    title = title.substr(0, 26) + "...";
                }
                
                std::string category = current->data.category;
                if (category.length() > 14) {
                    category = category.substr(0, 11) + "...";
                }
                
                std::cout << BOLD << "║" << RESET << " " << std::setw(2) << current->data.id << " " 
                         << BOLD << "║" << RESET << " " << std::left << std::setw(29) << title << " "
                         << BOLD << "║" << RESET << " " << std::left << std::setw(16) << dateBuffer << " "
                         << BOLD << "║" << RESET << " " << std::left << std::setw(12) << category << " "
                         << BOLD << "║" << RESET << "\n";
                
                current = current->next;
                count++;
                
                // Add a divider every 5 rows for readability
                if (count % 5 == 0 && current != nullptr) {
                    std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
                }
            }
            
            // Display table footer
            std::cout << BOLD << "╚════╩═══════════════════════════════╩════════════════════╩══════════════╝" << RESET << "\n";

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
    
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << CYAN << "Events on " << month << "/" << day << " (any year):" << RESET << "\n\n";
    
    // Find matching events first
    std::vector<HistoricalEvent> matchedEvents;
    HistoricalNode* current = list.getHead();
    
    while (current) {
        struct tm* timeInfo = localtime(&current->data.date);
        
        if (timeInfo->tm_mon + 1 == month && timeInfo->tm_mday == day) {
            matchedEvents.push_back(current->data);
        }
        
        current = current->next;
    }
    
    if (matchedEvents.empty()) {
        std::cout << YELLOW << "! " << RESET << "No events found on this date.\n";
        return;
    }
    
    // Display table header
    std::cout << BOLD << "╔════╦═══════════════════════════════╦════════════════════╦══════════════╗" << RESET << "\n";
    std::cout << BOLD << "║" << CYAN << " ID " << RESET << BOLD << "║" << CYAN << " Title                          " << RESET << BOLD << "║" << CYAN << " Date             " << RESET << BOLD << "║" << CYAN << " Category      " << RESET << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
    
    // Display table rows
    int count = 0;
    
    for (const auto& event : matchedEvents) {
        char dateBuffer[20];
        struct tm* timeInfo = localtime(&event.date);
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
        
        std::string title = event.title;
        if (title.length() > 29) {
            title = title.substr(0, 26) + "...";
        }
        
        std::string category = event.category;
        if (category.length() > 14) {
            category = category.substr(0, 11) + "...";
        }
        
        std::cout << BOLD << "║" << RESET << " " << std::setw(2) << event.id << " " 
                << BOLD << "║" << RESET << " " << std::left << std::setw(29) << title << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(16) << dateBuffer << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(12) << category << " "
                << BOLD << "║" << RESET << "\n";
        
        count++;
        
        // Add a divider every 5 rows for readability
        if (count % 5 == 0 && count < matchedEvents.size()) {
            std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
        }
    }
    
    // Display table footer
    std::cout << BOLD << "╚════╩═══════════════════════════════╩════════════════════╩══════════════╝" << RESET << "\n";
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
    
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << CYAN << "Events in year " << year << ":" << RESET << "\n\n";
    
    // Find matching events first
    std::vector<HistoricalEvent> matchedEvents;
    HistoricalNode* current = list.getHead();
    
    while (current) {
        struct tm* timeInfo = localtime(&current->data.date);
        
        if (timeInfo->tm_year + 1900 == year) {
            matchedEvents.push_back(current->data);
        }
        
        current = current->next;
    }
    
    if (matchedEvents.empty()) {
        std::cout << YELLOW << "! " << RESET << "No events found in this year.\n";
        return;
    }
    
    // Display table header
    std::cout << BOLD << "╔════╦═══════════════════════════════╦════════════════════╦══════════════╗" << RESET << "\n";
    std::cout << BOLD << "║" << CYAN << " ID " << RESET << BOLD << "║" << CYAN << " Title                          " << RESET << BOLD << "║" << CYAN << " Date             " << RESET << BOLD << "║" << CYAN << " Category      " << RESET << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
    
    // Display table rows
    int count = 0;
    
    for (const auto& event : matchedEvents) {
        char dateBuffer[20];
        struct tm* timeInfo = localtime(&event.date);
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
        
        std::string title = event.title;
        if (title.length() > 29) {
            title = title.substr(0, 26) + "...";
        }
        
        std::string category = event.category;
        if (category.length() > 14) {
            category = category.substr(0, 11) + "...";
        }
        
        std::cout << BOLD << "║" << RESET << " " << std::setw(2) << event.id << " " 
                << BOLD << "║" << RESET << " " << std::left << std::setw(29) << title << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(16) << dateBuffer << " "
                << BOLD << "║" << RESET << " " << std::left << std::setw(12) << category << " "
                << BOLD << "║" << RESET << "\n";
        
        count++;
        
        // Add a divider every 5 rows for readability
        if (count % 5 == 0 && count < matchedEvents.size()) {
            std::cout << BOLD << "╠════╬═══════════════════════════════╬════════════════════╬══════════════╣" << RESET << "\n";
        }
    }
    
    // Display table footer
    std::cout << BOLD << "╚════╩═══════════════════════════════╩════════════════════╩══════════════╝" << RESET << "\n";
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

void CliManager::displayCommandHelp() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║              " << MAGENTA << "COMMAND REFERENCE" << RESET << BOLD << "               ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << GREEN << "EVENT COMMANDS:" << RESET << "                              " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event add" << RESET << "            Add a new event      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event add -f" << RESET << "         Add first event      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event add -b" << RESET << "         Add at beginning     " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event add -d" << RESET << "         Add by date order    " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event list" << RESET << "           View all events      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event list -d MM/DD" << RESET << "  Find by month/day    " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event list -y YYYY" << RESET << "   Find by year         " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event list -c TEXT" << RESET << "   Find by category     " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event list -l TEXT" << RESET << "   Find by location     " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event list id ID" << RESET << "     Find by ID           " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event sort-date" << RESET << "      Sort by date         " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event sort-title" << RESET << "     Sort by title        " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event delete <id>" << RESET << "    Delete an event      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> event update <id>" << RESET << "    Update an event      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << GREEN << "EXPORT COMMANDS:" << RESET << "                             " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> export -t" << RESET << "            Export titles        " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> export -c [category]" << RESET << " Export by category   " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> export -r [result]" << RESET << "   Export by result     " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> export -txt/-csv/-json" << RESET << " Export format      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> export -o <filename>" << RESET << " Output filename      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << GREEN << "OTHER COMMANDS:" << RESET << "                              " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> sample" << RESET << "               Add sample data      " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> help" << RESET << "                 View commands        " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << YELLOW << "> exit" << RESET << "                 Exit program         " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n";
}

void CliManager::handleCommandMode() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << BOLD << "╔══════════════════════════════════════════════════╗" << RESET << "\n";
    std::cout << BOLD << "║              " << CYAN << "COMMAND MODE" << RESET << BOLD << "                   ║" << RESET << "\n";
    std::cout << BOLD << "╠══════════════════════════════════════════════════╣" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << "Enter commands directly (type 'exit' to return)   " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "║ " << RESET << "Type 'help' to see available commands             " << BOLD << "║" << RESET << "\n";
    std::cout << BOLD << "╚══════════════════════════════════════════════════╝" << RESET << "\n\n";
    
    bool inCommandMode = true;
    
    while (inCommandMode) {
        std::cout << "\n" << BOLD << YELLOW << "hist-cli" << RESET << " " << BLUE << "$ " << RESET;
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "exit" || input == "quit" || input == "back") {
            inCommandMode = false;
            continue;
        }
        
        auto [command, args] = Eccc::Core::parseCommand(input);
        
        if (command == "help") {
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

}
}
