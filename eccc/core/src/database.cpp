#include "database.hpp"
#include <iostream>

Eccc::Core::Database* Eccc::Core::Database::databaseInstance  = nullptr;
std::once_flag Eccc::Core::Database::database_flag;

NoReturn Eccc::Core::Database::connectToDb() {
	try {
		sql.open(soci::postgresql, "dbname=main_db user=ecccAdmin password=schoolDb123_ host=eccc-db.postgres.database.azure.com port=5432");
		return {};
	}
	catch (const soci::soci_error& database_error) {
		return std::unexpected(std::format("An error connecting to the database happened: {}", database_error.what()));
	}
	catch (const std::runtime_error& standard_excpetion) {
		return std::unexpected(std::format("Some other expection: {}", standard_excpetion.what()));
	}
	catch (...) {
		return std::unexpected("Some other error");
	}
}


Eccc::Core::Database::ASYNC_NoReturn Eccc::Core::Database::async_connectToDb() {
	try {

		sql.open(soci::postgresql, "dbname=main_db user=ecccAdmin password=schoolDb123_ host=eccc-db.postgres.database.azure.com port=5432");
		return std::async(std::launch::async, []() -> NoReturn {
			return {};
		});
	}

	catch (const soci::soci_error& database_error) {
		return std::async(std::launch::async, [error = database_error]() -> NoReturn {
			return std::unexpected(std::format("An error connecting to the database happened: {}", error.what()));
		});
	}

	catch (const std::runtime_error& standard_excpetion) {
		return std::async(std::launch::async, [error = standard_excpetion]() -> NoReturn {
			return std::unexpected(std::format("Some other expection: {}", error.what()));
		});
	}

	catch (...) {
		return std::async(std::launch::async, []() -> NoReturn {
			return std::unexpected(std::format("General error."));
		});
	}
}




