#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <cstring>

const int MAX_MESSAGE_LENGTH = 20;

#pragma pack(push, 1)
struct QueueHeader {
    int head;
    int tail;
    int count;
    int capacity;
};
#pragma pack(pop)

class MessageQueue {
public:
    MessageQueue(const std::string &filename, int cap);
    ~MessageQueue();

    bool sendMessage(const std::string &message);
    std::string receiveMessage();
    void clear();

private:
    std::string filename;
    QueueHeader header;
    std::fstream file;
    const size_t headerSize = sizeof(QueueHeader);
    const size_t recordSize = MAX_MESSAGE_LENGTH;
    void initFile();
    void writeHeader();
    void readHeader();
    std::streampos getRecordPos(int index);
};

#endif // MESSAGE_QUEUE_H
