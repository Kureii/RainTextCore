#[[
# Name of the test target
set(TEST_TARGET_NAME "${PROJECT_NAME}_tests")

# List of test source files
set(TEST_SOURCE_FILES
        compress_test.cpp
        cypher_test.cpp
        tests.cpp
        )

# Add a test executable
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})

# Set the output directory for the test executable
set_target_properties(${TEST_TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
        )

# Attach project header files
target_include_directories(${TEST_TARGET_NAME} PRIVATE "${INCLUDE_DIR}")

# Connect Google Test Libraries
add_subdirectory(lib)
target_link_libraries(${TEST_TARGET_NAME} gtest_main gtest)

# Attach project source files as a library
target_link_libraries(${TEST_TARGET_NAME} ${PROJECT_NAME}_lib)
]]
# Název knihovny pro testování
set(TEST_LIBRARY_NAME "${PROJECT_NAME}_lib")

# Zdrojové soubory pro testovací knihovnu
set(TEST_LIBRARY_SOURCE_FILES
        ../src/main.cpp
        )

# Vytvořte testovací knihovnu
add_library(${TEST_LIBRARY_NAME} ${TEST_LIBRARY_SOURCE_FILES})

# Explicitly set the linker language for the test library target
set_target_properties(${TEST_LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Name of the test target
set(TEST_TARGET_NAME "${PROJECT_NAME}_tests")

# List of test source files
set(TEST_SOURCE_FILES
        tests.cpp
        )

# Add a test executable
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCE_FILES})

# Set the output directory for the test executable
set_target_properties(${TEST_TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
        )

find_package(SQLite3 REQUIRED)

# Attach project header files
target_include_directories(${TEST_TARGET_NAME} PRIVATE "${INCLUDE_DIR}" ${SQLite3_INCLUDE_DIRS})

# Connect Google Test Libraries
add_subdirectory(lib)

# Attach project source files as a library and SQLite3 library
target_link_libraries(${TEST_TARGET_NAME} ${TEST_LIBRARY_NAME} ${SQLite3_LIBRARIES} gtest_main gtest)

# Add target_compile_definitions for ENABLE_TESTS to both the test target and the test library
target_compile_definitions(${TEST_TARGET_NAME} PRIVATE ENABLE_TESTS)
target_compile_definitions(${TEST_LIBRARY_NAME} PRIVATE ENABLE_TESTS)

enable_testing()
add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})