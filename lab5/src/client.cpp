#include "employee.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc < 3) return 1;
    const char* server_fifo = argv[1];
    std::string filename = argv[2];

    pid_t pid = getpid();
    std::string client_fifo = "/tmp/fifo_" + std::to_string(pid);
    mkfifo(client_fifo.c_str(), 0666);

    int srv_fd = open(server_fifo, O_WRONLY);
    if (srv_fd < 0) { perror("open server"); return 1; }

    while (true) {
        std::cout << "Operation (r=read, w=write, q=quit): ";
        char op; std::cin >> op;

        struct {
            pid_t pid;
            employee emp;
            char op;
        } msg{ pid, {}, op };

        if (op == 'q') {
            write(srv_fd, &msg, sizeof(msg));
            break;
        }

        std::cout << "Enter employee ID: ";
        std::cin >> msg.emp.num;
        write(srv_fd, &msg, sizeof(msg));

        int cfd = open(client_fifo.c_str(), O_RDONLY);
        employee res;
        read(cfd, &res, sizeof(res));
        close(cfd);

        if (res.num < 0) {
            std::cout << "Employee with ID " << msg.emp.num << " not found." << std::endl;
            continue;
        }

        std::cout << "Received: ID=" << res.num
                  << " Name=" << res.name
                  << " Hours=" << res.hours << std::endl;

        if (op == 'w') {
            std::cout << "Enter new hours: ";
            std::cin >> res.hours;
            write(srv_fd, &res, sizeof(res));
        }
    }

    close(srv_fd);
    unlink(client_fifo.c_str());
    return 0;
}
