#include "singleton_orthogonal_strategies.hpp"
#include <iostream>
#include <string>

class MyLogger
{
public:
    void Log(const std::string& msg)
    {
        std::cout << "[LOG] " << msg << std::endl;
    }
};

using LoggerST = Singleton<
    MyLogger,
    CreateUsingNew,
    LifetimeNoDestroy,
    SingleThreaded
>;

using LoggerMT = Singleton<
    MyLogger,
    CreateUsingNew,
    LifetimeAtExit,
    MultiThreaded
>;

int main()
{
    LoggerST::Instance().Log("Запуск в однопоточном режиме");

    LoggerMT::Instance().Log("Запуск в многопоточном режиме");

    return 0;
}
