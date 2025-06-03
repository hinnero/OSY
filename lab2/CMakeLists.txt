cmake_minimum_required(VERSION 3.11)
project(lab2 CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(lab2
        src/main.cpp
        src/minmax.cpp
        src/average.cpp
)

add_executable(runTests
        tests/average_test.cpp
        tests/minmax_test.cpp
        src/minmax.cpp
        src/average.cpp
)

target_link_libraries(runTests gtest gtest_main)

add_test(NAME RunAllTests COMMAND runTests)
