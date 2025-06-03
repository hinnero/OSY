#include "palindrome.h"
#include <algorithm>

bool isPalindrome(const std::string &str) {
    if(!str.empty() && str[0] == '-') {
        return false;
    }
    std::string rev = str;
    std::reverse(rev.begin(), rev.end());
    return str == rev;
}
