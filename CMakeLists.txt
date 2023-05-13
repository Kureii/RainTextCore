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

add_subdirectory(lib EXCLUDE_FROM_ALL)

find_path(CRYPTO++_INCLUDE_DIR cryptopp/cryptlib.h)
find_library(CRYPTO++_LIBRARY NAMES cryptopp libcryptopp)
find_package(Threads REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/lib/argon2/include" "${CMAKE_SOURCE_DIR}/lib/scrypt")


include_directories(${CRYPTO++_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/core/rain_text_core.cpp
        include/core/rain_text_core.h
        src/utils/rain_text_core_utils.cpp
        src/utils/cipher/aes.cpp
        src/utils/cipher/chacha20.cpp
        include/utils/cipher/aes.h
        include/utils/cipher/chacha20.h
        )

target_link_libraries(${PROJECT_NAME}
        ${CRYPTO++_LIBRARY}
        scryptlib::scrypt
        argon2lib::argon2
        )

# Add subdirectories with CMakeLists.txt
set(TESTS_SUBDIR "tests")

add_subdirectory(${TESTS_SUBDIR})
