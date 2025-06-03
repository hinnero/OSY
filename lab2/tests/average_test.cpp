#include "average.h"
#include "threads.h"
#include <gtest/gtest.h>
#include <vector>

TEST(ComputeAverageTest, HandlesPositiveNumbers) {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int average = 0;
    computeAverage(arr, average);
    EXPECT_EQ(average, 3);
}

TEST(ComputeAverageTest, HandlesNegativeNumbers) {
    std::vector<int> arr = { -1, -2, -3, -4, -5 };
    int average = 0;
    computeAverage(arr, average);
    EXPECT_EQ(average, -3);
}

TEST(ComputeAverageTest, HandlesMixedNumbers) {
    std::vector<int> arr = { -1, 0, 1 };
    int average = 0;
    computeAverage(arr, average);
    EXPECT_EQ(average, 0);
}

TEST(ComputeAverageTest, HandlesEmptyArray) {
    std::vector<int> arr = {};
    int average = 0;
    computeAverage(arr, average);
    EXPECT_EQ(average, 0);
}
