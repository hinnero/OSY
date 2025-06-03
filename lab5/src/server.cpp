#include "employee.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>

const char* SERVER_FIFO = "/tmp/server_fifo";

void create_employee_file(const std::string& filename, const std::vector<employee>& employees) {
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
    for (const auto& emp : employees) {
        ofs.write(reinterpret_cast<const char*>(&emp), sizeof(emp));
    }
}

void print_file(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    employee emp;
    while (ifs.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        std::cout << "ID: " << emp.num
                  << ", Name: " << emp.name
                  << ", Hours: " << emp.hours << std::endl;
    }
}

bool lock_record(int fd, long idx, bool write_lock) {
    struct flock fl{};
    fl.l_type   = write_lock ? F_WRLCK : F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = idx * sizeof(employee);
    fl.l_len    = sizeof(employee);
    return fcntl(fd, F_SETLKW, &fl) != -1;
}

bool unlock_record(int fd, long idx) {
    struct flock fl{};
    fl.l_type   = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = idx * sizeof(employee);
    fl.l_len    = sizeof(employee);
    return fcntl(fd, F_SETLKW, &fl) != -1;
}

int main() {
    std::string filename;
    int client_count;

    std::cout << "Enter filename: ";
    std::cin >> filename;
    std::cout << "Enter number of employees: ";
    int n; std::cin >> n;

    std::vector<employee> employees(n);
    for (int i = 0; i < n; ++i) {
        std::cout << "Employee " << i+1 << " ID: ";   std::cin >> employees[i].num;
        std::cout << "Employee " << i+1 << " Name: "; std::cin >> employees[i].name;
        std::cout << "Employee " << i+1 << " Hours: ";std::cin >> employees[i].hours;
    }

    create_employee_file(filename, employees);
    std::cout << "Initial file:" << std::endl;
    print_file(filename);

    mkfifo(SERVER_FIFO, 0666);

    std::cout << "Enter number of clients: ";
    std::cin >> client_count;

    for (int i = 0; i < client_count; ++i) {
        if (fork() == 0) {
            execl("./client", "client", SERVER_FIFO, filename.c_str(), nullptr);
            perror("execl");
            return 1;
        }
    }

    int server_rd = open(SERVER_FIFO, O_RDONLY);
    int server_wr = open(SERVER_FIFO, O_WRONLY);
    if (server_rd < 0 || server_wr < 0) {
        perror("open server fifo");
        return 1;
    }

    int remaining = client_count;
    while (remaining > 0) {
        struct {
            pid_t pid;
            employee emp;
            char op;
        } msg;

        ssize_t r = read(server_rd, &msg, sizeof(msg));
        if (r != sizeof(msg)) {
            if (errno == EINTR) continue;
            perror("read");
            break;
        }

        if (msg.op == 'q') {
            --remaining;
            continue;
        }

        std::string client_fifo = "/tmp/fifo_" + std::to_string(msg.pid);
        int data_fd = open(filename.c_str(), O_RDWR);
        if (data_fd < 0) { perror("open data file"); continue; }

        off_t idx = -1;
        for (off_t j = 0; ; ++j) {
            employee e;
            if (pread(data_fd, &e, sizeof(e), j * sizeof(e)) != sizeof(e)) break;
            if (e.num == msg.emp.num) { idx = j; break; }
        }

        int cfd = open(client_fifo.c_str(), O_WRONLY);
        if (idx < 0) {
            employee nf{-1, "", 0.0};
            write(cfd, &nf, sizeof(nf));
            close(cfd);
            close(data_fd);
            continue;
        }

        if (msg.op == 'r') {
            lock_record(data_fd, idx, false);
            employee e;
            pread(data_fd, &e, sizeof(e), idx * sizeof(e));
            unlock_record(data_fd, idx);
            write(cfd, &e, sizeof(e));
            close(cfd);

        } else if (msg.op == 'w') {
            lock_record(data_fd, idx, true);
            employee cur;
            pread(data_fd, &cur, sizeof(cur), idx * sizeof(cur));
            write(cfd, &cur, sizeof(cur));
            close(cfd);

            employee mod;
            read(server_rd, &mod, sizeof(mod));
            pwrite(data_fd, &mod, sizeof(mod), idx * sizeof(mod));
            unlock_record(data_fd, idx);
        }

        close(data_fd);
    }

    close(server_rd);
    close(server_wr);
    unlink(SERVER_FIFO);

    while (wait(nullptr) > 0) {}

    std::cout << "Final file:" << std::endl;
    print_file(filename);
    return 0;
}
