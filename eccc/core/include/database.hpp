#pragma once
#include <memory>
#include <expected>
#include <format>
#include <thread>
#include <future>
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
using NoReturn = std::expected<void, std::string>;
namespace Eccc {
	namespace Core {

		class Database {

			public:

				static std::future<std::expected<Database*, std::string>> getInstance() {
					try {
						std::call_once(database_flag, []() {
							databaseInstance = new Database();
							});

						return std::async(std::launch::async, []() -> std::expected<Database*, std::string> {
							return databaseInstance;
							});
					}
					catch (const std::exception& e) {
						return std::async(std::launch::async, [error = std::format("Error when creating database: {}", e.what())]()
							-> std::expected<Database*, std::string> {
								return std::unexpected(error);
							});
					}
				}

				Database(const Database&) = delete;
				Database& operator=(const Database&) = delete;
				soci::session sql;

				soci::session* getSession() {
					if (!sql.is_connected()) {
						throw std::runtime_error("Failed to connect to the database");
					}
					return &sql;
				}

				NoReturn connectToDb();


			private:
				
				static Database* databaseInstance;
				static std::once_flag database_flag;
				explicit Database() {};
				~Database() {
					delete databaseInstance;
				}



		};
	}
}