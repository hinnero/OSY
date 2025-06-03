#include "MessageQueue.h"
#include <iostream>

static void writeFixed(std::fstream &fs, const char* data, size_t size, std::streampos pos) {
    fs.seekp(pos);
    fs.write(data, size);
    fs.flush();
}

MessageQueue::MessageQueue(const std::string &filename, int cap)
        : filename(filename)
{
    header.capacity = cap;
    header.head = 0;
    header.tail = 0;
    header.count = 0;
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        file.clear();
        file.open(filename, std::ios::out | std::ios::binary);
        if (!file)
            throw std::runtime_error("Невозможно создать файл");
        initFile();
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    } else {
        readHeader();
    }
}

MessageQueue::~MessageQueue() {
    if (file.is_open()) {
        file.close();
    }
}

void MessageQueue::initFile() {
    file.write(reinterpret_cast<char*>(&header), headerSize);
    char empty[MAX_MESSAGE_LENGTH];
    std::memset(empty, 0, MAX_MESSAGE_LENGTH);
    for (int i = 0; i < header.capacity; i++) {
        file.write(empty, MAX_MESSAGE_LENGTH);
    }
    file.flush();
}

void MessageQueue::writeHeader() {
    file.clear();
    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<char*>(&header), headerSize);
    file.flush();
}

void MessageQueue::readHeader() {
    file.clear();
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&header), headerSize);
}

std::streampos MessageQueue::getRecordPos(int index) {
    return headerSize + index * recordSize;
}

bool MessageQueue::sendMessage(const std::string &message) {
    readHeader();
    if (header.count >= header.capacity) {
        return false;
    }
    char buffer[MAX_MESSAGE_LENGTH];
    std::memset(buffer, 0, MAX_MESSAGE_LENGTH);
    std::strncpy(buffer, message.c_str(), MAX_MESSAGE_LENGTH);
    std::streampos pos = getRecordPos(header.tail);
    file.seekp(pos, std::ios::beg);
    file.write(buffer, MAX_MESSAGE_LENGTH);
    file.flush();
    header.tail = (header.tail + 1) % header.capacity;
    header.count++;
    writeHeader();
    return true;
}

std::string MessageQueue::receiveMessage() {
    readHeader();
    if (header.count <= 0) {
        return "";
    }
    char buffer[MAX_MESSAGE_LENGTH + 1];
    std::memset(buffer, 0, MAX_MESSAGE_LENGTH + 1);
    std::streampos pos = getRecordPos(header.head);
    file.seekg(pos, std::ios::beg);
    file.read(buffer, MAX_MESSAGE_LENGTH);
    header.head = (header.head + 1) % header.capacity;
    header.count--;
    writeHeader();
    return std::string(buffer);
}

void MessageQueue::clear() {
    header.head = 0;
    header.tail = 0;
    header.count = 0;
    writeHeader();
    char empty[MAX_MESSAGE_LENGTH];
    std::memset(empty, 0, MAX_MESSAGE_LENGTH);
    for (int i = 0; i < header.capacity; i++) {
        std::streampos pos = getRecordPos(i);
        file.seekp(pos);
        file.write(empty, MAX_MESSAGE_LENGTH);
    }
    file.flush();
}
