#include "utilities.hpp"

namespace Eccc {
namespace Core {

time_t createDate(int year, int month, int day) {
    struct tm timeInfo = {};
    timeInfo.tm_year = year - 1900;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    return mktime(&timeInfo);
}

std::pair<std::string, std::vector<std::string>> parseCommand(const std::string& input) {
    std::string command;
    std::vector<std::string> args;

    std::string current;
    bool inQuotes = false;

    for (char c : input) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ' ' && !inQuotes) {
            if (!current.empty()) {
                if (command.empty()) {
                    command = current;
                } else {
                    args.push_back(current);
                }
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        if (command.empty()) {
            command = current;
        } else {
            args.push_back(current);
        }
    }

    return {command, args};
}

}
}
