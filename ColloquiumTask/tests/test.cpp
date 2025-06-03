#include <gtest/gtest.h>
#include "../src/fibonacci.h"
#include "../src/palindrome.h"
#include "../src/reverse_list.h"
#include "../src/list_node.h"
#include <vector>

TEST(FibonacciTest, GeneratesCorrectSequence) {
    auto fib = generateFibonacci(7);
    std::vector<long long> expected = {0, 1, 1, 2, 3, 5, 8};
    EXPECT_EQ(fib, expected);
}

TEST(FibonacciTest, ThrowsOnZero) {
    EXPECT_THROW(generateFibonacci(0), std::invalid_argument);
}

TEST(PalindromeTest, PalindromeNumber) {
    EXPECT_TRUE(isPalindrome("121"));
    EXPECT_TRUE(isPalindrome("1221"));
}

TEST(PalindromeTest, NotPalindromeNumber) {
    EXPECT_FALSE(isPalindrome("123"));
    EXPECT_FALSE(isPalindrome("-121"));
}

TEST(ReverseListTest, ReverseCorrectly) {
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);

    ListNode* reversed = reverseList(head);

    std::vector<int> result;
    while(reversed) {
        result.push_back(reversed->data);
        ListNode* temp = reversed;
        reversed = reversed->next;
        delete temp;
    }

    std::vector<int> expected = {4, 3, 2, 1};
    EXPECT_EQ(result, expected);
}
