#ifndef AVERAGE_H
#define AVERAGE_H

#include <vector>
#include "threads.h"

void computeAverage(const std::vector<int>& arr, int& average);

void averageThreadFunc(ThreadData* data);

#endif // AVERAGE_H