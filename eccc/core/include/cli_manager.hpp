#pragma once

#include "database.hpp"
#include "historical_linked_list.hpp"
#include "historical_db_manager.hpp"
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <map>

namespace Eccc {
    namespace Core {

        class CliGeneralLogger {
            protected:
                virtual void displayDashboard() = 0;
                virtual void displayInteractiveMenu() = 0;
                virtual void displaySearchSubmenu() = 0;
                virtual void displaySortSubmenu() = 0;
                virtual void displayExportSubmenu() = 0;
                virtual void displayPressEnterToContinue() = 0;
                virtual void displayEventDetails(const HistoricalEvent& event) = 0;
                virtual void displayProgressBar(int current, int total, const std::string& label) = 0;

                virtual void displayModernSearchMenu() = 0;
                virtual void displayModernSortMenu() = 0;
                virtual void displayModernExportMenu() = 0;
        };

        class CliManager : protected CliGeneralLogger {
        public:
            using COMMAND_VARIANT_TYPE = std::variant<std::vector<std::string>, std::pair<std::string, std::string>>;
	        explicit CliManager() = default;
            explicit CliManager(std::variant<std::shared_ptr<Database>, Database*> db) {
                try {
			        auto& ptr = std::get<std::shared_ptr<Database>>(db);
                    dbManager = HistoricalDbManager(ptr);
                }
                catch (const std::bad_variant_access& ex) {
			        auto& ptr = std::get<Database*>(db);
			        dbManager = HistoricalDbManager(ptr);
                }
            }
            void run();

            static void displayLogo();
            static void displayCommandHelp();

        private:
            HistoricalDbManager dbManager;

            void addSampleData();

            static HistoricalEvent inputEventDetails();
            static std::pair<std::string, std::vector<std::string>> parseCommand(const std::string& input);
            static std::string formatDate(time_t date);

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
            void handleWipeDatabase();
            ExportFormat getFormatFromString(const std::string& formatStr);
    
            void displayDashboard() override;
            void displayInteractiveMenu() override;
            void displaySearchSubmenu() override;
            void displaySortSubmenu() override;
            void displayExportSubmenu() override;
            void displayPressEnterToContinue() override;
            void displayEventDetails(const HistoricalEvent& event) override;
            void displayProgressBar(int current, int total, const std::string& label) override;
    
            void displayModernSearchMenu() override;
            void displayModernSortMenu() override;
            void displayModernExportMenu() override;
            void handleModernExport(int exportType);
            void handleCommandInput(const std::string& command, const std::vector<std::string>& args, bool& running, int& commandMode);

            void handleFindEvent(const COMMAND_VARIANT_TYPE args);
            void handleCategorySearch(const COMMAND_VARIANT_TYPE args);
            void handleLocationSearch(const COMMAND_VARIANT_TYPE args);
        };

    }
}
