#ifndef MINMAX_H
#define MINMAX_H

#include <vector>
#include "threads.h"

void computeMinMax(const std::vector<int>& arr, int& minVal, int& maxVal);

void minMaxThreadFunc(ThreadData* data);

#endif // MINMAX_H