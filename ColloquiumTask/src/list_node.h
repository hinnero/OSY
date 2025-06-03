#ifndef LIST_NODE_H
#define LIST_NODE_H

struct ListNode {
    int data;
    ListNode* next;
    ListNode(int d) : data(d), next(nullptr) {}
};

#endif // LIST_NODE_H
