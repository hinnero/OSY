#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include "../src/employee.h"

TEST(EmployeeStruct, EqualityOperator) {
    employee a{1, "Alice", 40.0};
    employee b{1, "Alice", 40.0};
    EXPECT_TRUE(a == b);
    employee c{2, "Bob", 35.5};
    EXPECT_FALSE(a == c);
}

TEST(FileIO, CreateAndReadBack) {
    std::string fname = "temp_test.dat";
    std::vector<employee> vec = {{1, "A", 1.0}, {2, "B", 2.0}};
    {
    std::ofstream ofs(fname, std::ios::binary);
    for (auto& e : vec) ofs.write(reinterpret_cast<char*>(&e), sizeof(e));
    }
    std::ifstream ifs(fname, std::ios::binary);
    employee e1, e2;
    ifs.read(reinterpret_cast<char*>(&e1), sizeof(e1));
    ifs.read(reinterpret_cast<char*>(&e2), sizeof(e2));
    EXPECT_EQ(e1, vec[0]);
    EXPECT_EQ(e2, vec[1]);
}

TEST(EmployeeStruct, NameDifference) {
    employee a{1, "Alice", 10.0};
    employee b{1, "Alicf", 10.0};
    EXPECT_FALSE(a == b);
}

TEST(EmployeeStruct, HoursDifference) {
    employee a{1, "Alice", 10.0};
    employee b{1, "Alice", 12.5};
    EXPECT_FALSE(a == b);
}

TEST(EmployeeStruct, ZeroHours) {
    employee a{5, "Zero", 0.0};
    employee b{5, "Zero", 0.0};
    EXPECT_TRUE(a == b);
}

TEST(EmployeeStruct, LongNameTruncation) {
    employee a{6, "LongNameX", 20.0};
    employee b{6, "LongNameY", 20.0};
    EXPECT_FALSE(a == b);
}