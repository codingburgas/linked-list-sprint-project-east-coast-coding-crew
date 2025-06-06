#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <utility>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

#ifdef __APPLE__
	#define TIME_VAR time_t
#endif

#ifdef _WIN32
	#define TIME_VAR std::time_t 
#endif

namespace Eccc {
	namespace Core {
	TIME_VAR createDate(int year, int month, int day);
	std::pair<std::string, std::vector<std::string>> parseCommand(const std::string& input);

	}
}
