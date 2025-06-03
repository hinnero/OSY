#include "MessageQueue.h"
#include <gtest/gtest.h>

const std::string testFile = "test_queue.bin";

TEST(MessageQueueTest, SendReceiveOneMessage) {
    MessageQueue mq(testFile, 5);
    mq.clear();
    std::string message = "Hello";
    EXPECT_TRUE(mq.sendMessage(message));
    std::string received = mq.receiveMessage();
    EXPECT_EQ(received, message);
}

TEST(MessageQueueTest, FIFOOrder) {
    MessageQueue mq(testFile, 5);
    mq.clear();
    std::string msg1 = "First";
    std::string msg2 = "Second";
    std::string msg3 = "Third";
    EXPECT_TRUE(mq.sendMessage(msg1));
    EXPECT_TRUE(mq.sendMessage(msg2));
    EXPECT_TRUE(mq.sendMessage(msg3));
    EXPECT_EQ(mq.receiveMessage(), msg1);
    EXPECT_EQ(mq.receiveMessage(), msg2);
    EXPECT_EQ(mq.receiveMessage(), msg3);
}

TEST(MessageQueueTest, ReceiveFromEmptyQueue) {
    MessageQueue mq(testFile, 3);
    mq.clear();
    std::string received = mq.receiveMessage();
    EXPECT_TRUE(received.empty());
}
