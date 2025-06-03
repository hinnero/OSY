#include "fibonacci.h"
#include <stdexcept>

std::vector<long long> generateFibonacci(unsigned int n) {
    if(n == 0) {
        throw std::invalid_argument("n должно быть натуральным числом (n > 0)");
    }
    std::vector<long long> fib;
    fib.reserve(n);
    fib.push_back(0);
    if(n == 1) return fib;
    fib.push_back(1);
    for(unsigned int i = 2; i < n; ++i) {
        fib.push_back(fib[i - 1] + fib[i - 2]);
    }
    return fib;
}
