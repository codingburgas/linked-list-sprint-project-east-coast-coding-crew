#include <vector>

#ifdef __APPLE__
    #include "utilities.hpp"
#endif

#ifdef _WIN32
    #include "../include/utilities.hpp"
#endif

namespace Eccc {
    namespace Core {

TIME_VAR createDate(int year, int month, int day) {
    if (year >= 1970) {
        struct tm timeInfo = {};
        timeInfo.tm_year = year - 1900;
        timeInfo.tm_mon = month - 1;
        timeInfo.tm_mday = day;
        timeInfo.tm_hour = 12;
        timeInfo.tm_isdst = -1;
        
        return mktime(&timeInfo);
    } else {
        long long encoded = year * 10000 + month * 100 + day;
        return -encoded;
    }
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
