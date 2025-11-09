#ifndef THREAD_SAFE_LINKED_LIST_H
#define THREAD_SAFE_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../Mutex/Mutex.h"

char LinkedList_err_msg[100];

typedef struct LinkedListNode {
    void *val;
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *first;
    size_t size;
#if PLATFORM_WINDOWS
    CRITICAL_SECTION lock;
#elif PLATFORM_POSIX
    pthread_mutex_t lock;
#endif
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
    Mutex_init(&ll->lock);
}

LinkedListNode LinkedList_getFinal(LinkedList *ll) {
    Mutex_lock(&ll->lock);
    if (ll->size == 0) {
        Mutex_unlock(&ll->lock);
        puts("program exited with code 1\nBadFunctionCall: ll->size=0\nwhere: LinkedList_getFinal");
        exit(1);
    }
    if (ll->size == 1) {
        LinkedListNode result = *(ll->first);
        Mutex_unlock(&ll->lock);
        return result;
    }
    LinkedListNode *result_ref = ll->first;
    for (size_t i = 0; i < ll->size-1; ++i) {
        result_ref = result_ref->next;
    }
    LinkedListNode result = *result_ref;
    Mutex_unlock(&ll->lock);
    return result;
}

LinkedListNode LinkedList_getAt(LinkedList *ll, size_t index) {
    Mutex_lock(&ll->lock);
    if (ll->size == 0) {
        Mutex_unlock(&ll->lock);
        puts("program exited with code 1\nBadFunctionCall: ll->size=0\nwhere: LinkedList_getAt");
        exit(1);
    }
    if (index >= ll->size) {
        Mutex_unlock(&ll->lock);
        puts("program exited with code 1\nInvalidArgument/OutOfRange: index >= ll->size\nwhere: LinkedList_getAt");
        exit(1);
    }
    if (ll->size == 1) {
        LinkedListNode result = *(ll->first);
        Mutex_unlock(&ll->lock);
        return result;
    }
    LinkedListNode *result_ref = ll->first;
    for (size_t i = 0; i < index; ++i) {
        result_ref = result_ref->next;
    }
    LinkedListNode result = *result_ref;
    Mutex_unlock(&ll->lock);
    return result;
}

LinkedListNode *LinkedList_getRefFinal(LinkedList *ll) {
    Mutex_lock(&ll->lock);
    if (ll->size == 0) {
        Mutex_unlock(&ll->lock);
        return NULL;
    }
    if (ll->size == 1) {
        LinkedListNode *result = ll->first;
        Mutex_unlock(&ll->lock);
        return result;
    }
    LinkedListNode *result = ll->first;
    for (size_t i = 0; i < ll->size-1; ++i) {
        result = result->next;
    }
    Mutex_unlock(&ll->lock);
    return result;
}

LinkedListNode *LinkedList_getRefAt(LinkedList *ll, size_t index) {
    Mutex_lock(&ll->lock);
    if (index >= ll->size) {
        strcpy(LinkedList_err_msg, "InvalidArgument: index >= ll->size\nwhere: LinkedList_getRefAt\n");
        Mutex_unlock(&ll->lock);
        return NULL;
    }
    if (index == 0) {
        LinkedListNode *result = ll->first;
        Mutex_unlock(&ll->lock);
        return result;
    }
    LinkedListNode *result = ll->first;
    for (size_t i = 0; i < index; ++i) {
        result = result->next;
    }
    Mutex_unlock(&ll->lock);
    return result;
}

LinkedListNode **LinkedList_end(LinkedList *ll) {
    Mutex_lock(&ll->lock);
    if (ll->size == 0) {
        Mutex_unlock(&ll->lock);
        return &(ll->first);
    }
    LinkedListNode *final_node = ll->first;
    for (size_t i = 0; i < ll->size-1; ++i) {
        final_node = final_node->next;
    }
    Mutex_unlock(&ll->lock);
    return &(final_node->next);
}

void LinkedList_free(LinkedList *ll) {
    Mutex_lock(&ll->lock);
    LinkedListNode *current = ll->first;
    for (size_t i = 0; i < ll->size; ++i) {
        LinkedListNode *next = current->next;
        free(current->val);
        free(current);
        current = next;
    }
    Mutex_unlock(&ll->lock);
    Mutex_destroy(&ll->lock);
}

bool LinkedList_headInsert(LinkedList *ll, void *val) {
    Mutex_lock(&ll->lock);
    LinkedListNode *new_node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    if (!new_node) {
        Mutex_unlock(&ll->lock);
        return false;
    }
    new_node->val = val;
    new_node->next = ll->first;
    ll->first = new_node;
    ll->size++;
    Mutex_unlock(&ll->lock);
    return true;
}

bool LinkedList_tailInsert(LinkedList *ll, void *val) {
    Mutex_lock(&ll->lock);
    LinkedListNode *new_node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    if (!new_node) {
        Mutex_unlock(&ll->lock);
        return false;
    }
    new_node->val = val;
    new_node->next = NULL;
    if (ll->size == 0) {
        ll->first = new_node;
    } else {
        LinkedListNode *final_node = ll->first;
        for (size_t i = 0; i < ll->size-1; ++i) {
            final_node = final_node->next;
        }
        final_node->next = new_node;
    }
    ll->size++;
    Mutex_unlock(&ll->lock);
    return true;
}

bool LinkedList_insert(LinkedList *ll, void *val, size_t index) {
    Mutex_lock(&ll->lock);
    if (index > ll->size) {
        Mutex_unlock(&ll->lock);
        return false;
    }
    LinkedListNode *new_node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    if (!new_node) {
        Mutex_unlock(&ll->lock);
        return false;
    }
    new_node->val = val;
    if (index == 0) {
        new_node->next = ll->first;
        ll->first = new_node;
    } else {
        LinkedListNode *prev_node = ll->first;
        for (size_t i = 0; i < index-1; ++i) {
            prev_node = prev_node->next;
        }
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }
    ll->size++;
    Mutex_unlock(&ll->lock);
    return true;
}

bool LinkedList_pop(LinkedList *ll, size_t index) {
    Mutex_lock(&ll->lock);
    if (index >= ll->size) {
        Mutex_unlock(&ll->lock);
        return false;
    }
    LinkedListNode *to_remove;
    if (index == 0) {
        to_remove = ll->first;
        ll->first = to_remove->next;
    } else {
        LinkedListNode *prev_node = ll->first;
        for (size_t i = 0; i < index-1; ++i) {
            prev_node = prev_node->next;
        }
        to_remove = prev_node->next;
        prev_node->next = to_remove->next;
    }
    free(to_remove->val);
    free(to_remove);
    ll->size--;
    Mutex_unlock(&ll->lock);
    return true;
}

#endif // THREAD_SAFE_LINKED_LIST_H