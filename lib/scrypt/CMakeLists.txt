cmake_minimum_required(VERSION 3.14)
project(libscrypt LANGUAGES C)

add_compile_definitions(NDEBUG)
set(CMAKE_CXX_FLAGS "-g2 -O3 -fPIC -pipe")
set(CMAKE_C_FLAGS "-g2 -O3 -fPIC -pipe")
add_compile_options(-g2 -O3 -fPIC -pipe)

set(SOURCES
        crypto_scrypt-nosse.c
        sha256.c
        crypto-mcf.c
        b64.c
        crypto-scrypt-saltgen.c
        crypto_scrypt-check.c
        crypto_scrypt-hash.c
        slowequals.c
        )

add_library(scrypt STATIC ${SOURCES})

target_link_libraries(scrypt PRIVATE m)


target_include_directories(scrypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(scryptlib::scrypt ALIAS scrypt)
