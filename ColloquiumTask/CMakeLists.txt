cmake_minimum_required(VERSION 3.11)
project(MyProject CXX)

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

add_executable(main_app
        src/main.cpp
        src/fibonacci.cpp
        src/palindrome.cpp
        src/reverse_list.cpp
)

add_executable(runTests
        tests/test.cpp
        src/fibonacci.cpp
        src/palindrome.cpp
        src/reverse_list.cpp
)

target_link_libraries(runTests gtest gtest_main pthread)

enable_testing()
add_test(NAME runTests COMMAND runTests)
