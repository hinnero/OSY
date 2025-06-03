#ifndef THREADS_H
#define THREADS_H

#include <vector>

struct ThreadData {
    const std::vector<int>* arr;
    int minVal;
    int maxVal;
    int average;
};

#endif // THREADS_H