#ifndef LINKED_LIST_STRUCT_H
#define LINKED_LIST_STRUCT_H


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


char LinkedList_err_msg[100];

typedef struct LinkedListNode {
    void *val;
    LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *first;
    size_t size;
} LinkedList;

void LinkedList_init(LinkedList *ll);
LinkedListNode LinkedList_getFinal(LinkedList *ll);
LinkedListNode LinkedList_getAt(LinkedList *ll, size_t index);
LinkedListNode *LinkedList_getRefFinal(LinkedList *ll);
LinkedListNode *LinkedList_getRefAt(LinkedList *ll, size_t index);
LinkedListNode **LinkedList_end(LinkedList *ll);
bool LinkedList_headInsert(LinkedList *ll, void *val);
bool LinkedList_tailInsert(LinkedList *ll, void *val);
bool LinkedList_insert(LinkedList *ll, void *val, size_t index);
bool LinkedList_pop(LinkedList *ll, size_t index);
void LinkedList_free(LinkedList *ll);


void LinkedList_init(LinkedList *ll) {
    ll->first = NULL;
    ll->size = 0;
}

LinkedListNode LinkedList_getFinal(LinkedList *ll) {
    if (ll->size == 0) {
        puts("program exited with code 1\nBadFunctionCall: ll->size=0\nwhere: LinkedList_getFinal");
        exit(1);
    }
    if (ll->size == 1) {
        return *(ll->first);
    }
    LinkedListNode *result_ref = ll->first;
    for (size_t i = 0; i < ll->size-1; ++i) {
        result_ref = result_ref->next;
    }
    return *result_ref;
}
LinkedListNode LinkedList_getAt(LinkedList *ll, size_t index) {
    if (ll->size == 0) {
        puts("program exited with code 1\nBadFunctionCall: ll->size=0\nwhere: LinkedList_getAt");
        exit(1);
    }
    if (index >= ll->size) {
        puts("program exited with code 1\nInvalidArgument/OutOfRange: index >= ll->size\nwhere: LinkedList_getAt");
        exit(1);
    }
    if (ll->size == 1) {
        return *(ll->first);
    }
    LinkedListNode *result_ref = ll->first;
    for (size_t i = 0; i < index; ++i) {
        result_ref = result_ref->next;
    }
    return *result_ref;
}

LinkedListNode *LinkedList_getRefFinal(LinkedList *ll) {
    if (ll->size == 0) {
        return NULL;
    }
    if (ll->size == 1) {
        return ll->first;
    }
    LinkedListNode *result = ll->first;
    for (size_t i = 0; i < ll->size-1; ++i) {
        result = result->next;
    }
    return result;
}
LinkedListNode *LinkedList_getRefAt(LinkedList *ll, size_t index) {
    if (index >= ll->size) {
        strcpy(LinkedList_err_msg, "InvalidArgument: index >= ll->size\nwhere: LinkedList_getRefAt\n");
        return NULL;
    }
    if (index == 0) {
        return ll->first;
    }
    LinkedListNode *result = ll->first;
    for (size_t i = 0; i < index; ++i) {
        result = result->next;
    }
    return result;
}
LinkedListNode **LinkedList_end(LinkedList *ll) {
    if (ll->size == 0) {
        return &(ll->first);
    }
    LinkedListNode *final_node = ll->first;
    // get the final node (with val)
    for (size_t i = 0; i < ll->size-1; ++i) {
        final_node = final_node->next;
    }
    return &(final_node->next);
}

bool LinkedList_headInsert(LinkedList *ll, void *val) {
    LinkedListNode *new_first = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    if (new_first == NULL) {
        strcpy(LinkedList_err_msg, "MemoryAllocationFailed\nwhere: LinkedList_insert - index=0\n");
        return false;
    }
    new_first->val = val;
    new_first->next = ll->first;
    ll->first = new_first;
    ++ll->size;
    return true;
}
bool LinkedList_tailInsert(LinkedList *ll, void *val) {
    LinkedListNode *new_final = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    if (new_final == NULL) {
        strcpy(LinkedList_err_msg, "MemoryAllocationFailed\nwhere: LinkedList_tailInsert\n");
        return false;
    }
    LinkedListNode *old_final = LinkedList_getRefFinal(ll);
    old_final->next = new_final;
    new_final->val = val;
    new_final->next = NULL;
    ++ll->size;
    return true;
}

bool LinkedList_insert(LinkedList *ll, void *val, size_t index) {
    if (index >= ll->size) {
        strcpy(LinkedList_err_msg, "InvalidArgument/OutOfRange: index >= ll->size\nwhere: LinkedList_insert\n");
        return false;
    }
    if (index == 0) {
        bool result = LinkedList_headInsert(ll, val);
        if (result) {
            return true;
        }
        strcpy(LinkedList_err_msg + strcspn(LinkedList_err_msg, "\0"), "where: LinkedList_insert\n");
        return false;
    }
    if (index == ll->size-1) {
        bool result = LinkedList_tailInsert(ll, val);
        if (result) {
            return true;
        }
        strcpy(LinkedList_err_msg + strcspn(LinkedList_err_msg, "\0"), "where: LinkedList_insert\n");
        return false;
    }
    LinkedListNode *before = LinkedList_getRefAt(ll, index-1);
    LinkedListNode *after = before->next;
    LinkedListNode *new_node = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    if (new_node == NULL) {
        strcpy(LinkedList_err_msg, "MemoryAllocationFailed\nwhere: LinkedList_insert - index in mid\n");
        return false;
    }
    before->next = new_node;
    new_node->val = val;
    new_node->next = after;
    ++ll->size;
    return true;
}

bool LinkedList_pop(LinkedList *ll, size_t index) {
    if (index >= ll->size) {
        strcpy(LinkedList_err_msg, "InvalidArgument/OutOfRange: index >= ll->size\nwhere: LinkedList_pop\n");
        return false;
    }
    LinkedListNode *before = LinkedList_getRefAt(ll, index-1);
    LinkedListNode *to_del = before->next;
    before->next = before->next->next;
    free(to_del);
    return true;
}
void LinkedList_free(LinkedList *ll) {
    LinkedListNode *current = ll->first;
    for (size_t i = 0; i < ll->size; ++i) {
        LinkedListNode *next = current->next;
        free(current->val);
        free(current);
        current = next;
    }
}

#endif // LINKED_LIST_STRUCT_H