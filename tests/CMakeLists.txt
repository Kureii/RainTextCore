# Name of the test target
set(TEST_TARGET_NAME "${PROJECT_NAME}_tests")

# List of test source files
set(TEST_SOURCE_FILES
        tests.cpp
        test_rain_text_core_utils.cpp
        test_cipher.cpp
)

# Add a test executable
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})

# Set the output directory for the test executable
set_target_properties(${TEST_TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

# Add dependencies if needed
add_dependencies(${TEST_TARGET_NAME} scrypt Argon2)

# Include directories
target_include_directories(${TEST_TARGET_NAME} PRIVATE "${INCLUDE_DIR}" ${SQLite3_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/lib/argon2/include" "${CMAKE_SOURCE_DIR}/lib/scrypt")

# Find and include GTest
find_package(GTest REQUIRED)

# Link the test target with main library and other dependencies
target_link_libraries(${TEST_TARGET_NAME}
        RainTextCore
        cryptopp
        gtest_main
        gtest
        scryptlib::scrypt
        argon2lib::argon2
)

# Add target_compile_definitions for ENABLE_TESTS
target_compile_definitions(${TEST_TARGET_NAME} PRIVATE ENABLE_TESTS)

# Enable testing and add test
enable_testing()
add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})
