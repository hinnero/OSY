//
// Created by Райцев Олег on 29.05.25.
//

#ifndef SYNC_POLICY_HPP
#define SYNC_POLICY_HPP

#include <mutex>

struct SingleThreadedSync {
    struct Lock { explicit Lock(std::mutex&) {} };
    static std::mutex& getMutex() {
        static std::mutex dummy;
        return dummy;
    }
};

struct MultiThreadedSync {
    struct Lock {
        explicit Lock(std::mutex& m) : mtx(m) { mtx.lock(); }
        ~Lock() { mtx.unlock(); }
    private:
        std::mutex& mtx;
    };
    static std::mutex& getMutex() {
        static std::mutex m;
        return m;
    }
};

#endif // SYNC_POLICY_HPP
