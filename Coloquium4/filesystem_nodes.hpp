//
// Created by Райцев Олег on 29.05.25.
//
#ifndef FILESYSTEM_NODES_HPP
#define FILESYSTEM_NODES_HPP

#include "visitor.hpp"
#include "sync_policy.hpp"
#include <vector>
#include <string>
#include <memory>

template <typename SyncPolicy>
class Node {
public:
    explicit Node(std::string name) : name_(std::move(name)) {}
    virtual ~Node() = default;
    virtual void accept(Visitor& v) = 0;
    const std::string& name() const { return name_; }

protected:
    std::string name_;
    std::mutex& mutex_ = SyncPolicy::getMutex();
};

template <typename SyncPolicy>
class File : public Node<SyncPolicy> {
public:
    using Node<SyncPolicy>::Node;
    void accept(Visitor& v) override {
        SyncPolicy::Lock lock(this->mutex_);
        v.visit(*this);
    }
};

template <typename SyncPolicy>
class ImageFile : public File<SyncPolicy> {
public:
    using File<SyncPolicy>::File;
    void accept(Visitor& v) override {
        File<SyncPolicy>::accept(v);
        v.visit(*this);
    }
};

template <typename SyncPolicy>
class TextFile : public File<SyncPolicy> {
public:
    using File<SyncPolicy>::File;
    void accept(Visitor& v) override {
        File<SyncPolicy>::accept(v);
        v.visit(*this);
    }
};

template <typename SyncPolicy>
class Folder : public Node<SyncPolicy> {
public:
    using Node<SyncPolicy>::Node;

    void add(std::shared_ptr<Node<SyncPolicy>> node) {
        SyncPolicy::Lock lock(this->mutex_);
        children_.push_back(std::move(node));
    }

    void accept(Visitor& v) override {
        SyncPolicy::Lock lock(this->mutex_);
        v.visit(*this);
        for (auto& c : children_) {
            c->accept(v);
        }
    }

private:
    std::vector<std::shared_ptr<Node<SyncPolicy>>> children_;
};

template <typename SyncPolicy>
class Disk : public Folder<SyncPolicy> {
public:
    using Folder<SyncPolicy>::Folder;

    void accept(Visitor& v) override {
        SyncPolicy::Lock lock(this->mutex_);
        v.visit(*this);
        Folder<SyncPolicy>::accept(v);
    }
};

#endif // FILESYSTEM_NODES_HPP
