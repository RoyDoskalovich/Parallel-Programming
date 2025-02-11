#include <pthread.h>
#include "LevelQueue.h"


void initLevelQueue(LevelQueue *q) {
    /* Initialize the mutex */
    
    if (pthread_mutex_init(&(q->mutex), NULL) != 0) {
        perror("mutex init has failed.");
        exit(1);
    }

    /* Init start and end to NULL */
    q->end = q->start = NULL;
}


void insertNode(LevelQueue *q, node n) {
/* Create a new node with the given data */
    node *newNode = malloc(sizeof(node));
    newNode->next = NULL;
    newNode->v = n.v;

    /* Synchronously insert the new node */
    pthread_mutex_lock(&q->mutex);

    if ((q->end == NULL && q->start != NULL) ||
        (q->end != NULL && q->start == NULL)) {
        perror("'end' and 'start' are not in sync.");
        exit(1);
    }

    if (q->end == NULL) {
        q->start = q->end = newNode;
    } else {
        q->end->next = newNode;
        q->end = q->end->next;
    }

    pthread_mutex_unlock(&q->mutex);
}


vertex popVertex(LevelQueue *q) {
    /* Synchronously pop the first element */
    pthread_mutex_lock(&q->mutex);

    if ((q->end == NULL && q->start != NULL) ||
        (q->end != NULL && q->start == NULL)) {
        perror("'end' and 'start' are not in sync.");
        exit(1);
    }

    if (q->start == NULL) {
        perror("cannot pop from an empty queue.");
        exit(1);
    }

    node *popNode = q->start;

    if ((q->start = popNode->next) == NULL) {
        q->end = NULL;
    }

    pthread_mutex_unlock(&q->mutex);

    /* Free the popped node and return the data */
    vertex output = popNode->v;
    free(popNode);
    return output;
}


int isLevelQueueEmpty(LevelQueue *q) {
    /* Synchronously check if the queue is empty */
    pthread_mutex_lock(&q->mutex);
    int isEmpty = (q->start == NULL);
    pthread_mutex_unlock(&q->mutex);

    /* Return the result */
    return isEmpty;
}


