cmake_minimum_required(VERSION 3.20)

project(boap_sem_ii_cw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Source files ----
set(SOURCES
    src/main.cpp
    src/database/Database.cpp
)

add_executable(boap_app ${SOURCES})

# ---- SQLite ----
find_package(SQLite3 REQUIRED)

target_link_libraries(boap_app PRIVATE SQLite::SQLite3)

# ---- Include dirs ----
target_include_directories(boap_app PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
