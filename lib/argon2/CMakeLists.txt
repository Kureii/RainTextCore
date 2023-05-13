cmake_minimum_required(VERSION 3.10)
project(argon2)

set(CMAKE_C_STANDARD 99)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(include)
include_directories(src)

set(SOURCE_FILES
        src/argon2.c
        src/core.c
        src/blake2/blake2b.c
        src/thread.c
        src/encoding.c
        src/ref.c
        src/opt.c
        )

add_library(argon2 STATIC ${SOURCE_FILES})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

target_include_directories(argon2
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )


if(WIN32)
    target_link_libraries(argon2 PRIVATE bcrypt)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(argon2 PRIVATE Threads::Threads)
endif()

add_library(argon2lib::argon2 ALIAS argon2)
