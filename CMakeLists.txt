cmake_minimum_required(VERSION 3.25)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
        message("Build for Android")
        set(CMAKE_SYSTEM_VERSION 28) # API level
        set(CMAKE_ANDROID_STL_TYPE c++_static)
        #set(CRYPTOPP_LIB_PATH ${CMAKE_SOURCE_DIR}/lib/cryptopp/output/android/${ANDROID_ABI}/libcryptopp.a)
        set(CMAKE_SYSTEM_PROCESSOR ${ANDROID_ABI})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        message("Build for Windows")
        set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
        set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
        # set(CRYPTOPP_LIB_PATH ${CMAKE_SOURCE_DIR}/lib/cryptopp/output/windows/libcryptopp.a)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        message("Build for Linux")
        # set(CRYPTOPP_LIB_PATH ${CMAKE_SOURCE_DIR}/lib/cryptopp/output/linux/libcryptopp.a)
        set(CMAKE_SYSTEM_PROCESSOR "x86_64")
endif()

add_compile_definitions(NDEBUG)
set(CMAKE_CXX_FLAGS "-g2 -O3 -fPIC -pipe")
set(CMAKE_C_FLAGS "-g2 -O3 -fPIC -pipe")
add_compile_options(-g2 -O3 -fPIC -pipe)


# Set C++23 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project name
project(
        "RainTextCore"
        VERSION 0.5.0
        LANGUAGES CXX
)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
        if(ANDROID_ABI STREQUAL "armeabi-v7a")
                set(CMAKE_SYSTEM_PROCESSOR "armeabi-v7a")
        elseif(ANDROID_ABI STREQUAL "arm64-v8a")
                set(CMAKE_SYSTEM_PROCESSOR "arm64-v8a")
        elseif(ANDROID_ABI STREQUAL "x86")
                set(CMAKE_SYSTEM_PROCESSOR "x86")
        elseif(ANDROID_ABI STREQUAL "x86_64")
                set(CMAKE_SYSTEM_PROCESSOR "x86_64")
        endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(CMAKE_SYSTEM_PROCESSOR "x86_64")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
                set(CMAKE_SYSTEM_PROCESSOR "x86_64")
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
                set(CMAKE_SYSTEM_PROCESSOR "aarch64")
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
                set(CMAKE_SYSTEM_PROCESSOR "arm")
        endif()
endif()
message("CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})


include(include/include.cmake)
include(src/src.cmake)

add_subdirectory(lib)

# Linkování knihovny

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