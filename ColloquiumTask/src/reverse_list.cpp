#include "reverse_list.h"

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* current = head;
    while(current) {
        ListNode* next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}
