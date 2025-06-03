// Receiver.cpp
#include "MessageQueue.h"
#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>

void handleSigUsr1(int sig) {
    std::cout << "\nReceiver: Получен сигнал SIGUSR1" << std::endl;
}

int main() {
    std::signal(SIGUSR1, handleSigUsr1);

    std::cout << "Receiver PID: " << getpid() << std::endl;
    std::string filename;
    int capacity;
    std::cout << "Receiver: Введите имя бинарного файла для сообщений: ";
    std::cin >> filename;
    std::cout << "Receiver: Введите количество записей в файле: ";
    std::cin >> capacity;
    MessageQueue mq(filename, capacity);
    std::cout << "Receiver: Файл \"" << filename << "\" создан с " << capacity << " записями." << std::endl;
    std::cout << "Receiver: Запустите Sender процессы вручную и укажите PID этого Receiver." << std::endl;
    std::cout << "Receiver: Для чтения сообщений введите команду read, для завершения - exit." << std::endl;
    std::string command;
    while (true) {
        std::cout << "Receiver: Введите команду (read - чтение сообщения, exit - завершение): ";
        std::cin >> command;
        if (command == "read") {
            std::string msg = mq.receiveMessage();
            if (msg.empty()) {
                std::cout << "Receiver: Сообщений нет. Ожидание нового сообщения..." << std::endl;
            } else {
                std::cout << "Receiver: Прочитано сообщение: " << msg << std::endl;
            }
        } else if (command == "exit") {
            std::cout << "Receiver: Завершение работы." << std::endl;
            break;
        } else {
            std::cout << "Receiver: Неизвестная команда." << std::endl;
        }
    }
    return 0;
}
