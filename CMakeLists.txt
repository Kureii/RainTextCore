cmake_minimum_required(VERSION 3.22)

# Project name
set(PROJECT_NAME "RainTextCore")
project(${PROJECT_NAME} LANGUAGES CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add header files
include_directories(include)

add_subdirectory(lib)

find_path(CRYPTO++_INCLUDE_DIR cryptopp/cryptlib.h)
find_library(CRYPTO++_LIBRARY NAMES cryptopp libcryptopp)
find_package(Threads REQUIRED)

include_directories(${CRYPTO++_INCLUDE_DIR})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        ${CRYPTO++_LIBRARY}
        scryptlib::scrypt
        argon2lib::argon2
        )

# Add subdirectories with CMakeLists.txt
set(SOURCE_SUBDIR "src")
#set(TESTS_SUBDIR "tests")

add_subdirectory(${SOURCE_SUBDIR})
#add_subdirectory(${TESTS_SUBDIR})
