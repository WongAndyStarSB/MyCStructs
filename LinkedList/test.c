#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char val;
    struct Node *next;
} Node;
typedef struct List {
    Node *first;
} List;
int main() {
    Node a;
    a.next = (Node *)malloc(sizeof(Node));
    a.val = 'a';
    Node *n = &a;
    printf("%p\n", &(n->next));
    printf("%p\n", &(a.next));
    n = n->next;
    printf("%p\n", &(n));

    
    return 0;
}