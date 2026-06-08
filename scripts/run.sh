# @file run.sh
# @brief Build and run script for MarkForge CLI application.
#
# Compiles all C++ sources using clang++ and executes the binary.
#!/usr/bin/env bash

set -e

# ---------------- Project Paths ----------------

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build"
APP_NAME="markforge"

mkdir -p "$BUILD_DIR"

# ---------------- Source Discovery ----------------

CPP_FILES=$(find "$SRC_DIR" -type f -name "*.cpp")

if [ -z "$CPP_FILES" ]; then
    echo "No .cpp files found in $SRC_DIR"
    exit 1
fi

# ---------------- Build Step ----------------

clang++ -std=c++17 $CPP_FILES -o "$BUILD_DIR/$APP_NAME"  -lsqlite3

# ---------------- Run Application ----------------

"$BUILD_DIR/$APP_NAME" "$@"
