#include <stdlib.h>

typedef struct {
    int enq;
    int proc;
    int idx;
} Task;

typedef struct {
    long long proc;
    int idx;
} Item;

static int cmpTask(const void *x, const void *y) {
    const Task *a = (const Task *)x;
    const Task *b = (const Task *)y;

    if (a->enq < b->enq) return -1;
    if (a->enq > b->enq) return 1;
    return (a->idx > b->idx) - (a->idx < b->idx);
}

static int less(Item a, Item b) {
    return a.proc < b.proc ||
           (a.proc == b.proc && a.idx < b.idx);
}

static void push(Item *heap, int *size, Item value) {
    int i = (*size)++;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!less(value, heap[parent])) break;
        heap[i] = heap[parent];
        i = parent;
    }

    heap[i] = value;
}

static Item pop(Item *heap, int *size) {
    Item result = heap[0];
    Item last = heap[--(*size)];

    if (*size > 0) {
        int i = 0;

        while (2 * i + 1 < *size) {
            int child = 2 * i + 1;
            if (child + 1 < *size && less(heap[child + 1], heap[child])) {
                child++;
            }

            if (!less(heap[child], last)) break;

            heap[i] = heap[child];
            i = child;
        }

        heap[i] = last;
    }

    return result;
}

int* getOrder(int** tasks, int tasksSize, int* tasksColSize, int* returnSize) {
    (void)tasksColSize;

    Task *sorted = malloc((size_t)tasksSize * sizeof(Task));
    Item *heap = malloc((size_t)tasksSize * sizeof(Item));
    int *answer = malloc((size_t)tasksSize * sizeof(int));

    for (int j = 0; j < tasksSize; j++) {
        sorted[j].enq = tasks[j][0];
        sorted[j].proc = tasks[j][1];
        sorted[j].idx = j;
    }

    qsort(sorted, tasksSize, sizeof(Task), cmpTask);

    int next = 0;
    int heapSize = 0;
    int completed = 0;
    long long time = 0;

    while (completed < tasksSize) {
        if (heapSize == 0 && next < tasksSize && time < sorted[next].enq) {
            time = sorted[next].enq;
        }

        while (next < tasksSize && sorted[next].enq <= time) {
            push(heap, &heapSize, (Item){sorted[next].proc, sorted[next].idx});
            next++;
        }

        Item task = pop(heap, &heapSize);
        answer[completed++] = task.idx;
        time += task.proc;
    }

    free(sorted);
    free(heap);

    *returnSize = tasksSize;
    return answer;
}
