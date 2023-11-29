#include <linux/slab.h>
#include <linux/printk.h>
#include "btree_cache.h"

typedef struct Node {
    unsigned long *node;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} CircularQueue;

void initQueue(CircularQueue* q) {
    Node *current, *previous = NULL;
    Node *first = NULL;

    for (int i = 0; i < 4; i++) {
        current = kmalloc(sizeof(Node), GFP_KERNEL);
        if (!current) {
            printk(KERN_ERR "Memory allocation failed for node %d\n", i);
            current = first;
            while (current) {
                Node *temp = current->next;
                kfree(current);
                current = temp;
            }
            return;
        }

        current->node = NULL;  
        current->next = NULL;

        if (i == 0) {
            first = current;
        } else {
            previous->next = current;
        }

        previous = current;
    }

    current->next = first;
    q->head = first;
}

void setNodeValue(CircularQueue* q, unsigned long * value) {
    Node* current = q->head;
    current->node = value;
}

void* getNodeValue(CircularQueue* q) {
    Node* current = q->head;
    return current->node;
}

void freeQueue(CircularQueue* q) {
    Node *current = q->head;

    if (!current) {
        return;
    }

    Node *first = current;
    do {
        Node *temp = current;
        current = current->next;
        kfree(temp);
    } while (current != first);
}
