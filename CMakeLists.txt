# @file CMakeLists.txt
# @brief Build configuration for MarkForge CLI application.
#
# Defines compilation settings, source discovery,
# dependency management, and platform-specific linking.

cmake_minimum_required(VERSION 3.20)

project(markforge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- Source Collection ----------------
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# ---------------- Target Definition ----------------
add_executable(markforge ${SOURCES})

# ---------------- External Dependencies ----------------
find_package(SQLite3 REQUIRED)
target_link_libraries(markforge PRIVATE SQLite::SQLite3)

# ---------------- Include Paths ----------------
target_include_directories(markforge PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)