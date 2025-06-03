cmake_minimum_required(VERSION 3.11)
project(lab5_os_named_pipes CXX)

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

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)

target_link_libraries(server pthread)
target_link_libraries(client pthread)

target_include_directories(server PRIVATE src)
target_include_directories(client PRIVATE src)
add_executable(test_employee
        tests/test_employee.cpp
        src/employee.h
)
target_link_libraries(test_employee gtest gtest_main pthread)
add_test(NAME EmployeeTests COMMAND test_employee)