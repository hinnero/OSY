#include "shared_data.h"
#include "marker.h"
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

int main() {
    int arraySize = 0;
    std::cout << "Enter array size: ";
    std::cin >> arraySize;
    if (arraySize <= 0) {
        std::cerr << "Array size must be positive." << std::endl;
        return 1;
    }

    SharedData shared;
    shared.arr.resize(arraySize, 0);

    int numMarkers = 0;
    std::cout << "Enter number of marker threads: ";
    std::cin >> numMarkers;
    if (numMarkers <= 0) {
        std::cerr << "Number of marker threads must be positive." << std::endl;
        return 1;
    }

    std::vector<std::thread> markerThreads;
    std::vector<std::unique_ptr<MarkerControl>> controls;

    for (int i = 1; i <= numMarkers; ++i) {
        controls.push_back(std::make_unique<MarkerControl>());
        markerThreads.emplace_back(markerThread, i, &shared, controls.back().get());
    }

    {
        std::unique_lock<std::mutex> lock(shared.start_mutex);
        shared.start_flag = true;
        shared.cv_start.notify_all();
    }

    std::vector<int> activeMarkers(numMarkers);
    std::iota(activeMarkers.begin(), activeMarkers.end(), 0);

    while (!activeMarkers.empty()) {
        {
            std::unique_lock<std::mutex> lock(shared.wait_mutex);
            shared.cv_all_waiting.wait(lock, [&shared, &activeMarkers] {
                return shared.waiting_count >= static_cast<int>(activeMarkers.size());
            });
        }

        {
            std::unique_lock<std::mutex> lock(shared.arr_mutex);
            std::cout << "Array contents: ";
            for (const auto& value : shared.arr) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }

        int terminateMarkerId = 0;
        std::cout << "Enter marker thread id to terminate: ";
        std::cin >> terminateMarkerId;

        auto it = std::find_if(activeMarkers.begin(), activeMarkers.end(),
                               [terminateMarkerId](int markerIndex) {
                                   return (markerIndex + 1) == terminateMarkerId;
                               }
        );

        if (it == activeMarkers.end()) {
            std::cout << "Invalid marker thread id. Please try again." << std::endl;
            {
                std::unique_lock<std::mutex> lock(shared.wait_mutex);
                shared.waiting_count = 0;
            }
            for (int idx : activeMarkers) {
                std::unique_lock<std::mutex> lock(controls[idx]->m);
                controls[idx]->resume = true;
                controls[idx]->cv.notify_one();
            }
            continue;
        }

        int indexToRemove = *it;
        {
            std::unique_lock<std::mutex> lock(controls[indexToRemove]->m);
            controls[indexToRemove]->terminate = true;
            controls[indexToRemove]->cv.notify_one();
        }

        markerThreads[indexToRemove].join();
        std::cout << "Marker thread " << terminateMarkerId << " terminated." << std::endl;

        activeMarkers.erase(std::remove(activeMarkers.begin(), activeMarkers.end(), indexToRemove), activeMarkers.end());

        {
            std::unique_lock<std::mutex> lock(shared.wait_mutex);
            shared.waiting_count = 0;
        }
        for (int idx : activeMarkers) {
            std::unique_lock<std::mutex> lock(controls[idx]->m);
            controls[idx]->resume = true;
            controls[idx]->cv.notify_one();
        }
    }

    std::cout << "All marker threads have terminated. Final array:" << std::endl;
    {
        std::unique_lock<std::mutex> lock(shared.arr_mutex);
        for (const auto& value : shared.arr) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
