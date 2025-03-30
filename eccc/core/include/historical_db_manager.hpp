#pragma once

#include "database.hpp"
#include "historical_linked_list.hpp"
#include <vector>
#include <string>
#include <expected>
#include <format>

namespace Eccc {
    namespace Core {
        class HistoricalDbManager {
        private:
            std::shared_ptr<Database> dbConnection;

        public:
            explicit HistoricalDbManager(std::shared_ptr<Database> db);
            auto setupSchema() -> std::expected<bool, std::string>;

            // CRUD operations
            auto createEvent(const HistoricalEvent& event) -> std::expected<int, std::string>;
            auto readEvent(int id) -> std::expected<HistoricalEvent, std::string>;
            auto updateEvent(const HistoricalEvent& event) -> std::expected<bool, std::string>;
            auto deleteEvent(int id) -> std::expected<bool, std::string>;

            // Batch operations
            auto getAllEvents() -> std::expected<std::vector<HistoricalEvent>, std::string>;
            auto getEventsByCategory(const std::string& category) -> std::expected<std::vector<HistoricalEvent>, std::string>;
            auto getEventsByDateRange(time_t startDate, time_t endDate) -> std::expected<std::vector<HistoricalEvent>, std::string>;
            auto getEventsByLocation(const std::string& location) -> std::expected<std::vector<HistoricalEvent>, std::string>;

            auto loadEventsToLinkedList() -> std::expected<HistoricalLinkedList, std::string>;
            auto saveLinkedListToDb(const HistoricalLinkedList& list) -> std::expected<bool, std::string>;
        };

    }
}
