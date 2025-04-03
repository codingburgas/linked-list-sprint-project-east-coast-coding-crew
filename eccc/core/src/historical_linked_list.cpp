#include "historical_linked_list.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

namespace Eccc {
    namespace Core {
       HistoricalLinkedList::HistoricalLinkedList() : head(nullptr), tail(nullptr), size(0) {};
        HistoricalLinkedList::~HistoricalLinkedList() {
            clear();
        }

        HistoricalLinkedList::HistoricalLinkedList(HistoricalLinkedList&& other) noexcept
            : head(other.head), tail(other.tail), size(other.size) {
            other.head = nullptr;
            other.tail = nullptr;
            other.size = 0;
        }

        HistoricalLinkedList& HistoricalLinkedList::operator=(HistoricalLinkedList&& other) noexcept {
            if (this != &other) {
                clear();

                head = other.head;
                tail = other.tail;
                size = other.size;

                other.head = nullptr;
                other.tail = nullptr;
                other.size = 0;
            }
            return *this;
        }

        void HistoricalLinkedList::insertAtEnd(const HistoricalEvent& event) {
            HistoricalNode* newNode = new HistoricalNode(event);

            if (isEmpty()) {
                head = tail = newNode;
            } else {
                newNode->prev = tail;
                tail->next = newNode;
                tail = newNode;
            }

            size++;
        }

        void HistoricalLinkedList::insertAtBeginning(const HistoricalEvent& event) {
            HistoricalNode* newNode = new HistoricalNode(event);

            if (isEmpty()) {
                head = tail = newNode;
            } else {
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }

            size++;
        }

        void HistoricalLinkedList::insertAfter(HistoricalNode* prevNode, const HistoricalEvent& event) {
            if (prevNode == nullptr) {
                std::cerr << "The given previous node cannot be NULL" << std::endl;
                return;
            }

            HistoricalNode* newNode = new HistoricalNode(event);

            if (prevNode == tail) {
                prevNode->next = newNode;
                newNode->prev = prevNode;
                tail = newNode;
            } else {
                newNode->next = prevNode->next;
                newNode->prev = prevNode;
                prevNode->next->prev = newNode;
                prevNode->next = newNode;
            }

            size++;
        }

        void HistoricalLinkedList::insertByDate(const HistoricalEvent& event) {
            if (isEmpty() || event.date < head->data.date) {
                insertAtBeginning(event);
                return;
            }
            
            if (event.date >= tail->data.date) {
                insertAtEnd(event);
                return;
            }
            
            HistoricalNode* current = head;
            while (current != nullptr && current->data.date <= event.date) {
                current = current->next;
            }
            
            if (current == nullptr) {
                insertAtEnd(event);
            } else {
                HistoricalNode* newNode = new HistoricalNode(event);
                newNode->next = current;
                newNode->prev = current->prev;
                current->prev->next = newNode;
                current->prev = newNode;
                size++;
            }
        }

        bool HistoricalLinkedList::deleteNode(int id) {
            if (isEmpty()) {
                return false;
            }

            HistoricalNode* current = head;

            if (current->data.id == id) {
                head = current->next;

                if (head != nullptr) {
                    head->prev = nullptr;
                } else {
                    tail = nullptr;
                }

                delete current;
                size--;
                return true;
            }

            while (current != nullptr && current->data.id != id) {
                current = current->next;
            }

            if (current == nullptr) {
                return false;
            }

            if (current == tail) {
                tail = current->prev;
                tail->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            delete current;
            size--;
            return true;
        }

        HistoricalNode* HistoricalLinkedList::find(int id) {
            HistoricalNode* current = head;

            while (current != nullptr) {
                if (current->data.id == id) {
                    return current;
                }
                current = current->next;
            }

            return nullptr;
        }

        HistoricalNode* HistoricalLinkedList::findByTitle(const std::string& title) {
            HistoricalNode* current = head;

            while (current != nullptr) {
                if (current->data.title == title) {
                    return current;
                }
                current = current->next;
            }

            return nullptr;
        }

        HistoricalNode* HistoricalLinkedList::findByLocation(const std::string& location) {
            HistoricalNode* current = head;

            while (current != nullptr) {
                if (current->data.location == location) {
                    return current;
                }
                current = current->next;
            }

            return nullptr;
        }

        HistoricalNode* HistoricalLinkedList::findByCategory(const std::string& category) {
            HistoricalNode* current = head;

            while (current != nullptr) {
                if (current->data.category == category) {
                    return current;
                }
                current = current->next;
            }

            return nullptr;
        }

        void HistoricalLinkedList::sortByDate() {
            // Add debug output to understand the list content before sorting
            // std::cout << "Debug: Sorting list by date, current order:\n";
            // HistoricalNode* current = head;
            // while (current != nullptr) {
            //     std::cout << "  ID: " << current->data.id 
            //               << ", Title: " << current->data.title 
            //               << ", Date timestamp: " << current->data.date 
            //               << ", Formatted: " << formatDateSafe(current->data.date) << "\n";
            //     current = current->next;
            // }

            // Sort by chronological order (oldest to newest)
            // We need to decode date values properly:
            // For ancient dates in YYYYMMDD format, decode them to get year
            // For Unix timestamps, convert them to year too
            // Then sort by decoded year
            sortBy([](const HistoricalEvent& a, const HistoricalEvent& b) {
                // For debugging the actual values being compared
                // std::cout << "  Comparing dates: " << a.date << " vs " << b.date 
                //           << " (Events: " << a.title << " vs " << b.title << ")\n";
                
                // Extract year from negative YYYYMMDD value or from Unix timestamp
                int yearA, yearB;
                
                // For negative values (ancient dates in YYYYMMDD format)
                if (a.date < 0) {
                    yearA = std::abs(a.date) / 10000; // Extract year part (e.g., -19690720 -> 1969)
                } else {
                    // For standard Unix timestamps
                    time_t timestamp = a.date;
                    struct tm* timeinfo = localtime(&timestamp);
                    yearA = timeinfo ? (timeinfo->tm_year + 1900) : 0;
                }
                
                if (b.date < 0) {
                    yearB = std::abs(b.date) / 10000; // Extract year part
                } else {
                    time_t timestamp = b.date;
                    struct tm* timeinfo = localtime(&timestamp);
                    yearB = timeinfo ? (timeinfo->tm_year + 1900) : 0;
                }
                
                // Primary sort by year
                if (yearA != yearB) {
                    return yearA < yearB; // Older years first
                }
                
                // If years are equal, sort by the full date
                if (a.date < 0 && b.date < 0) {
                    // For two ancient dates, compare the encoded values
                    // Smaller encoded value = older date (e.g., -6810408 has a smaller year than -19690720)
                    return std::abs(a.date) < std::abs(b.date);
                } else if (a.date < 0 && b.date > 0) {
                    // Ancient date comes before Unix timestamp if same year
                    // (Shouldn't really happen as Unix timestamps start from 1970)
                    return true;
                } else if (a.date > 0 && b.date < 0) {
                    // Unix timestamp comes after ancient date if same year
                    return false;
                } else {
                    // Both are Unix timestamps - normal comparison
                    return a.date < b.date;
                }
            });
            
            // Add debug output to verify the sorted order
            // std::cout << "Debug: After sorting by date, new order:\n";
            // current = head;
            // while (current != nullptr) {
            //     std::cout << "  ID: " << current->data.id 
            //               << ", Title: " << current->data.title 
            //               << ", Date timestamp: " << current->data.date 
            //               << ", Formatted: " << formatDateSafe(current->data.date) << "\n";
            //     current = current->next;
            // }
        }

        void HistoricalLinkedList::sortByTitle() {
            sortBy([](const HistoricalEvent& a, const HistoricalEvent& b) {
                return a.title < b.title;
            });
        }

        void HistoricalLinkedList::sortBy(std::function<bool(const HistoricalEvent&, const HistoricalEvent&)> compare) {
            if (isEmpty() || size == 1) {
                return;
            }

            std::vector<HistoricalEvent> events;
            HistoricalNode* current = head;

            while (current != nullptr) {
                events.push_back(current->data);
                current = current->next;
            }

            std::sort(events.begin(), events.end(), compare);
            clear();

            for (const auto& event : events) {
                insertAtEnd(event);
            }
        }

        std::size_t HistoricalLinkedList::getSize() const {
            return size;
        }

        bool HistoricalLinkedList::isEmpty() const {
            return head == nullptr;
        }

        void HistoricalLinkedList::display() const {
            if (isEmpty()) {
                std::cout << "The list is empty." << std::endl;
                return;
            }

            HistoricalNode* current = head;
            while (current != nullptr) {
                const HistoricalEvent& event = current->data;
                std::cout << "ID: " << event.id << std::endl;
                std::cout << "Title: " << event.title << std::endl;
                std::cout << "Description: " << event.description << std::endl;
                std::cout << "Location: " << event.location << std::endl;
                std::cout << "Date: " << formatDateSafe(event.date) << std::endl;
                std::cout << "Category: " << event.category << std::endl;
                std::cout << "Significance: " << event.significance << std::endl;
                std::cout << "Leader: " << event.leader << std::endl;
                std::cout << "Participants: " << event.participants << std::endl;
                std::cout << "Result: " << event.result << std::endl;
                std::cout << "------------------------" << std::endl;

                current = current->next;
            }
        }

        std::string HistoricalLinkedList::formatDateSafe(time_t date) {
            char timeBuffer[80];
            
            if (date < 0) {
                long long encoded = -date;
                int year = encoded / 10000;
                int month = (encoded % 10000) / 100;
                int day = encoded % 100;
                
                sprintf(timeBuffer, "%04d-%02d-%02d", year, month, day);
            } else {
                struct tm* timeInfo = localtime(&date);
                if (timeInfo != nullptr) {
                    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d", timeInfo);
                } else {
                    strcpy(timeBuffer, "1970-01-01");
                }
            }
            
            return std::string(timeBuffer);
        }

        void HistoricalLinkedList::clear() {
            HistoricalNode* current = head;
            HistoricalNode* next = nullptr;

            while (current != nullptr) {
                next = current->next;
                delete current;
                current = next;
            }

            head = tail = nullptr;
            size = 0;
        }

        HistoricalNode* HistoricalLinkedList::getHead() const {
            return head;
        }

        HistoricalNode* HistoricalLinkedList::getTail() const {
            return tail;
        }
    }
}
