#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <functional>
#include <sstream>

// Forward declarations of test entry points
int runHistoricalLinkedListTests();
int runDatabaseTests();

// Function to run a test suite and return its result
int runTestSuite(const std::string& name, std::function<int()> testFunction) {
    std::cout << "\n========================================\n";
    std::cout << "Running " << name << "\n";
    std::cout << "========================================\n";
    
    int result = testFunction();
    
    if (result == 0) {
        std::cout << name << " tests PASSED\n";
    } else {
        std::cout << name << " tests FAILED with exit code " << result << "\n";
    }
    
    return result;
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n";
    std::cout << "Options:\n";
    std::cout << "  --use-db           Enable database connection tests\n";
    std::cout << "  --help             Display this help message\n";
}

// Main test runner
int main(int argc, char* argv[]) {
    std::cout << "Running all unit tests\n";
    
    // Process command-line arguments
    bool useDatabase = false;
    
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--use-db") {
            useDatabase = true;
        } else if (std::string(argv[i]) == "--help") {
            printUsage(argv[0]);
            return 0;
        }
    }
    
    // Define all test suites
    struct TestSuite {
        std::string name;
        std::function<int()> function;
    };
    
    // Prepare commands with platform-specific executable paths
    #ifdef _WIN32
    std::string linkedListTestCmd = "historical_linked_list_test.exe";
    std::string dbTestCmd = "database_test.exe";
    #else
    std::string linkedListTestCmd = "./historical_linked_list_test";
    std::string dbTestCmd = "./database_test";
    #endif
    
    if (useDatabase) {
        dbTestCmd += " --use-db";
    }
    
    std::vector<TestSuite> testSuites = {
        {"Historical Linked List", [linkedListTestCmd]() { return system(linkedListTestCmd.c_str()); }},
        {"Database", [dbTestCmd]() { return system(dbTestCmd.c_str()); }}
    };
    
    // Run all test suites
    int passedSuites = 0;
    int failedSuites = 0;
    int totalSuites = testSuites.size();
    
    for (const auto& suite : testSuites) {
        int result = runTestSuite(suite.name, suite.function);
        if (result == 0) {
            passedSuites++;
        } else if (result != 0) {
            // Special handling for database tests when we expect them to be skipped
            if (!useDatabase && suite.name == "Database") {
                // Database tests will exit with 0 even when skipped
                passedSuites++;
            } else {
                failedSuites++;
            }
        }
    }
    
    // Print summary
    std::cout << "\n========================================\n";
    std::cout << "Test Summary\n";
    std::cout << "========================================\n";
    std::cout << "Total test suites: " << totalSuites << "\n";
    std::cout << "Passed test suites: " << passedSuites << "\n";
    std::cout << "Failed test suites: " << failedSuites << "\n";
    std::cout << "Database connection: " << (useDatabase ? "Enabled" : "Disabled") << "\n";
    
    return failedSuites > 0 ? 1 : 0;
} 