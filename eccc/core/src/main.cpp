#include "database.hpp"
#include <iostream>
using Eccc::Core::Database;
using ASYNC_NoReturn = std::future<std::expected<void, std::string>>;

auto instance = Eccc::Core::Database::getInstance(); // this is not a smart pointer to avoid double converting in the next line from a normal to a smart pointer
auto databaseHandle = std::make_shared<std::expected<Database*, std::string>>(instance.get());
std::shared_ptr<ASYNC_NoReturn> generealFutureHandle;

int main() {

	if (!databaseHandle || !databaseHandle->has_value()) {
		std::cout << (databaseHandle->has_value() ? databaseHandle->error() : "error");
		return 1;
	}

	Database* db = databaseHandle->value();
	generealFutureHandle = std::make_shared<ASYNC_NoReturn>(db->async_connectToDb());
	auto res = generealFutureHandle.get()->get();

	if (!res.has_value()) {
		std::cout << std::format("db connection failed: {} ", res.error()) << std::endl;
		return 1;
	}

	std::cout << "successfully connected to the db." << std::endl;

}