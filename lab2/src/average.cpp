#include "average.h"
#include <iostream>
#include <chrono>
#include <thread>

void computeAverage(const std::vector<int>& arr, int& average) {
    if (arr.empty()) return;
    int sum = 0;
    for (size_t i = 0; i < arr.size(); i++) {
        sum += arr[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
    average = sum / static_cast<int>(arr.size());
}

void averageThreadFunc(ThreadData* data) {
    if (!data || !data->arr) return;
    computeAverage(*(data->arr), data->average);
    std::cout << "Поток average: Среднее значение = " << data->average << std::endl;
}