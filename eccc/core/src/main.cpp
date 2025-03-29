#include "database.hpp"
#include <iostream>
using Eccc::Core::Database;

auto instance = Eccc::Core::Database::getInstance();
auto handle = std::make_shared<std::expected<Database*, std::string>>(instance.get());

int main() {
	if (handle) {
		if (handle->has_value()) {
			Database* db = handle->value();
			auto res = db->connectToDb();
			if (!res.has_value()) {
				std::cout << std::format("db connection failed: {} ", res.error()) << std::endl;
				return 1;
			}

			std::cout << "successfully connected to the db." << std::endl;
		}
	}
}