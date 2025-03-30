#include "historical_db_manager.hpp"
#include <soci/soci.h>
#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>

namespace Eccc {
    namespace Core {
        HistoricalDbManager::HistoricalDbManager(std::shared_ptr<Database> db) : dbConnection(db) {}

        auto HistoricalDbManager::setupSchema() -> std::expected<bool, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                bool tableExists = false;
                
                try {
                    (*sql) << "SELECT 1 FROM historical_events LIMIT 1";
                    tableExists = true;
                } catch (const soci::soci_error&) {

                }

                if (!tableExists) {
                    (*sql) << "CREATE TABLE IF NOT EXISTS historical_events ("
                           "id SERIAL PRIMARY KEY,"
                           "title VARCHAR(255) NOT NULL,"
                           "description TEXT,"
                           "location VARCHAR(255),"
                           "event_date BIGINT,"
                           "category VARCHAR(100),"
                           "significance INTEGER,"
                           "leader VARCHAR(255),"
                           "participants TEXT,"
                           "result TEXT"
                           ")";
                } else {
                    try {
                        (*sql) << "SELECT leader FROM historical_events LIMIT 1";
                    } catch (const soci::soci_error&) {
                        try {
                            (*sql) << "ALTER TABLE historical_events ADD COLUMN leader VARCHAR(255)";
                        } catch (const soci::soci_error& e) {
                            std::cerr << "Warning: Could not add leader column: " << e.what() << std::endl;
                        }
                    }
                    
                    try {
                        (*sql) << "SELECT participants FROM historical_events LIMIT 1";
                    } catch (const soci::soci_error&) {
                        try {
                            (*sql) << "ALTER TABLE historical_events ADD COLUMN participants TEXT";
                        } catch (const soci::soci_error& e) {
                            std::cerr << "Warning: Could not add participants column: " << e.what() << std::endl;
                        }
                    }
                    
                    try {
                        (*sql) << "SELECT result FROM historical_events LIMIT 1";
                    } catch (const soci::soci_error&) {
                        try {
                            (*sql) << "ALTER TABLE historical_events ADD COLUMN result TEXT";
                        } catch (const soci::soci_error& e) {
                            std::cerr << "Warning: Could not add result column: " << e.what() << std::endl;
                        }
                    }
                }

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
                       "(title, description, location, event_date, category, significance, leader, participants, result) "
                       "VALUES (:title, :description, :location, :date, :category, :significance, :leader, :participants, :result) "
                       "RETURNING id",
                       soci::use(event.title),
                       soci::use(event.description),
                       soci::use(event.location),
                       soci::use(eventDate),
                       soci::use(event.category),
                       soci::use(event.significance),
                       soci::use(event.leader),
                       soci::use(event.participants),
                       soci::use(event.result),
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
                std::string title, description, location, category, leader, participants, result;
                long long dateTimestamp;
                int significance;

                soci::indicator titleInd, descInd, locInd, dateInd, catInd, sigInd, leaderInd, partInd, resultInd;

                (*sql) << "SELECT id, title, description, location, event_date, "
                       "category, significance, leader, participants, result FROM historical_events WHERE id = :id",
                       soci::use(id),
                       soci::into(event.id),
                       soci::into(title, titleInd),
                       soci::into(description, descInd),
                       soci::into(location, locInd),
                       soci::into(dateTimestamp, dateInd),
                       soci::into(category, catInd),
                       soci::into(significance, sigInd),
                       soci::into(leader, leaderInd),
                       soci::into(participants, partInd),
                       soci::into(result, resultInd);

                if (sql->got_data()) {
                    event.title = (titleInd == soci::i_ok) ? title : "";
                    event.description = (descInd == soci::i_ok) ? description : "";
                    event.location = (locInd == soci::i_ok) ? location : "";
                    event.date = (dateInd == soci::i_ok) ? static_cast<time_t>(dateTimestamp) : 0;
                    event.category = (catInd == soci::i_ok) ? category : "";
                    event.significance = (sigInd == soci::i_ok) ? significance : 0;
                    event.leader = (leaderInd == soci::i_ok) ? leader : "";
                    event.participants = (partInd == soci::i_ok) ? participants : "";
                    event.result = (resultInd == soci::i_ok) ? result : "";

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
                       "category = :category, significance = :significance, "
                       "leader = :leader, participants = :participants, result = :result "
                       "WHERE id = :id",
                       soci::use(event.title),
                       soci::use(event.description),
                       soci::use(event.location),
                       soci::use(eventDate),
                       soci::use(event.category),
                       soci::use(event.significance),
                       soci::use(event.leader),
                       soci::use(event.participants),
                       soci::use(event.result),
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
                    "SELECT id, title, description, location, event_date, category, significance, leader, participants, result "
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
                    event.leader = row.get<std::string>(7, "");
                    event.participants = row.get<std::string>(8, "");
                    event.result = row.get<std::string>(9, "");

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
                    "SELECT id, title, description, location, event_date, category, significance, leader, participants, result "
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
                    event.leader = row.get<std::string>(7, "");
                    event.participants = row.get<std::string>(8, "");
                    event.result = row.get<std::string>(9, "");

                    events.push_back(event);
                }

                return events;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error retrieving events: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error retrieving events: {}", e.what()));
            }
        }

        auto HistoricalDbManager::getEventsByDateRange(time_t startDate, time_t endDate) -> std::expected<std::vector<HistoricalEvent>, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                std::vector<HistoricalEvent> events;

                long long startTimestamp = static_cast<long long>(startDate);
                long long endTimestamp = static_cast<long long>(endDate);

                soci::rowset<soci::row> rows = (sql->prepare <<
                    "SELECT id, title, description, location, event_date, category, significance, leader, participants, result "
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
                    event.leader = row.get<std::string>(7, "");
                    event.participants = row.get<std::string>(8, "");
                    event.result = row.get<std::string>(9, "");

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
                    "SELECT id, title, description, location, event_date, category, significance, leader, participants, result "
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
                    event.leader = row.get<std::string>(7, "");
                    event.participants = row.get<std::string>(8, "");
                    event.result = row.get<std::string>(9, "");

                    events.push_back(event);
                }

                return events;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error retrieving events by location: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error retrieving events by location: {}", e.what()));
            }
        }

        auto HistoricalDbManager::getEventsByResult(const std::string& resultPattern) -> std::expected<std::vector<HistoricalEvent>, std::string> {
            try {
                auto* sql = dbConnection->getSession();
                std::vector<HistoricalEvent> events;

                std::string likePattern = "%" + resultPattern + "%";

                soci::rowset<soci::row> rows = (sql->prepare <<
                    "SELECT id, title, description, location, event_date, category, significance, leader, participants, result "
                    "FROM historical_events WHERE result LIKE :result ORDER BY event_date",
                    soci::use(likePattern));

                for (const auto& row : rows) {
                    HistoricalEvent event;
                    event.id = row.get<int>(0);
                    event.title = row.get<std::string>(1, "");
                    event.description = row.get<std::string>(2, "");
                    event.location = row.get<std::string>(3, "");
                    event.date = static_cast<time_t>(row.get<long long>(4, 0));
                    event.category = row.get<std::string>(5, "");
                    event.significance = row.get<int>(6, 0);
                    event.leader = row.get<std::string>(7, "");
                    event.participants = row.get<std::string>(8, "");
                    event.result = row.get<std::string>(9, "");

                    events.push_back(event);
                }

                return events;
            } catch (const soci::soci_error& e) {
                return std::unexpected(std::format("Database error retrieving events: {}", e.what()));
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error retrieving events: {}", e.what()));
            }
        }

        auto HistoricalDbManager::exportAllEventTitles(const std::string& filename, ExportFormat format) -> std::expected<bool, std::string> {
            auto eventsResult = getAllEvents();
            if (!eventsResult) {
                return std::unexpected(eventsResult.error());
            }

            const auto& events = eventsResult.value();
            if (events.empty()) {
                return std::unexpected("No events to export");
            }

            std::ofstream file(filename);
            if (!file.is_open()) {
                return std::unexpected(std::format("Could not open file for writing: {}", filename));
            }

            try {
                switch (format) {
                    case ExportFormat::TEXT: {
                        file << "# List of Historical Event Titles\n\n";
                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            file << "- " << event.title << " (" << dateStr << ")\n";
                        }
                        break;
                    }
                    case ExportFormat::CSV: {
                        file << "ID,Title,Date\n";
                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            file << event.id << ",\"" << event.title << "\"," << dateStr << "\n";
                        }
                        break;
                    }
                    case ExportFormat::JSON: {
                        nlohmann::json jsonOutput;
                        jsonOutput["report_type"] = "All Event Titles";
                        jsonOutput["generated_at"] = time(nullptr);
                        nlohmann::json eventsJson = nlohmann::json::array();

                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            nlohmann::json eventJson;
                            eventJson["id"] = event.id;
                            eventJson["title"] = event.title;
                            eventJson["date"] = dateStr;
                            eventsJson.push_back(eventJson);
                        }
                        jsonOutput["events"] = eventsJson;
                        file << jsonOutput.dump(4);
                        break;
                    }
                }
                return true;
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error exporting events: {}", e.what()));
            }
        }

        auto HistoricalDbManager::exportEventsByCategory(const std::string& category, const std::string& filename, ExportFormat format) -> std::expected<bool, std::string> {
            auto eventsResult = getEventsByCategory(category);
            if (!eventsResult) {
                return std::unexpected(eventsResult.error());
            }

            const auto& events = eventsResult.value();
            if (events.empty()) {
                return std::unexpected(std::format("No events found for category: {}", category));
            }

            std::ofstream file(filename);
            if (!file.is_open()) {
                return std::unexpected(std::format("Could not open file for writing: {}", filename));
            }

            try {
                switch (format) {
                    case ExportFormat::TEXT: {
                        file << "# Historical Events in Category: " << category << "\n\n";
                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            file << "## " << event.title << " (" << dateStr << ")\n";
                            file << "Location: " << event.location << "\n";
                            file << "Description: " << event.description << "\n";
                            file << "Leader: " << event.leader << "\n";
                            file << "Participants: " << event.participants << "\n";
                            file << "Result: " << event.result << "\n\n";
                        }
                        break;
                    }
                    case ExportFormat::CSV: {
                        file << "ID,Title,Date,Location,Description,Leader,Participants,Result\n";
                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            file << event.id << ",\"" << event.title << "\","
                                 << dateStr << ",\"" << event.location << "\",\""
                                 << event.description << "\",\"" << event.leader << "\",\""
                                 << event.participants << "\",\"" << event.result << "\"\n";
                        }
                        break;
                    }
                    case ExportFormat::JSON: {
                        nlohmann::json jsonOutput;
                        jsonOutput["report_type"] = "Events by Category";
                        jsonOutput["category"] = category;
                        jsonOutput["generated_at"] = time(nullptr);
                        nlohmann::json eventsJson = nlohmann::json::array();

                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            nlohmann::json eventJson;
                            eventJson["id"] = event.id;
                            eventJson["title"] = event.title;
                            eventJson["date"] = dateStr;
                            eventJson["location"] = event.location;
                            eventJson["description"] = event.description;
                            eventJson["leader"] = event.leader;
                            eventJson["participants"] = event.participants;
                            eventJson["result"] = event.result;
                            eventsJson.push_back(eventJson);
                        }
                        jsonOutput["events"] = eventsJson;
                        file << jsonOutput.dump(4);
                        break;
                    }
                }
                return true;
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error exporting events: {}", e.what()));
            }
        }

        auto HistoricalDbManager::exportEventsByResult(const std::string& resultPattern, const std::string& filename, ExportFormat format) -> std::expected<bool, std::string> {
            auto eventsResult = getEventsByResult(resultPattern);
            if (!eventsResult) {
                return std::unexpected(eventsResult.error());
            }

            const auto& events = eventsResult.value();
            if (events.empty()) {
                return std::unexpected(std::format("No events found with result containing: {}", resultPattern));
            }

            std::ofstream file(filename);
            if (!file.is_open()) {
                return std::unexpected(std::format("Could not open file for writing: {}", filename));
            }

            try {
                switch (format) {
                    case ExportFormat::TEXT: {
                        file << "# Historical Events with Result Containing: " << resultPattern << "\n\n";
                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            file << "## " << event.title << " (" << dateStr << ")\n";
                            file << "Location: " << event.location << "\n";
                            file << "Category: " << event.category << "\n";
                            file << "Description: " << event.description << "\n";
                            file << "Leader: " << event.leader << "\n";
                            file << "Participants: " << event.participants << "\n";
                            file << "Result: " << event.result << "\n\n";
                        }
                        break;
                    }
                    case ExportFormat::CSV: {
                        file << "ID,Title,Date,Location,Category,Description,Leader,Participants,Result\n";
                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            file << event.id << ",\"" << event.title << "\","
                                 << dateStr << ",\"" << event.location << "\",\""
                                 << event.category << "\",\"" << event.description << "\",\""
                                 << event.leader << "\",\"" << event.participants << "\",\""
                                 << event.result << "\"\n";
                        }
                        break;
                    }
                    case ExportFormat::JSON: {
                        nlohmann::json jsonOutput;
                        jsonOutput["report_type"] = "Events by Result";
                        jsonOutput["result_pattern"] = resultPattern;
                        jsonOutput["generated_at"] = time(nullptr);
                        nlohmann::json eventsJson = nlohmann::json::array();

                        for (const auto& event : events) {
                            struct tm* timeInfo = localtime(&event.date);
                            char dateStr[11];
                            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
                            
                            nlohmann::json eventJson;
                            eventJson["id"] = event.id;
                            eventJson["title"] = event.title;
                            eventJson["date"] = dateStr;
                            eventJson["location"] = event.location;
                            eventJson["category"] = event.category;
                            eventJson["description"] = event.description;
                            eventJson["leader"] = event.leader;
                            eventJson["participants"] = event.participants;
                            eventJson["result"] = event.result;
                            eventsJson.push_back(eventJson);
                        }
                        jsonOutput["events"] = eventsJson;
                        file << jsonOutput.dump(4);
                        break;
                    }
                }
                return true;
            } catch (const std::exception& e) {
                return std::unexpected(std::format("Error exporting events: {}", e.what()));
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
                                   "(id, title, description, location, event_date, category, significance, leader, participants, result) "
                                   "VALUES (:id, :title, :description, :location, :date, :category, :significance, :leader, :participants, :result)",
                                   soci::use(event.id),
                                   soci::use(event.title),
                                   soci::use(event.description),
                                   soci::use(event.location),
                                   soci::use(eventDate),
                                   soci::use(event.category),
                                   soci::use(event.significance),
                                   soci::use(event.leader),
                                   soci::use(event.participants),
                                   soci::use(event.result);
                        } else {
                            (*sql) << "INSERT INTO historical_events "
                                   "(title, description, location, event_date, category, significance, leader, participants, result) "
                                   "VALUES (:title, :description, :location, :date, :category, :significance, :leader, :participants, :result)",
                                   soci::use(event.title),
                                   soci::use(event.description),
                                   soci::use(event.location),
                                   soci::use(eventDate),
                                   soci::use(event.category),
                                   soci::use(event.significance),
                                   soci::use(event.leader),
                                   soci::use(event.participants),
                                   soci::use(event.result);
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
