//
// Created by Райцев Олег on 29.05.25.
//
#include "filesystem_nodes.hpp"
#include <thread>
#include <cassert>
#include <iostream>

class CountingVisitor : public Visitor {
public:
    void visit(Disk& d)      override { ++diskCount; }
    void visit(Folder& f)    override { ++folderCount; }
    void visit(File& f)      override { ++fileCount; }
    void visit(ImageFile& f) override { ++imageCount; }
    void visit(TextFile& f)  override { ++textCount; }

    int diskCount   = 0;
    int folderCount = 0;
    int fileCount   = 0;
    int imageCount  = 0;
    int textCount   = 0;
};

void unit_test_single_thread() {
    using Sync = SingleThreadedSync;
    Disk<Sync> disk("D:");
    auto folder = std::make_shared<Folder<Sync>>("Docs");
    folder->add(std::make_shared<TextFile<Sync>>("readme.txt"));
    disk.add(folder);

    CountingVisitor v;
    disk.accept(v);
    assert(v.diskCount   == 1);
    assert(v.folderCount == 1);
    assert(v.fileCount   == 1);
    assert(v.textCount   == 1);
    std::cout << "Unit test (single-thread) passed.\n";
}

void e2e_test_multi_thread() {
    using Sync = MultiThreadedSync;
    Disk<Sync> disk("E:");
    auto folder = std::make_shared<Folder<Sync>>("Pics");
    folder->add(std::make_shared<ImageFile<Sync>>("photo.jpg"));
    disk.add(folder);

    CountingVisitor v;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() { disk.accept(v); });
    }
    for (auto& t : threads) t.join();

    assert(v.diskCount   == 10);
    assert(v.folderCount == 10);
    assert(v.fileCount   == 10);
    assert(v.imageCount  == 10);
    std::cout << "E2E test (multi-thread) passed.\n";
}

int main() {
    unit_test_single_thread();
    e2e_test_multi_thread();
    std::cout << "All tests passed.\n";
    return 0;
}
