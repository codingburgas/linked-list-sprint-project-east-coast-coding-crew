cmake_minimum_required(VERSION 3.20)

set(WORKSPACE_NAME "Eccc")
set(PROJECT_NAME "Core")
set(DEP_FOLDER "dependencies")

# Add option for enabling database tests
option(ENABLE_DB_TESTS "Enable database connection for unit tests" OFF)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Main executable
add_executable(${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES "eccc/Core/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "eccc/Core/include/*.h" "eccc/Core/include/*.hpp")

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES} ${HEADER_FILES})

# Test executables
add_executable(historical_linked_list_test
    eccc/Core/tests/historical_linked_list_test.cpp
    eccc/Core/src/historical_linked_list.cpp
)

add_executable(database_test
    eccc/Core/tests/database_test.cpp
    eccc/Core/src/database.cpp
)

add_executable(unitTests
    eccc/Core/tests/run_tests.cpp
)

# Set common include directories
set(COMMON_INCLUDE_DIRS
    ${DEP_FOLDER}/libpq/include
    ${DEP_FOLDER}/openssl/include
    ${DEP_FOLDER}/cpr/include
    ${DEP_FOLDER}/fmt/include
    ${DEP_FOLDER}/ftxui/include
    ${DEP_FOLDER}/wfrest/base
    ${DEP_FOLDER}/wfrest/core
    ${DEP_FOLDER}/wfrest/util
    ${DEP_FOLDER}/nlohmann
    ${DEP_FOLDER}/sqlite_orm
    ${DEP_FOLDER}/websocketpp
    ${CMAKE_SOURCE_DIR}/${WORKSPACE_NAME}/${PROJECT_NAME}/include
)

# Apply include directories to all executables
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(historical_linked_list_test PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(database_test PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(unitTests PRIVATE ${COMMON_INCLUDE_DIRS})

if(APPLE)
    set(APPLE_INCLUDE_DIRS
        /usr/local/include
        /opt/homebrew/include
        /opt/homebrew/include/postgresql@14
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE ${APPLE_INCLUDE_DIRS})
    target_include_directories(historical_linked_list_test PRIVATE ${APPLE_INCLUDE_DIRS})
    target_include_directories(database_test PRIVATE ${APPLE_INCLUDE_DIRS})
    target_include_directories(unitTests PRIVATE ${APPLE_INCLUDE_DIRS})
endif()

if(WIN32)
    set(WIN32_LINK_DIRS
        ${DEP_FOLDER}/libpq/lib
        ${DEP_FOLDER}/openssl/lib
        ${DEP_FOLDER}/fmt/src
        ${DEP_FOLDER}/ftxui/src
        ${DEP_FOLDER}/cpr/libs/Debug-libs
        ${DEP_FOLDER}/cpr/libs/Release-libs
    )
    
    set(WIN32_LIBS
        libpq
        libssl
        libcrypto
        fmt
        ftxui
        cpr
        ws2_32
    )
    
    link_directories(${WIN32_LINK_DIRS})
    
    target_link_libraries(${PROJECT_NAME} PRIVATE ${WIN32_LIBS})
    target_link_libraries(historical_linked_list_test PRIVATE ${WIN32_LIBS})
    target_link_libraries(database_test PRIVATE ${WIN32_LIBS})
endif()

set(FTXUI_DIR "${CMAKE_SOURCE_DIR}/${DEP_FOLDER}/ftxui")
if(EXISTS "${FTXUI_DIR}" AND IS_DIRECTORY "${FTXUI_DIR}")
    set(FTXUI_SOURCES "")
    file(GLOB_RECURSE FTXUI_DOM_SOURCES "${FTXUI_DIR}/src/dom/*.cpp")
    file(GLOB_RECURSE FTXUI_SCREEN_SOURCES "${FTXUI_DIR}/src/screen/*.cpp")
    file(GLOB_RECURSE FTXUI_COMPONENT_SOURCES "${FTXUI_DIR}/src/component/*.cpp")
    list(APPEND FTXUI_SOURCES ${FTXUI_DOM_SOURCES} ${FTXUI_SCREEN_SOURCES} ${FTXUI_COMPONENT_SOURCES})
    
    target_sources(${PROJECT_NAME} PRIVATE ${FTXUI_SOURCES})
    target_sources(historical_linked_list_test PRIVATE ${FTXUI_SOURCES})
    target_sources(database_test PRIVATE ${FTXUI_SOURCES})
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:easteregg_DEBUG>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_LIBRARIES $<$<CONFIG:Debug>:libcurl-d_imp cpr>)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>:easteregg_RELEASE>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_LIBRARIES $<$<CONFIG:Release>:libcurl_imp cpr>)

if(APPLE)
    set(APPLE_LINK_DIRS
        /usr/local/lib
        /opt/homebrew/lib
        /opt/homebrew/lib/postgresql@14
    )
    
    set(APPLE_LIBS
        soci_core
        soci_sqlite3
        soci_postgresql
        pq
    )
    
    target_link_directories(${PROJECT_NAME} PRIVATE ${APPLE_LINK_DIRS})
    target_link_directories(historical_linked_list_test PRIVATE ${APPLE_LINK_DIRS})
    target_link_directories(database_test PRIVATE ${APPLE_LINK_DIRS})
    target_link_directories(unitTests PRIVATE ${APPLE_LINK_DIRS})
    
    target_link_libraries(${PROJECT_NAME} PRIVATE ${APPLE_LIBS})
    target_link_libraries(historical_linked_list_test PRIVATE ${APPLE_LIBS})
    target_link_libraries(database_test PRIVATE ${APPLE_LIBS})
endif()

# Determine if we should run tests with database connection
set(TEST_CMD "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unitTests")
if(ENABLE_DB_TESTS)
    set(TEST_CMD "${TEST_CMD} --use-db")
endif()

# Make unitTests run automatically when built
if(UNIX)
    add_custom_command(TARGET unitTests 
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E echo "Running unit tests..."
                     COMMAND ${CMAKE_COMMAND} -E echo "Database tests: ${ENABLE_DB_TESTS}")
                     
    if(ENABLE_DB_TESTS)
        add_custom_command(TARGET unitTests 
                         POST_BUILD
                         COMMAND sh -c "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unitTests --use-db"
                         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                         COMMENT "Executing unit tests with database connection"
                         VERBATIM)
    else()
        add_custom_command(TARGET unitTests 
                         POST_BUILD
                         COMMAND sh -c "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unitTests"
                         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                         COMMENT "Executing unit tests without database connection"
                         VERBATIM)
    endif()
endif()