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

			using ASYNC_SINGLETON_RETURN = std::future<std::expected<Database*, std::string>>;
			using ASYNC_NoReturn = std::future<std::expected<void, std::string>>;

			public:

				static ASYNC_SINGLETON_RETURN getInstance() {
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

				soci::session& getSession() {
					if (!sql.is_connected()) {
						throw std::runtime_error("Failed to connect to the database");
					}
					return sql;
				}

				Database(const Database&) = delete;
				Database& operator=(const Database&) = delete;
				soci::session sql;

				// NON-ASYNC-METHOD FOR CONNECTING TO DB
				NoReturn connectToDb();
				
				// ASYNC-METHOD FOR CONNECTING TO DB 
				ASYNC_NoReturn async_connectToDb(); // (UPDATE MAIN TO HANDLE STD::FUTURE IN CASE OF USING THS)

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