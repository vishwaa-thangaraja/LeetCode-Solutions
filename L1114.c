#include <stdlib.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int stage;
} Foo;

// Function Declaration, do not remove
void printFirst();
void printSecond();
void printThird();

Foo* fooCreate() {
    Foo* obj = (Foo*)malloc(sizeof(Foo));
    if (obj == NULL) return NULL;

    pthread_mutex_init(&obj->mutex, NULL);
    pthread_cond_init(&obj->cond, NULL);
    obj->stage = 0;

    return obj;
}

void first(Foo* obj) {
    pthread_mutex_lock(&obj->mutex);

    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();

    obj->stage = 1;
    pthread_cond_broadcast(&obj->cond);
    pthread_mutex_unlock(&obj->mutex);
}

void second(Foo* obj) {
    pthread_mutex_lock(&obj->mutex);

    while (obj->stage < 1) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }

    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();

    obj->stage = 2;
    pthread_cond_broadcast(&obj->cond);
    pthread_mutex_unlock(&obj->mutex);
}

void third(Foo* obj) {
    pthread_mutex_lock(&obj->mutex);

    while (obj->stage < 2) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }

    // printThird() outputs "third". Do not change or remove this line.
    printThird();

    obj->stage = 3;
    pthread_cond_broadcast(&obj->cond);
    pthread_mutex_unlock(&obj->mutex);
}

void fooFree(Foo* obj) {
    if (obj == NULL) return;

    pthread_mutex_destroy(&obj->mutex);
    pthread_cond_destroy(&obj->cond);
    free(obj);
}
