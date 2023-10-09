add_subdirectory(scrypt)
add_subdirectory(argon2)

target_include_directories(scrypt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/scrypt ${CMAKE_BINARY_DIR}/lib/scrypt)
target_include_directories(Argon2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/argon2/src/Argon2)



