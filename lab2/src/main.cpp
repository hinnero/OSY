#include <iostream>
#include <vector>
#include <thread>
#include "threads.h"
#include "minmax.h"
#include "average.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    int n;
    cout << "Введите количество элементов массива: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Введите элементы массива:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    ThreadData data;
    data.arr = &arr;

    std::thread tMinMax(minMaxThreadFunc, &data);
    std::thread tAverage(averageThreadFunc, &data);

    tMinMax.join();
    tAverage.join();

    int avg = data.average;
    int minVal = data.minVal;
    int maxVal = data.maxVal;
    for (int i = 0; i < n; i++) {
        if (arr[i] == minVal || arr[i] == maxVal) {
            arr[i] = avg;
        }
    }

    cout << "Изменённый массив: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
