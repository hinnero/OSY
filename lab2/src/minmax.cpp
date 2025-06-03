#include "minmax.h"
#include <iostream>
#include <chrono>
#include <thread>

void computeMinMax(const std::vector<int>& arr, int& minVal, int& maxVal) {
    if (arr.empty()) return;
    minVal = arr[0];
    maxVal = arr[0];

    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < minVal)
            minVal = arr[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        if (arr[i] > maxVal)
            maxVal = arr[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }
}

void minMaxThreadFunc(ThreadData* data) {
    if (!data || !data->arr) return;
    computeMinMax(*(data->arr), data->minVal, data->maxVal);
    std::cout << "Поток min_max: Минимальный элемент = " << data->minVal << std::endl;
    std::cout << "Поток min_max: Максимальный элемент = " << data->maxVal << std::endl;
}