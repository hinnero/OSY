#include "minmax.h"
#include "threads.h"
#include <gtest/gtest.h>
#include <vector>

TEST(ComputeMinMaxTest, HandlesPositiveNumbers) {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int minVal = 0;
    int maxVal = 0;
    computeMinMax(arr, minVal, maxVal);
    EXPECT_EQ(minVal, 1);
    EXPECT_EQ(maxVal, 5);
}

TEST(ComputeMinMaxTest, HandlesNegativeNumbers) {
    std::vector<int> arr = { -1, -2, -3, -4, -5 };
    int minVal = 0;
    int maxVal = 0;
    computeMinMax(arr, minVal, maxVal);
    EXPECT_EQ(minVal, -5);
    EXPECT_EQ(maxVal, -1);
}

TEST(ComputeMinMaxTest, HandlesMixedNumbers) {
    std::vector<int> arr = { -1, 0, 1 };
    int minVal = 0;
    int maxVal = 0;
    computeMinMax(arr, minVal, maxVal);
    EXPECT_EQ(minVal, -1);
    EXPECT_EQ(maxVal, 1);
}

TEST(ComputeMinMaxTest, HandlesEmptyArray) {
    std::vector<int> arr = {};
    int minVal = 0;
    int maxVal = 0;
    computeMinMax(arr, minVal, maxVal);
    EXPECT_EQ(minVal, 0);
    EXPECT_EQ(maxVal, 0);
}
