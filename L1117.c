#include <stdlib.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int hydrogenCount;
    int oxygenCount;
    int ready;
    int finished;
} H2O;

void releaseHydrogen();
void releaseOxygen();

H2O* h2oCreate() {
    H2O* obj = (H2O*)malloc(sizeof(H2O));
    if (obj == NULL) return NULL;

    pthread_mutex_init(&obj->mutex, NULL);
    pthread_cond_init(&obj->cond, NULL);

    obj->hydrogenCount = 0;
    obj->oxygenCount = 0;
    obj->ready = 0;
    obj->finished = 0;

    return obj;
}

void hydrogen(H2O* obj) {
    pthread_mutex_lock(&obj->mutex);

    while (obj->ready || obj->hydrogenCount == 2) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }

    obj->hydrogenCount++;

    if (obj->hydrogenCount == 2 && obj->oxygenCount == 1) {
        obj->ready = 1;
        pthread_cond_broadcast(&obj->cond);
    }

    while (!obj->ready) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }

    // releaseHydrogen() outputs "H". Do not change or remove this line.
    releaseHydrogen();

    obj->finished++;
    if (obj->finished == 3) {
        obj->hydrogenCount = 0;
        obj->oxygenCount = 0;
        obj->ready = 0;
        obj->finished = 0;
        pthread_cond_broadcast(&obj->cond);
    }

    pthread_mutex_unlock(&obj->mutex);
}

void oxygen(H2O* obj) {
    pthread_mutex_lock(&obj->mutex);

    while (obj->ready || obj->oxygenCount == 1) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }

    obj->oxygenCount++;

    if (obj->hydrogenCount == 2 && obj->oxygenCount == 1) {
        obj->ready = 1;
        pthread_cond_broadcast(&obj->cond);
    }

    while (!obj->ready) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }

    // releaseOxygen() outputs "O". Do not change or remove this line.
    releaseOxygen();

    obj->finished++;
    if (obj->finished == 3) {
        obj->hydrogenCount = 0;
        obj->oxygenCount = 0;
        obj->ready = 0;
        obj->finished = 0;
        pthread_cond_broadcast(&obj->cond);
    }

    pthread_mutex_unlock(&obj->mutex);
}

void h2oFree(H2O* obj) {
    if (obj == NULL) return;

    pthread_mutex_destroy(&obj->mutex);
    pthread_cond_destroy(&obj->cond);
    free(obj);
}
