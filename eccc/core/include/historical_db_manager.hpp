#pragma once

#include "database.hpp"
#include "historical_linked_list.hpp"
#include <vector>
#include <string>
#include <expected>
#include <format>
#include <iostream>

namespace Eccc {
    namespace Core {
        enum class ExportFormat {
            TEXT,
            CSV,
            JSON
        };
		using GENERIC_HISTORIC_RETURN_TYPE = std::expected<std::vector<HistoricalEvent>, std::string>;
        using GENERIC_NUMBER_RETURN_TYPE = std::expected<int, std::string>;
		using GENERIC_BOOL_RETURN_TYPE = std::expected<bool, std::string>;

        class HistoricalDbManager {
        private:
            std::shared_ptr<Database> dbConnection;

        public:
            explicit HistoricalDbManager(std::shared_ptr<Database> db);
            std::expected<bool, std::string> setupSchema();

            GENERIC_NUMBER_RETURN_TYPE createEvent(const HistoricalEvent& event);
            std::expected<HistoricalEvent, std::string> readEvent(int id);
            GENERIC_BOOL_RETURN_TYPE updateEvent(const HistoricalEvent& event);
            GENERIC_BOOL_RETURN_TYPE deleteEvent(int id);

            GENERIC_HISTORIC_RETURN_TYPE getAllEvents();
            GENERIC_HISTORIC_RETURN_TYPE getEventsByCategory(const std::string& category);
            GENERIC_HISTORIC_RETURN_TYPE getEventsByDateRange(time_t startDate, time_t endDate);
            GENERIC_HISTORIC_RETURN_TYPE getEventsByLocation(const std::string& location);
            GENERIC_HISTORIC_RETURN_TYPE getEventsByResult(const std::string& resultPattern);

            std::expected<HistoricalLinkedList, std::string> loadEventsToLinkedList();
            GENERIC_BOOL_RETURN_TYPE saveLinkedListToDb(const HistoricalLinkedList& list);

            GENERIC_BOOL_RETURN_TYPE exportAllEventTitles(const std::string& filename, ExportFormat format);
            GENERIC_BOOL_RETURN_TYPE exportEventsByCategory(const std::string& category, const std::string& filename, ExportFormat format);
            GENERIC_BOOL_RETURN_TYPE exportEventsByResult(const std::string& resultPattern, const std::string& filename, ExportFormat format);
        };

    }
}
