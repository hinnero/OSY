//
// Created by Райцев Олег on 29.05.25.
//
#ifndef SINGLETON_ORTHOGONAL_STRATEGIES_HPP
#define SINGLETON_ORTHOGONAL_STRATEGIES_HPP

#include <mutex>
#include <atomic>

struct SingleThreaded
{
    struct Lock
    {
        explicit Lock(std::mutex&) {}
    };
    static std::mutex& GetMutex()
    {
        static std::mutex dummy;
        return dummy;
    }
};

struct MultiThreaded
{
    struct Lock
    {
        explicit Lock(std::mutex& m) : mtx(m) { mtx.lock(); }
        ~Lock()                { mtx.unlock(); }
    private:
        std::mutex& mtx;
    };

    static std::mutex& GetMutex()
    {
        static std::mutex m;
        return m;
    }
};

template <typename T>
struct CreateUsingNew
{
    static T* Create()    { return new T; }
    static void Destroy(T* p) { delete p; }
};

template <typename T>
struct CreateUsingStatic
{
    static T* Create()
    {
        static T instance;
        return &instance;
    }
    static void Destroy(T*) {}
};

template <typename T>
struct LifetimeNoDestroy
{
    static void ScheduleDestruction(T*, void (*)()) {}
};

template <typename T>
struct LifetimeAtExit
{
    static void ScheduleDestruction(T* /*p*/, void (*dtor)())
    {
        std::atexit(dtor);
    }
};

template <
    typename T,
    template<class> class CreationPolicy   = CreateUsingNew,
    template<class> class LifetimePolicy   = LifetimeAtExit,
    typename ThreadingModel                = SingleThreaded
>
class Singleton
{
public:
    static T& Instance()
    {
        if (!pInstance_.load(std::memory_order_acquire)) {
            typename ThreadingModel::Lock lock(GetMutex());
            if (!pInstance_.load(std::memory_order_relaxed)) {
                T* temp = CreationPolicy<T>::Create();
                try {
                    LifetimePolicy<T>::ScheduleDestruction(temp, &Destroy);
                } catch (...) {
                    CreationPolicy<T>::Destroy(temp);
                    throw;
                }
                pInstance_.store(temp, std::memory_order_release);
            }
        }
        return *pInstance_.load(std::memory_order_acquire);
    }

private:
    static void Destroy()
    {
        CreationPolicy<T>::Destroy(pInstance_.load());
        pInstance_.store(nullptr);
    }

    static std::mutex& GetMutex()
    {
        return ThreadingModel::GetMutex();
    }

    static inline std::atomic<T*> pInstance_{nullptr};
};

#endif // SINGLETON_ORTHOGONAL_STRATEGIES_HPP