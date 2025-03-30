#pragma once
#include <string>
#include <ctime>
#include <memory>
#include <functional>

namespace Eccc {
    namespace Core {
        struct HistoricalEvent {
            int id;
            std::string title;
            std::string description;
            std::string location;
            std::time_t date;
            std::string category;
            int significance;
            std::string leader;
            std::string participants;
            std::string result;

            HistoricalEvent(int id, const std::string& title, const std::string& description,
                           const std::string& location, std::time_t date,
                           const std::string& category, int significance,
                           const std::string& leader = "", const std::string& participants = "", 
                           const std::string& result = "")
                : id(id), title(title), description(description), location(location),
                  date(date), category(category), significance(significance),
                  leader(leader), participants(participants), result(result) {}

            HistoricalEvent() : id(0), date(0), significance(0) {}
        };

        struct HistoricalNode {
            HistoricalEvent data;
            HistoricalNode* next;
            HistoricalNode* prev;

            explicit HistoricalNode(const HistoricalEvent& event)
                : data(event), next(nullptr), prev(nullptr) {}
        };

        class HistoricalLinkedList {
        private:
            HistoricalNode* head;
            HistoricalNode* tail;
            int size;

        public:
            HistoricalLinkedList();
            ~HistoricalLinkedList();

            HistoricalLinkedList(const HistoricalLinkedList&) = delete;
            HistoricalLinkedList& operator=(const HistoricalLinkedList&) = delete;

            HistoricalLinkedList(HistoricalLinkedList&& other) noexcept;
            HistoricalLinkedList& operator=(HistoricalLinkedList&& other) noexcept;

            void insertAtEnd(const HistoricalEvent& event);
            void insertAtBeginning(const HistoricalEvent& event);
            void insertAfter(HistoricalNode* prevNode, const HistoricalEvent& event);
            bool deleteNode(int id);
            HistoricalNode* find(int id);

            void sortByDate();
            void sortByTitle();
            void sortBy(std::function<bool(const HistoricalEvent&, const HistoricalEvent&)> compare);

            HistoricalNode* findByTitle(const std::string& title);
            HistoricalNode* findByLocation(const std::string& location);
            HistoricalNode* findByCategory(const std::string& category);

            int getSize() const;
            bool isEmpty() const;
            void display() const;
            void clear();

            HistoricalNode* getHead() const;
            HistoricalNode* getTail() const;
        };
    }
}
