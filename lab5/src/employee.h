#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <cstring>

struct employee {
    int num;
    char name[10];
    double hours;
};

inline bool operator==(const employee& a, const employee& b) {
    return a.num == b.num
           && std::strncmp(a.name, b.name, sizeof(a.name)) == 0
           && a.hours == b.hours;
}

#endif // EMPLOYEE_H