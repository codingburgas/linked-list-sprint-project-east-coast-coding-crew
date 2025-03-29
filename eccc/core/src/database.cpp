#include "database.hpp"
#include <iostream>

Eccc::Core::Database* Eccc::Core::Database::databaseInstance  = nullptr;
std::once_flag Eccc::Core::Database::database_flag;

NoReturn Eccc::Core::Database::connectToDb() {
	try {
		sql.open(soci::postgresql, "dbname=niggers user=postgres password=daddy host=localhost port=5432");
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




