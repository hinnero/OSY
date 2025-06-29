#include "filesystem_nodes.hpp"
#include <iostream>
#include <thread>

struct DemoVisitor : Visitor {
    void visit(Disk& d) override {
        std::cout << "Disk:       " << d.name() << "\n";
    }
    void visit(Folder& f) override {
        std::cout << "Folder:     " << f.name() << "\n";
    }
    void visit(File& f) override {
        std::cout << "File:       " << f.name() << "\n";
    }
    void visit(ImageFile& f) override {
        std::cout << "ImageFile:  " << f.name() << "\n";
    }
    void visit(TextFile& f) override {
        std::cout << "TextFile:   " << f.name() << "\n";
    }
};

int main() {
    using Sync = SingleThreadedSync;
    Disk<Sync> disk("C:");
    auto docs = std::make_shared<Folder<Sync>>("Docs");
    docs->add(std::make_shared<TextFile<Sync>>("todo.txt"));
    docs->add(std::make_shared<ImageFile<Sync>>("diagram.png"));
    disk.add(docs);

    std::cout << "=== Single-threaded traversal ===\n";
    DemoVisitor visitor1;
    disk.accept(visitor1);

    // Теперь демонстрация в многопоточном режиме
    using SyncMT = MultiThreadedSync;
    Disk<SyncMT> diskMT("D:");
    auto pics = std::make_shared<Folder<SyncMT>>("Pics");
    pics->add(std::make_shared<ImageFile<SyncMT>>("photo1.jpg"));
    pics->add(std::make_shared<ImageFile<SyncMT>>("photo2.jpg"));
    diskMT.add(pics);

    std::cout << "\n=== Multi-threaded traversal (3 threads) ===\n";
    DemoVisitor visitor2;
    std::thread t1([&]{ diskMT.accept(visitor2); });
    std::thread t2([&]{ diskMT.accept(visitor2); });
    std::thread t3([&]{ diskMT.accept(visitor2); });

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
