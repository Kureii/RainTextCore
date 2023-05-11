cmake_minimum_required(VERSION 3.14)
project(libscrypt LANGUAGES C)

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

target_compile_options(scrypt PRIVATE -D_FORTIFY_SOURCE=2 -fPIC -O2 -Wall -g -fstack-protector)
target_link_libraries(scrypt PRIVATE m)


target_include_directories(scrypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Oprava: Přidání aliasu pro cíl "sha3"
add_library(scryptlib::scrypt ALIAS scrypt)
