cmake_minimum_required(VERSION 3.10)

set(WORKSPACE_NAME "Eccc")
set(PROJECT_NAME "Core")
set(DEP_FOLDER "dependencies")

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB_RECURSE SRC_FILES "eccc/Core/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "eccc/Core/include/*.h" "eccc/Core/include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

#target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
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

if(APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE
        /usr/local/include
        /opt/homebrew/include
        /opt/homebrew/include/postgresql@14
    )
endif()

if(WIN32)
    link_directories(
        ${DEP_FOLDER}/libpq/lib
        ${DEP_FOLDER}/openssl/lib
        ${DEP_FOLDER}/fmt/src
        ${DEP_FOLDER}/ftxui/src
        ${DEP_FOLDER}/cpr/libs/Debug-libs
        ${DEP_FOLDER}/cpr/libs/Release-libs
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        libpq
        libssl
        libcrypto
        fmt
        ftxui
        cpr
        ws2_32
    )
endif()

set(FTXUI_DIR "${CMAKE_SOURCE_DIR}/${DEP_FOLDER}/ftxui")
if(EXISTS "${FTXUI_DIR}" AND IS_DIRECTORY "${FTXUI_DIR}")
    set(FTXUI_SOURCES "")
    file(GLOB_RECURSE FTXUI_DOM_SOURCES "${FTXUI_DIR}/src/dom/*.cpp")
    file(GLOB_RECURSE FTXUI_SCREEN_SOURCES "${FTXUI_DIR}/src/screen/*.cpp")
    file(GLOB_RECURSE FTXUI_COMPONENT_SOURCES "${FTXUI_DIR}/src/component/*.cpp")
    list(APPEND FTXUI_SOURCES ${FTXUI_DOM_SOURCES} ${FTXUI_SCREEN_SOURCES} ${FTXUI_COMPONENT_SOURCES})
    
    target_sources(${PROJECT_NAME} PRIVATE ${FTXUI_SOURCES})
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:easteregg_DEBUG>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_LIBRARIES $<$<CONFIG:Debug>:libcurl-d_imp cpr>)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>:easteregg_RELEASE>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_LIBRARIES $<$<CONFIG:Release>:libcurl_imp cpr>)

if(APPLE)
    target_link_directories(${PROJECT_NAME} PRIVATE
        /usr/local/lib
        /opt/homebrew/lib
        /opt/homebrew/lib/postgresql@14
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE soci_core soci_sqlite3 soci_postgresql pq)
endif()
