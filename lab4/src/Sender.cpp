#include "MessageQueue.h"
#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Sender: Требуется указать имя файла и PID Receiver." << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    int receiverPid = std::stoi(argv[2]);
    int capacity = 10;
    MessageQueue mq(filename, capacity);
    if (kill(receiverPid, SIGUSR1) != 0) {
        std::cerr << "Sender: Не удалось отправить сигнал готовности." << std::endl;
    } else {
        std::cout << "Sender: Сигнал готовности отправлен Receiver (PID " << receiverPid << ")." << std::endl;
    }
    std::cout << "Sender: Файл \"" << filename << "\" открыт для отправки сообщений." << std::endl;
    std::cout << "Sender: Для выхода введите команду exit." << std::endl;
    std::string input;
    std::cin.ignore();
    while (true) {
        std::cout << "Sender: Введите сообщение (не более 20 символов) или exit для завершения: ";
        std::getline(std::cin, input);
        if (input == "exit") {
            std::cout << "Sender: Завершение работы." << std::endl;
            break;
        }
        if (input.length() > MAX_MESSAGE_LENGTH) {
            input = input.substr(0, MAX_MESSAGE_LENGTH);
            std::cout << "Sender: Сообщение обрезано до 20 символов." << std::endl;
        }
        if (!mq.sendMessage(input)) {
            std::cout << "Sender: Очередь заполнена. Попробуйте позже." << std::endl;
        } else {
            std::cout << "Sender: Сообщение отправлено." << std::endl;
        }
    }
    return 0;
}
