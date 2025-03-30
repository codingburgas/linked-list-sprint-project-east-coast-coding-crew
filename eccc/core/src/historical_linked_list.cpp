#include "historical_linked_list.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

namespace Eccc {
    namespace Core {
        HistoricalLinkedList::HistoricalLinkedList() : head(nullptr), tail(nullptr), size(0) {}
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
            sortBy([](const HistoricalEvent& a, const HistoricalEvent& b) {
                return a.date < b.date;
            });
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

        int HistoricalLinkedList::getSize() const {
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

                char timeBuffer[80];
                struct tm* timeInfo = localtime(&event.date);
                strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d", timeInfo);

                std::cout << "Date: " << timeBuffer << std::endl;
                std::cout << "Category: " << event.category << std::endl;
                std::cout << "Significance: " << event.significance << std::endl;
                std::cout << "Leader: " << event.leader << std::endl;
                std::cout << "Participants: " << event.participants << std::endl;
                std::cout << "Result: " << event.result << std::endl;
                std::cout << "------------------------" << std::endl;

                current = current->next;
            }
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
