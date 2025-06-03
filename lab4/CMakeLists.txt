cmake_minimum_required(VERSION 3.11)
project(lab4 CXX)

set(CMAKE_CXX_STANDARD 17)
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

add_executable(receiver
        src/Receiver.cpp
        src/MessageQueue.cpp
)

add_executable(sender
        src/Sender.cpp
        src/MessageQueue.cpp
)

add_executable(runTests
        tests/message_queue_test.cpp
        src/MessageQueue.cpp
)

target_link_libraries(runTests gtest gtest_main)

add_test(NAME RunAllTests COMMAND runTests)
