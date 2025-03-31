#include "database.hpp"
#include "cli_manager.hpp"
#include <iostream>
#include <memory>

#ifdef _WIN32
	#include <Windows.h>
	#include <fcntl.h>
	#include <io.h>
	std::future<bool> enable() {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE)
			return std::async(std::launch::async, []() -> bool {
			return false;
				});

		SetConsoleOutputCP(CP_UTF8);

		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode))
			return std::async(std::launch::async, []() -> bool {
			return false;
				});

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode))
			return std::async(std::launch::async, []() -> bool {
			return false;
				});

		return std::async(std::launch::async, []() -> bool {
			return true;
			});
	}
#endif


using namespace Eccc::Core;
int main() {

	#ifdef _WIN32
		auto enabled = enable().get();
	#endif

	#ifdef __APPLE__
		bool enabled = true;
	#endif 

	if (enabled) {
		auto dbHandle = Database::getInstance().get();
		if (!dbHandle) {
			std::cerr << "Failed to get database instance: " << dbHandle.error() << "\n";
			return 1;
		}

		auto db = dbHandle.value();

		auto connectHandle = db->async_connectToDb();
		auto connectionResult = connectHandle.get();
		if (!connectionResult) {
			std::cerr << "Failed to connect to database: " << connectionResult.error() << "\n";
			return 1;
		}

		std::cout << "Connected to the database\n";

		CliManager cliManager(db);
		cliManager.run();
	}

}
