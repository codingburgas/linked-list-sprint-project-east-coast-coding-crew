#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "../include/historical_linked_list.hpp"

using namespace Eccc::Core;

// Simple test framework
#define TEST(name) void name()
#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cerr << "ASSERTION FAILED at " << __FILE__ << ":" << __LINE__ << "\n"; \
            std::cerr << "  Expected: " << (expected) << "\n"; \
            std::cerr << "  Actual: " << (actual) << "\n"; \
            throw std::runtime_error("Test assertion failed"); \
        } \
    } while (0)

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "ASSERTION FAILED at " << __FILE__ << ":" << __LINE__ << "\n"; \
            std::cerr << "  Expected: true\n"; \
            std::cerr << "  Actual: false\n"; \
            throw std::runtime_error("Test assertion failed"); \
        } \
    } while (0)

// Helper function to create a sample event
HistoricalEvent createSampleEvent(int id, const std::string& title) {
    return HistoricalEvent(
        id, title, "Description", "Location", 
        std::time(nullptr), "Category", 5, 
        "Leader", "Participants", "Result"
    );
}

// Helper function to create a sample event with specific date
HistoricalEvent createSampleEventWithDate(int id, const std::string& title, std::time_t date) {
    return HistoricalEvent(
        id, title, "Description", "Location", 
        date, "Category", 5, 
        "Leader", "Participants", "Result"
    );
}

// Helper to check if a linked list is properly sorted by given criteria
template<typename Comparator>
bool isSorted(HistoricalLinkedList& list, Comparator comp) {
    if (list.getSize() <= 1) return true;
    
    HistoricalNode* current = list.getHead();
    while (current->next != nullptr) {
        if (!comp(current->data, current->next->data)) {
            return false;
        }
        current = current->next;
    }
    return true;
}

// Tests for HistoricalLinkedList
TEST(testConstructor) {
    HistoricalLinkedList list;
    ASSERT_EQ(0, list.getSize());
    ASSERT_TRUE(list.isEmpty());
}

TEST(testInsertAtEnd) {
    HistoricalLinkedList list;
    list.insertAtEnd(createSampleEvent(1, "Event 1"));
    ASSERT_EQ(1, list.getSize());
    ASSERT_EQ(1, list.getHead()->data.id);
    ASSERT_EQ("Event 1", list.getHead()->data.title);
    
    list.insertAtEnd(createSampleEvent(2, "Event 2"));
    ASSERT_EQ(2, list.getSize());
    ASSERT_EQ(2, list.getTail()->data.id);
    ASSERT_EQ("Event 2", list.getTail()->data.title);
}

TEST(testInsertAtBeginning) {
    HistoricalLinkedList list;
    list.insertAtBeginning(createSampleEvent(1, "Event 1"));
    ASSERT_EQ(1, list.getSize());
    
    list.insertAtBeginning(createSampleEvent(2, "Event 2"));
    ASSERT_EQ(2, list.getSize());
    ASSERT_EQ(2, list.getHead()->data.id);
    ASSERT_EQ("Event 2", list.getHead()->data.title);
}

TEST(testFind) {
    HistoricalLinkedList list;
    list.insertAtEnd(createSampleEvent(1, "Event 1"));
    list.insertAtEnd(createSampleEvent(2, "Event 2"));
    
    HistoricalNode* node = list.find(2);
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(2, node->data.id);
    ASSERT_EQ("Event 2", node->data.title);
    
    node = list.find(3);
    ASSERT_TRUE(node == nullptr);
}

TEST(testDeleteNode) {
    HistoricalLinkedList list;
    list.insertAtEnd(createSampleEvent(1, "Event 1"));
    list.insertAtEnd(createSampleEvent(2, "Event 2"));
    list.insertAtEnd(createSampleEvent(3, "Event 3"));
    
    ASSERT_TRUE(list.deleteNode(2));
    ASSERT_EQ(2, list.getSize());
    ASSERT_TRUE(list.find(2) == nullptr);
    
    ASSERT_TRUE(list.deleteNode(1));
    ASSERT_EQ(1, list.getSize());
    ASSERT_EQ(3, list.getHead()->data.id);
    
    ASSERT_TRUE(list.deleteNode(3));
    ASSERT_EQ(0, list.getSize());
    ASSERT_TRUE(list.isEmpty());
    
    ASSERT_TRUE(!list.deleteNode(4));
}

TEST(testSortByDate) {
    HistoricalLinkedList list;
    
    // Add events with different dates (oldest to newest)
    std::time_t now = std::time(nullptr);
    std::time_t yesterday = now - 24*60*60;
    std::time_t lastWeek = now - 7*24*60*60;
    std::time_t lastMonth = now - 30*24*60*60;
    
    // Insert in random order
    list.insertAtEnd(createSampleEventWithDate(1, "Yesterday Event", yesterday));
    list.insertAtEnd(createSampleEventWithDate(2, "Last Month Event", lastMonth));
    list.insertAtEnd(createSampleEventWithDate(3, "Current Event", now));
    list.insertAtEnd(createSampleEventWithDate(4, "Last Week Event", lastWeek));
    
    // Sort by date
    list.sortByDate();
    
    // Verify order is by date (oldest to newest)
    HistoricalNode* current = list.getHead();
    ASSERT_EQ(2, current->data.id); // Last Month Event
    current = current->next;
    ASSERT_EQ(4, current->data.id); // Last Week Event
    current = current->next;
    ASSERT_EQ(1, current->data.id); // Yesterday Event
    current = current->next;
    ASSERT_EQ(3, current->data.id); // Current Event
    
    // Also verify using the helper function
    auto dateComparator = [](const HistoricalEvent& a, const HistoricalEvent& b) {
        return a.date <= b.date;
    };
    ASSERT_TRUE(isSorted(list, dateComparator));
}

TEST(testSortByTitle) {
    HistoricalLinkedList list;
    
    // Insert events with different titles in random order
    list.insertAtEnd(createSampleEvent(1, "World War II"));
    list.insertAtEnd(createSampleEvent(2, "American Revolution"));
    list.insertAtEnd(createSampleEvent(3, "Moon Landing"));
    list.insertAtEnd(createSampleEvent(4, "Fall of the Berlin Wall"));
    
    // Sort by title
    list.sortByTitle();
    
    // Verify order is alphabetical by title
    HistoricalNode* current = list.getHead();
    ASSERT_EQ(2, current->data.id); // American Revolution
    current = current->next;
    ASSERT_EQ(4, current->data.id); // Fall of the Berlin Wall
    current = current->next;
    ASSERT_EQ(3, current->data.id); // Moon Landing
    current = current->next;
    ASSERT_EQ(1, current->data.id); // World War II
    
    // Also verify using the helper function
    auto titleComparator = [](const HistoricalEvent& a, const HistoricalEvent& b) {
        return a.title <= b.title;
    };
    ASSERT_TRUE(isSorted(list, titleComparator));
}

TEST(testSortByCustomCriteria) {
    HistoricalLinkedList list;
    
    // Insert events with different significance values
    list.insertAtEnd(HistoricalEvent(1, "Event 1", "Description", "Location", std::time(nullptr), "Category", 8));
    list.insertAtEnd(HistoricalEvent(2, "Event 2", "Description", "Location", std::time(nullptr), "Category", 3));
    list.insertAtEnd(HistoricalEvent(3, "Event 3", "Description", "Location", std::time(nullptr), "Category", 10));
    list.insertAtEnd(HistoricalEvent(4, "Event 4", "Description", "Location", std::time(nullptr), "Category", 5));
    
    // Define custom sort criteria by significance (highest to lowest)
    auto significanceComparator = [](const HistoricalEvent& a, const HistoricalEvent& b) {
        return a.significance > b.significance;
    };
    
    // Sort using custom criteria
    list.sortBy(significanceComparator);
    
    // Verify order is by significance (highest to lowest)
    HistoricalNode* current = list.getHead();
    ASSERT_EQ(3, current->data.id); // Significance 10
    current = current->next;
    ASSERT_EQ(1, current->data.id); // Significance 8
    current = current->next;
    ASSERT_EQ(4, current->data.id); // Significance 5
    current = current->next;
    ASSERT_EQ(2, current->data.id); // Significance 3
    
    // Also verify using the helper function
    ASSERT_TRUE(isSorted(list, significanceComparator));
}

// Run all tests
void runAllTests() {
    std::vector<std::pair<std::string, void(*)()>> tests = {
        {"testConstructor", testConstructor},
        {"testInsertAtEnd", testInsertAtEnd},
        {"testInsertAtBeginning", testInsertAtBeginning},
        {"testFind", testFind},
        {"testDeleteNode", testDeleteNode},
        {"testSortByDate", testSortByDate},
        {"testSortByTitle", testSortByTitle},
        {"testSortByCustomCriteria", testSortByCustomCriteria}
    };
    
    int passed = 0;
    int failed = 0;
    
    for (const auto& [name, test] : tests) {
        try {
            std::cout << "Running " << name << "... ";
            test();
            std::cout << "PASSED\n";
            passed++;
        } catch (const std::exception& e) {
            std::cout << "FAILED: " << e.what() << "\n";
            failed++;
        }
    }
    
    std::cout << "\nTest summary:\n";
    std::cout << "  Passed: " << passed << "\n";
    std::cout << "  Failed: " << failed << "\n";
    std::cout << "  Total:  " << tests.size() << "\n";
    
    if (failed > 0) {
        std::exit(1);
    }
}

int main() {
    std::cout << "Running Historical Linked List Tests\n";
    runAllTests();
    return 0;
} 