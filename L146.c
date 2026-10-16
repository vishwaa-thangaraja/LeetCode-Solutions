#include <stdlib.h>

#define HASH_SIZE 10007

typedef struct Node {
    int key;
    int value;
    struct Node* prev;
    struct Node* next;
    struct Node* hashNext;
} Node;

typedef struct {
    int capacity;
    int size;
    Node* buckets[HASH_SIZE];
    Node* head;  // Most recently used
    Node* tail;  // Least recently used
} LRUCache;

static int hashKey(int key) {
    return key % HASH_SIZE;
}

static Node* findNode(LRUCache* obj, int key) {
    Node* node = obj->buckets[hashKey(key)];
    while (node != NULL) {
        if (node->key == key) return node;
        node = node->hashNext;
    }
    return NULL;
}

static void removeFromList(LRUCache* obj, Node* node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        obj->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        obj->tail = node->prev;
    }
}

static void addToFront(LRUCache* obj, Node* node) {
    node->prev = NULL;
    node->next = obj->head;

    if (obj->head != NULL) {
        obj->head->prev = node;
    } else {
        obj->tail = node;
    }

    obj->head = node;
}

static void removeFromHash(LRUCache* obj, Node* node) {
    int bucket = hashKey(node->key);
    Node** link = &obj->buckets[bucket];

    while (*link != NULL) {
        if (*link == node) {
            *link = node->hashNext;
            return;
        }
        link = &((*link)->hashNext);
    }
}

LRUCache* lRUCacheCreate(int capacity) {
    LRUCache* obj = calloc(1, sizeof(LRUCache));
    if (obj == NULL) return NULL;

    obj->capacity = capacity;
    return obj;
}

int lRUCacheGet(LRUCache* obj, int key) {
    Node* node = findNode(obj, key);
    if (node == NULL) return -1;

    removeFromList(obj, node);
    addToFront(obj, node);
    return node->value;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    Node* node = findNode(obj, key);

    if (node != NULL) {
        node->value = value;
        removeFromList(obj, node);
        addToFront(obj, node);
        return;
    }

    node = malloc(sizeof(Node));
    if (node == NULL) return;

    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    int bucket = hashKey(key);
    node->hashNext = obj->buckets[bucket];
    obj->buckets[bucket] = node;

    addToFront(obj, node);
    obj->size++;

    if (obj->size > obj->capacity) {
        Node* lru = obj->tail;
        removeFromList(obj, lru);
        removeFromHash(obj, lru);
        free(lru);
        obj->size--;
    }
}

void lRUCacheFree(LRUCache* obj) {
    if (obj == NULL) return;

    Node* node = obj->head;
    while (node != NULL) {
        Node* next = node->next;
        free(node);
        node = next;
    }

    free(obj);
}
