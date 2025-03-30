#include "database.hpp"
#include "cli_manager.hpp"
#include <iostream>
#include <memory>

using namespace Eccc::Core;

int main() {
	auto dbHandle = Database::getInstance().get();
	if (!dbHandle) {
		std::cerr << "Failed to get database instance: " << dbHandle.error() << "\n";
		return 1;
	}

	std::shared_ptr<Database> db(dbHandle.value(), [](Database*){});

	auto connectionResult = db->connectToDb();
	if (!connectionResult) {
		std::cerr << "Failed to connect to database: " << connectionResult.error() << "\n";
		return 1;
	}

	std::cout << "Connected to the database\n";

	CliManager cliManager(db);
	cliManager.run();

	return 0;
}
