cmake_minimum_required(VERSION 3.20)

set(WORKSPACE_NAME "Eccc")
set(PROJECT_NAME "Core")
set(DEP_FOLDER "dependencies")

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES "Eccc/Core/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "Eccc/Core/include/*.h" "Eccc/Core/include/*.hpp")

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DEP_FOLDER}/libpq/include
    ${DEP_FOLDER}/openssl/include
    ${DEP_FOLDER}/cpr/curl/include
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

link_directories(
    ${DEP_FOLDER}/libpq/lib
    ${DEP_FOLDER}/openssl/lib
    ${DEP_FOLDER}/fmt/src
    ${DEP_FOLDER}/ftxui/src
    ${DEP_FOLDER}/cpr/libs/Debug-libs
    ${DEP_FOLDER}/cpr/libs/Release-libs
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    libpq
    libssl
    libcrypto
    fmt
    ftxui
    cpr
)

# Configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:easteregg_DEBUG>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_LIBRARIES $<$<CONFIG:Debug>:libcurl-d_imp cpr>)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>:easteregg_RELEASE>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_LIBRARIES $<$<CONFIG:Release>:libcurl_imp cpr>)
