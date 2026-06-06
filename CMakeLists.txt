cmake_minimum_required(VERSION 3.20)

project(markforge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Gather ALL sources ----
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(markforge ${SOURCES})

# ---- SQLite ----
find_package(SQLite3 REQUIRED)
target_link_libraries(markforge PRIVATE SQLite::SQLite3)

# ---- Include dirs ----
target_include_directories(markforge PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)