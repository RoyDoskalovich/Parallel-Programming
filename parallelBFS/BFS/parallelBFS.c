#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../SyncGraph/graph.h"
#include "../LevelQueue/LevelQueue.h"
#include "../ThreadPool/ThreadPool.h"
#include "../ThreadPool/TaskQueue.h"

#define NUM_THREADS 4

typedef struct
{
    int startRow;
    int endRow;
    int **matrix;
    int numVertices;
} InitMatrixThreadArgs;

typedef struct
{
    Graph *graph;
    vertex src;
    vertex v;
    int *distances;
    int **matrix;
    LevelQueue *nextLevel;
    ThreadPool *q;
} bfsArgs;

void *initPartMatrix(void *args)
{
    InitMatrixThreadArgs *initMatrixThreadArgs = (InitMatrixThreadArgs *)args;
    int startRow = initMatrixThreadArgs->startRow;
    int endRow = initMatrixThreadArgs->endRow;
    int **matrix = initMatrixThreadArgs->matrix;
    int numVertices = initMatrixThreadArgs->numVertices;

    for (int i = startRow; i < endRow; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            if (i != j)
            {
                matrix[i][j] = -1;
            }
            else
            {
                matrix[i][j] = 0;
            }
        }
    }
    pthread_exit(NULL);
}

void initMatrix(int **m, int numVertices)
{

    pthread_t threads[NUM_THREADS];
    InitMatrixThreadArgs initMatrixThreadArgs[NUM_THREADS];

    int rowPerThread = numVertices / NUM_THREADS;
    int remainingRows = numVertices % NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        initMatrixThreadArgs[i].startRow = i * rowPerThread;
        initMatrixThreadArgs[i].endRow = (i == NUM_THREADS) ? numVertices : (i + 1) * rowPerThread;
        initMatrixThreadArgs[i].matrix = m;
        initMatrixThreadArgs[i].numVertices = numVertices;
        if(i == NUM_THREADS - 1) {
            initMatrixThreadArgs[i].endRow += remainingRows;
        }

        pthread_create(&threads[i], NULL, initPartMatrix, (void *)&initMatrixThreadArgs[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
   
}

void *bfsParallelPart(void *args)
{
    bfsArgs *data = (bfsArgs *)args;
    Graph *graph = data->graph;
    vertex src = data->src;
    vertex v = data->v;
    int *distances = data->distances;
    int **matrix = data->matrix;
    LevelQueue *nextLevel = data->nextLevel;
    ThreadPool *q = data->q;
    free(data);

    graph->numVisits[v]++; // Color[v] <- black.
    node *neighborPtr = graph->adjacencyLists[v];

    while (neighborPtr != NULL)
    {
        vertex neighbor = neighborPtr->v;

        /* Synchronously increment the number of visits for the neighbor */
        pthread_mutex_lock(&graph->num_visits_mutexes[neighbor]);
        int neighborVisits = graph->numVisits[neighbor]++;
        pthread_mutex_unlock(&graph->num_visits_mutexes[neighbor]);

        /* If the neighbor has not been visited, add a new task to the queue */
        if (neighborVisits == 0)
        {
            insertNode(nextLevel, *neighborPtr); // TODO: Make sure it's fine!
            int temp = distances[v] + 1;
            int temp2 = matrix[src][neighbor];
            if (temp2 == -1)
            {
                matrix[src][neighbor] = temp;
                distances[neighbor] = temp;
            }
            else
            {
                matrix[src][neighbor] = temp < temp2 ? temp : temp2;
                distances[neighbor] = temp < temp2 ? temp : temp2;
            }
          
           
        }
        neighborPtr = neighborPtr->next;
     
    }
    q->runningThreads--;
}

void bfs(Graph *graph, int **m)
{
    int numVertices = graph->numVertices;
    initMatrix(m, numVertices);
     /* Print matrix */
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("%d ", m[i][j]);
        }
        putchar('\n');
    }

    int distances[numVertices];

    for (vertex s = 0; s < numVertices; s++)
    {

        for (vertex v = 0; v < numVertices; v++)
        {
            graph->numVisits[v] = 0; // Color[v] <- white.
            distances[v] = -1;
        }

        distances[s] = 0;
        graph->numVisits[s]++; // Color[s] <- grey.
        int level = 0;
        LevelQueue *currentLevel = (LevelQueue *)(malloc)(sizeof(LevelQueue)); // TODO: Check if it's suppose to be NULL.
        initLevelQueue(currentLevel);
        LevelQueue *nextLevel = (LevelQueue *)(malloc)(sizeof(LevelQueue)); // TODO: Check if it's suppose to be NULL.
        initLevelQueue(nextLevel);
        node *nodeS = createNode(s);
        insertNode(currentLevel, *nodeS);
        while (1)
        {
            /* Init thread pool */
            TaskQueue q;
            initQueue(&q);
            ThreadPool pool;
            pool.maxThreads = 4;
            pool.runningThreads = 0;
            pool.q = &q;

            while (!isLevelQueueEmpty(currentLevel))
            {
                vertex v = popVertex(currentLevel);
                bfsArgs *args = malloc(sizeof(bfsArgs));
                args->graph = graph;
                args->src = s;
                args->v = v;
                args->distances = (int *)distances; // TODO: Make sure that it passed by reference.
                args->matrix = m;
                args->nextLevel = nextLevel;
                args->q = &pool;

                TaskData td = {bfsParallelPart, args};
                insert(pool.q, td);
            }
            runThreadPool(&pool);
            // TODO: Need to add barrier to finish all task before moving to next level.
            printf("Pool ran\n");
            if (isLevelQueueEmpty(nextLevel))
            {
                break;
            }
            LevelQueue *temp = currentLevel;
            currentLevel = nextLevel;
            nextLevel = temp;
        }
    }
    printf("BFS finished\n");
}