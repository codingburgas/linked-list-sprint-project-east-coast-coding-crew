#include "historical_db_manager.hpp"
#include <soci/soci.h>
#include <ctime>

namespace Eccc {
    namespace Core {
        HistoricalDbManager::HistoricalDbManager(std::shared_ptr<Database> db) : dbConnection(db) {}

        auto HistoricalDbManager::setupSchema() -> std::expected<bool, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                try {
                    (*sql) << "SELECT 1 FROM historical_events LIMIT 1";
                    return true;
                } catch (const soci::soci_error&) {

                }

                (*sql) << "CREATE TABLE IF NOT EXISTS historical_events ("
                       "id SERIAL PRIMARY KEY,"
                       "title VARCHAR(255) NOT NULL,"
                       "description TEXT,"
                       "location VARCHAR(255),"
                       "event_date BIGINT,"
                       "category VARCHAR(100),"
                       "significance INTEGER"
                       ")";

                return true;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error creating schema: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error creating schema: {}", e.what()));
            }
        }

        auto HistoricalDbManager::createEvent(const HistoricalEvent& event) -> std::expected<int, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                int newId = -1;

                long long eventDate = static_cast<long long>(event.date);

                (*sql) << "INSERT INTO historical_events "
                       "(title, description, location, event_date, category, significance) "
                       "VALUES (:title, :description, :location, :date, :category, :significance) "
                       "RETURNING id",
                       soci::use(event.title),
                       soci::use(event.description),
                       soci::use(event.location),
                       soci::use(eventDate),
                       soci::use(event.category),
                       soci::use(event.significance),
                       soci::into(newId);

                return newId;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error creating event: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error creating event: {}", e.what()));
            }
        }

        auto HistoricalDbManager::readEvent(int id) -> std::expected<HistoricalEvent, std::string> {
            try {
                auto* sql = dbConnection->getSession();

                HistoricalEvent event;
                std::string title, description, location, category;
                long long dateTimestamp;
                int significance;

                soci::indicator titleInd, descInd, locInd, dateInd, catInd, sigInd;

                (*sql) << "SELECT id, title, description, location, event_date, "
                       "category, significance FROM historical_events WHERE id = :id",
                       soci::use(id),
                       soci::into(event.id),
                       soci::into(title, titleInd),
                       soci::into(description, descInd),
                       soci::into(location, locInd),
                       soci::into(dateTimestamp, dateInd),
                       soci::into(category, catInd),
                       soci::into(significance, sigInd);

                if (sql->got_data()) {
                    event.title = (titleInd == soci::i_ok) ? title : "";
                    event.description = (descInd == soci::i_ok) ? description : "";
                    event.location = (locInd == soci::i_ok) ? location : "";
                    event.date = (dateInd == soci::i_ok) ? static_cast<time_t>(dateTimestamp) : 0;
                    event.category = (catInd == soci::i_ok) ? category : "";
                    event.significance = (sigInd == soci::i_ok) ? significance : 0;

                    return event;
                } else {
                    return std::unexpected(std::format("Event with ID {} not found", id));
                }
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error reading event: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error reading event: {}", e.what()));
            }
        }

        auto HistoricalDbManager::updateEvent(const HistoricalEvent& event) -> std::expected<bool, std::string> {
            try {
                auto* sql = dbConnection->getSession();

                long long eventDate = static_cast<long long>(event.date);

                (*sql) << "UPDATE historical_events SET "
                       "title = :title, description = :description, "
                       "location = :location, event_date = :date, "
                       "category = :category, significance = :significance "
                       "WHERE id = :id",
                       soci::use(event.title),
                       soci::use(event.description),
                       soci::use(event.location),
                       soci::use(eventDate),
                       soci::use(event.category),
                       soci::use(event.significance),
                       soci::use(event.id);

                return true;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error updating event: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error updating event: {}", e.what()));
            }
        }

        auto HistoricalDbManager::deleteEvent(int id) -> std::expected<bool, std::string> {
            try {
                auto* sql = dbConnection->getSession();

                (*sql) << "DELETE FROM historical_events WHERE id = :id", soci::use(id);

                return true;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error deleting event: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error deleting event: {}", e.what()));
            }
        }

        auto HistoricalDbManager::getAllEvents() -> std::expected<std::vector<HistoricalEvent>, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                std::vector<HistoricalEvent> events;

                soci::rowset<soci::row> rows = (sql->prepare <<
                    "SELECT id, title, description, location, event_date, category, significance "
                    "FROM historical_events ORDER BY event_date");

                for (const auto& row : rows) {
                    HistoricalEvent event;
                    event.id = row.get<int>(0);
                    event.title = row.get<std::string>(1, "");
                    event.description = row.get<std::string>(2, "");
                    event.location = row.get<std::string>(3, "");
                    event.date = static_cast<time_t>(row.get<long long>(4, 0));
                    event.category = row.get<std::string>(5, "");
                    event.significance = row.get<int>(6, 0);

                    events.push_back(event);
                }

                return events;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error retrieving events: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error retrieving events: {}", e.what()));
            }
        }

        auto HistoricalDbManager::getEventsByCategory(const std::string& category) -> std::expected<std::vector<HistoricalEvent>, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                std::vector<HistoricalEvent> events;

                soci::rowset<soci::row> rows = (sql->prepare <<
                    "SELECT id, title, description, location, event_date, category, significance "
                    "FROM historical_events WHERE category = :category ORDER BY event_date",
                    soci::use(category));

                for (const auto& row : rows) {
                    HistoricalEvent event;
                    event.id = row.get<int>(0);
                    event.title = row.get<std::string>(1, "");
                    event.description = row.get<std::string>(2, "");
                    event.location = row.get<std::string>(3, "");
                    event.date = static_cast<time_t>(row.get<long long>(4, 0));
                    event.category = row.get<std::string>(5, "");
                    event.significance = row.get<int>(6, 0);

                    events.push_back(event);
                }

                return events;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error retrieving events by category: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error retrieving events by category: {}", e.what()));
            }
        }

        auto HistoricalDbManager::getEventsByDateRange(time_t startDate, time_t endDate) -> std::expected<std::vector<HistoricalEvent>, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                std::vector<HistoricalEvent> events;

                long long startTimestamp = static_cast<long long>(startDate);
                long long endTimestamp = static_cast<long long>(endDate);

                soci::rowset<soci::row> rows = (sql->prepare <<
                    "SELECT id, title, description, location, event_date, category, significance "
                    "FROM historical_events WHERE event_date BETWEEN :start AND :end ORDER BY event_date",
                    soci::use(startTimestamp), soci::use(endTimestamp));

                for (const auto& row : rows) {
                    HistoricalEvent event;
                    event.id = row.get<int>(0);
                    event.title = row.get<std::string>(1, "");
                    event.description = row.get<std::string>(2, "");
                    event.location = row.get<std::string>(3, "");
                    event.date = static_cast<time_t>(row.get<long long>(4, 0));
                    event.category = row.get<std::string>(5, "");
                    event.significance = row.get<int>(6, 0);

                    events.push_back(event);
                }

                return events;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error retrieving events by date range: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error retrieving events by date range: {}", e.what()));
            }
        }

        auto HistoricalDbManager::getEventsByLocation(const std::string& location) -> std::expected<std::vector<HistoricalEvent>, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                std::vector<HistoricalEvent> events;

                soci::rowset<soci::row> rows = (sql->prepare <<
                    "SELECT id, title, description, location, event_date, category, significance "
                    "FROM historical_events WHERE location = :location ORDER BY event_date",
                    soci::use(location));

                for (const auto& row : rows) {
                    HistoricalEvent event;
                    event.id = row.get<int>(0);
                    event.title = row.get<std::string>(1, "");
                    event.description = row.get<std::string>(2, "");
                    event.location = row.get<std::string>(3, "");
                    event.date = static_cast<time_t>(row.get<long long>(4, 0));
                    event.category = row.get<std::string>(5, "");
                    event.significance = row.get<int>(6, 0);

                    events.push_back(event);
                }

                return events;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error retrieving events by location: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error retrieving events by location: {}", e.what()));
            }
        }

        auto HistoricalDbManager::loadEventsToLinkedList() -> std::expected<HistoricalLinkedList, std::string> {
            auto eventsResult = getAllEvents();

            if (!eventsResult) {
                return std::unexpected(eventsResult.error());
            }

            HistoricalLinkedList list;
            for (const auto& event : eventsResult.value()) {
                list.insertAtEnd(event);
            }

            return std::expected<HistoricalLinkedList, std::string>(std::in_place, std::move(list));
        }

        auto HistoricalDbManager::saveLinkedListToDb(const HistoricalLinkedList& list) -> std::expected<bool, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                sql->begin();

                try {
                    (*sql) << "DELETE FROM historical_events";

                    HistoricalNode* current = list.getHead();

                    while (current != nullptr) {
                        const HistoricalEvent& event = current->data;
                        long long eventDate = static_cast<long long>(event.date);

                        if (event.id > 0) {
                            (*sql) << "INSERT INTO historical_events "
                                   "(id, title, description, location, event_date, category, significance) "
                                   "VALUES (:id, :title, :description, :location, :date, :category, :significance)",
                                   soci::use(event.id),
                                   soci::use(event.title),
                                   soci::use(event.description),
                                   soci::use(event.location),
                                   soci::use(eventDate),
                                   soci::use(event.category),
                                   soci::use(event.significance);
                        } else {
                            (*sql) << "INSERT INTO historical_events "
                                   "(title, description, location, event_date, category, significance) "
                                   "VALUES (:title, :description, :location, :date, :category, :significance)",
                                   soci::use(event.title),
                                   soci::use(event.description),
                                   soci::use(event.location),
                                   soci::use(eventDate),
                                   soci::use(event.category),
                                   soci::use(event.significance);
                        }

                        current = current->next;
                    }
                    sql->commit();
                    return true;
                } catch (const std::exception& e) {
                    sql->rollback();
                    throw;
                }
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error saving linked list: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error saving linked list: {}", e.what()));
            }
        }

    }
}
