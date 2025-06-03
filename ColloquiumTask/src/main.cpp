#include <iostream>
#include <vector>
#include <string>
#include "fibonacci.h"
#include "palindrome.h"
#include "list_node.h"
#include "reverse_list.h"

void handleFibonacci() {
    std::cout << "Введите натуральное число n для генерации последовательности Фибоначчи: ";
    unsigned int n;
    std::cin >> n;
    try {
        auto fib = generateFibonacci(n);
        std::cout << "Первые " << n << " чисел Фибоначчи: ";
        for(auto num : fib) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    } catch(const std::exception& ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
    }
}

void handlePalindrome() {
    std::cout << "Введите число для проверки на палиндром: ";
    std::string input;
    std::cin >> input;
    if(isPalindrome(input))
        std::cout << input << " является палиндромом." << std::endl;
    else
        std::cout << input << " не является палиндромом." << std::endl;
}

void printList(ListNode* head) {
    while(head) {
        std::cout << head->data;
        if(head->next)
            std::cout << " -> ";
        head = head->next;
    }
    std::cout << std::endl;
}

void handleReverseList() {
    std::cout << "Введите элементы связного списка (через пробел, закончите ввод словом 'end'): ";
    std::vector<int> elements;
    int num;
    while(std::cin >> num) {
        elements.push_back(num);
    }
    std::cin.clear();
    std::string dummy;
    std::cin >> dummy;

    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for(auto val : elements) {
        ListNode* newNode = new ListNode(val);
        if(!head) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    std::cout << "Исходный список: ";
    printList(head);

    ListNode* reversed = reverseList(head);
    std::cout << "Развернутый список: ";
    printList(reversed);

    while(reversed) {
        ListNode* temp = reversed;
        reversed = reversed->next;
        delete temp;
    }
}

int main() {
    while(true) {
        std::cout << "\nВыберите функцию:\n";
        std::cout << "1. Генерация чисел Фибоначчи\n";
        std::cout << "2. Проверка на палиндром\n";
        std::cout << "3. Разворот связного списка\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";
        int choice;
        std::cin >> choice;
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cerr << "Неверный ввод. Повторите попытку.\n";
            continue;
        }
        switch(choice) {
            case 1:
                handleFibonacci();
                break;
            case 2:
                handlePalindrome();
                break;
            case 3:
                handleReverseList();
                break;
            case 0:
                std::cout << "Выход.\n";
                return 0;
            default:
                std::cerr << "Неверный выбор. Попробуйте снова.\n";
                break;
        }
    }
    return 0;
}
