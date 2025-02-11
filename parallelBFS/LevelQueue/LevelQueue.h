#ifndef LEVEL_QUEUE
#define LEVEL_QUEUE

#include "../SyncGraph/graph.h"

typedef struct {
    node *start;
    node *end;
    pthread_mutex_t mutex;
} LevelQueue;

void initLevelQueue(LevelQueue *q);

void insertNode(LevelQueue *q, node n);

vertex popVertex(LevelQueue *q);

int isLevelQueueEmpty(LevelQueue *q);

#endif // LEVEL_QUEUE