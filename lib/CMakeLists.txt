add_subdirectory(scrypt)
add_subdirectory(argon2)
add_subdirectory(cryptopp-cmake-CRYPTOPP_8_9_0)

target_include_directories(scrypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/scrypt ${CMAKE_BINARY_DIR}/lib/scrypt)
target_include_directories(Argon2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/argon2/src/Argon2 ${CMAKE_BINARY_DIR}/lib/Argon2)
