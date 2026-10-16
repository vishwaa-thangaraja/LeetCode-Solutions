#include <stdlib.h>
#include <string.h>

// Doubly Linked List Node representing cached elements
typedef struct Node {
    int key;
    int value;
    int freq;
    struct Node* prev;
    struct Node* next;
} Node;

// Doubly Linked List representing elements with the same frequency
typedef struct DList {
    Node* head;
    Node* tail;
} DList;

// Main LFU Cache structure
typedef struct {
    int capacity;
    int size;
    int minFreq;
    Node** keyMap;     // Hash map for key -> Node* (size 100001 per constraints)
    DList** freqMap;   // Hash map for freq -> DList* (max size 200005 per constraints)
} LFUCache;

// Helper function to create a new empty doubly linked list
DList* createDList() {
    DList* list = (DList*)malloc(sizeof(DList));
    list->head = (Node*)malloc(sizeof(Node));
    list->tail = (Node*)malloc(sizeof(Node));
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->head->prev = NULL;
    list->tail->next = NULL;
    return list;
}

// Helper to remove a node from its current frequency list
void removeNode(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

// Helper to add a node to the head (most recently used position) of a list
void addToHead(DList* list, Node* node) {
    node->next = list->head->next;
    node->prev = list->head;
    list->head->next->prev = node;
    list->head->next = node;
}

// Helper to check if a list is empty (only contains dummy head and tail)
int isListEmpty(DList* list) {
    return list->head->next == list->tail;
}

LFUCache* lFUCacheCreate(int capacity) {
    LFUCache* obj = (LFUCache*)malloc(sizeof(LFUCache));
    obj->capacity = capacity;
    obj->size = 0;
    obj->minFreq = 0;
    
    // Constraints: 0 <= key <= 100000
    obj->keyMap = (Node**)calloc(100001, sizeof(Node*));
    // Max calls 200000 means frequency won't exceed 200005
    obj->freqMap = (DList**)calloc(200005, sizeof(DList*));
    
    return obj;
}

// Updates the frequency counter and shifts the node to its new frequency list
void updateFreq(LFUCache* obj, Node* node) {
    int oldFreq = node->freq;
    int newFreq = oldFreq + 1;
    node->freq = newFreq;
    
    // Remove from the old frequency list
    DList* oldList = obj->freqMap[oldFreq];
    removeNode(node);
    
    // If the old list becomes empty and it was the minimum frequency, increment minFreq
    if (isListEmpty(oldList) && obj->minFreq == oldFreq) {
        obj->minFreq = newFreq;
    }
    
    // Create new frequency list if it doesn't exist yet
    if (!obj->freqMap[newFreq]) {
        obj->freqMap[newFreq] = createDList();
    }
    
    // Add node to the head of the new frequency list
    addToHead(obj->freqMap[newFreq], node);
}

int lFUCacheGet(LFUCache* obj, int key) {
    if (obj->capacity == 0 || obj->keyMap[key] == NULL) {
        return -1;
    }
    Node* node = obj->keyMap[key];
    updateFreq(obj, node);
    return node->value;
}

void lFUCachePut(LFUCache* obj, int key, int value) {
    if (obj->capacity == 0) return;
    
    // Scenario 1: Key already exists -> Update value and frequency
    if (obj->keyMap[key] != NULL) {
        Node* node = obj->keyMap[key];
        node->value = value;
        updateFreq(obj, node);
        return;
    }
    
    // Scenario 2: Cache is full -> Evict LFU (or LRU tie-breaker)
    if (obj->size >= obj->capacity) {
        DList* minList = obj->freqMap[obj->minFreq];
        Node* deleteNode = minList->tail->prev; // Tail->prev is the LRU item
        
        obj->keyMap[deleteNode->key] = NULL;
        removeNode(deleteNode);
        free(deleteNode);
        obj->size--;
    }
    
    // Scenario 3: Insert new item
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->freq = 1;
    
    obj->keyMap[key] = newNode;
    obj->minFreq = 1; // Resets min frequency to 1 for any newly added key
    
    if (!obj->freqMap[1]) {
        obj->freqMap[1] = createDList();
    }
    addToHead(obj->freqMap[1], newNode);
    obj->size++;
}

void lFUCacheFree(LFUCache* obj) {
    if (!obj) return;
    
    // Free all node mappings
    for (int i = 0; i <= 100000; i++) {
        if (obj->keyMap[i]) {
            free(obj->keyMap[i]);
        }
    }
    
    // Free all frequency list structures
    for (int i = 0; i < 200005; i++) {
        if (obj->freqMap[i]) {
            free(obj->freqMap[i]->head);
            free(obj->freqMap[i]->tail);
            free(obj->freqMap[i]);
        }
    }
    
    free(obj->keyMap);
    free(obj->freqMap);
    free(obj);
}
