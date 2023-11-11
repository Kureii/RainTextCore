cmake_minimum_required(VERSION 3.25)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
        message("Build for Android")
        set(CMAKE_SYSTEM_VERSION 28) # API level
        set(CMAKE_ANDROID_STL_TYPE c++_static)
        set(CMAKE_SYSTEM_PROCESSOR ${ANDROID_ABI})
        add_definitions(-DANDROID)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        message("Build for Windows")
        set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
        set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        message("Build for Linux")
endif()

add_compile_definitions(NDEBUG)
set(CMAKE_CXX_FLAGS "-g2 -O3 -fPIC -pipe")
set(CMAKE_C_FLAGS "-g2 -O3 -fPIC -pipe")
add_compile_options(-g2 -O3 -fPIC -pipe)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project name
project(
        "RainTextCore"
        VERSION 0.5.2
        LANGUAGES CXX
)

include(include/include.cmake)
include(src/src.cmake)

add_subdirectory(lib)

include(cmake/CPM.cmake)
include(cmake/FasterBuild.cmake)

include_directories(./include)

add_library(${PROJECT_NAME} STATIC
        ${rain-text-core-headers}
        ${rain-text-core-sources}
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        cryptopp::cryptopp
        scryptlib::scrypt
        argon2lib::argon2
        )
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_subdirectory(tests)
else ()
        message("tests are not available")
endif ()