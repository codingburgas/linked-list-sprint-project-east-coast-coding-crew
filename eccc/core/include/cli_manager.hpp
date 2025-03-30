#pragma once

#include "database.hpp"
#include "historical_linked_list.hpp"
#include "historical_db_manager.hpp"
#include "utilities.hpp"
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <iomanip>

namespace Eccc {
namespace Core {

class CliManager {
public:
    explicit CliManager(std::shared_ptr<Database> db);

    void run();

    static void displayLogo();
    static void displayCommandHelp();

private:
    HistoricalDbManager dbManager;

    // Menu display functions
    void displayMainMenu();
    void displayEventsMenu();
    void displaySearchMenu();
    void displaySortMenu();
    void displayExportMenu();
    void displayExportFormatMenu(std::string& filename, ExportFormat& format);

    // Menu handler functions
    void handleEventsMenu();
    void handleSearchMenu();
    void handleSortMenu();
    void handleExportMenu();
    void handleCommandMode();
    
    // UI helper functions
    void showLoadingAnimation(const std::string& message, int duration);
    void waitForKeyPress();

    void addSampleData();

    static HistoricalEvent inputEventDetails();

    void handleEventCommand(const std::vector<std::string>& args);
    void handleAddEvent();
    void handleAddFirstEvent();
    void handleAddEventAtBeginning();
    void handleAddEventByDate();
    void handleListEvents();
    void handleSortByDate();
    void handleSortByTitle();
    void handleDeleteEvent(const std::vector<std::string>& args);
    void handleUpdateEvent(const std::vector<std::string>& args);
    void handleListEventsByDate(const std::string& dateStr);
    void handleListEventsByYear(const std::string& yearStr);
    void handleExportCommand(const std::vector<std::string>& args);
    ExportFormat getFormatFromString(const std::string& formatStr);
    
    // Keep these private helper functions but don't expose them in the interface
    void handleFindEvent(const std::vector<std::string>& args);
    void handleCategorySearch(const std::vector<std::string>& args);
    void handleLocationSearch(const std::vector<std::string>& args);
};

}
}
