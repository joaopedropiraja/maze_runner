#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "maze.h"
#include "stack.h"
#include "msgassert.h"

// Private functions declarations
static Maze *m_create(unsigned rowsNum, unsigned colsNum);
static bool m_isValidLocation(Maze *m, int row, int col);
static void *walk(void *data);
typedef struct thread_data_s {
    Maze *m;
    Position p;
} ThreadData;

// Implementations

struct maze_s {
    char **data;
    unsigned rowsNum;
    unsigned colsNum;
    Position initial;
    bool foundExit;
    unsigned activeThreadsNum;
};

static Maze *m_create(unsigned rowsNum, unsigned colsNum) {
    Maze *m = (Maze*)malloc(sizeof(Maze));
    msgassert(m != NULL, "Memory allocation failed for maze: { rowsNum: %u, colsNum: %u }\n", rowsNum, colsNum);

    m->rowsNum = rowsNum;
    m->colsNum = colsNum;
    m->foundExit = false;
    m->activeThreadsNum = 0;

    m->data = (char**)malloc(rowsNum * sizeof(char*));
    msgassert(m->data != NULL, "Memory allocation failed for maze: { rowsNum: %u, colsNum: %u }\n", rowsNum, colsNum);
    for (unsigned i = 0; i < rowsNum; i++) {
        m->data[i] = (char*)malloc(colsNum);
        msgassert(m->data[i] != NULL, "Memory allocation failed for maze: { rowsNum: %u, colsNum: %u }\n", rowsNum, colsNum);
    }

    return m;
};

void m_delete(Maze *m) {
    if (!m) return;

    for (unsigned i = 0; i < m->rowsNum; i++) {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
};

Maze *m_loadFromFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    msgassert(file != NULL, "Failed to open file %s.", fileName);
    
    unsigned rowsNum, colsNum;
    fscanf(file, "%u %u\n", &rowsNum, &colsNum);    
    
    Maze *m = m_create(rowsNum, colsNum);

    char buffer[colsNum];
    for (unsigned i = 0; i < rowsNum; i++) {
        fscanf(file, "%s\n", buffer);
        for (unsigned j = 0; j < colsNum; j++) {
            if ((m->data[i][j] = buffer[j]) == 'e') {
                m->initial = (Position){ i, j };
            }
        }
    }

    fclose(file);

    return m;
};

static bool m_isValidLocation(Maze *m, int row, int col) {
    if (
        row < 0 || row >= (int)m->rowsNum || 
        col < 0 || col >= (int)m->colsNum
    ) return false;

    return m->data[row][col] == 'x' || m->data[row][col] == 's';
};

void m_print(Maze *m) {
    for (unsigned i = 0; i < m->rowsNum; i++) {
        for (unsigned j = 0; j < m->colsNum; j++) {
            printf("%c", m->data[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");    
};

static void *walk(void *data) {
  ThreadData *threadData = data;
  if (threadData->m->data[threadData->p.row][threadData->p.col] == 's') {
    threadData->m->foundExit = true;
    threadData->m->activeThreadsNum--;

    free(threadData);
    return NULL;
  }

  sleep_ms(15);
  threadData->m->data[threadData->p.row][threadData->p.col] = '.';

  int row = threadData->p.row + 1;
  int col = threadData->p.col;
  if (m_isValidLocation(threadData->m, row, col)) {
    ThreadData *newThreadData1 = malloc(sizeof(ThreadData));
    newThreadData1->m = threadData->m;
    
    pthread_t newThread1;
    newThreadData1->p = (Position){ row, col };

    threadData->m->activeThreadsNum++;
    pthread_create(&newThread1, NULL, walk, newThreadData1);
    pthread_detach(newThread1);
  }

  row = threadData->p.row - 1;
  if (m_isValidLocation(threadData->m, row, col)) {
    ThreadData *newThreadData2 = malloc(sizeof(ThreadData));
    newThreadData2->m = threadData->m;

    pthread_t newThread2;
    newThreadData2->p = (Position){ row, col };

    threadData->m->activeThreadsNum++;
    pthread_create(&newThread2, NULL, walk, newThreadData2);
    pthread_detach(newThread2);    
  }

  row = threadData->p.row;
  col = threadData->p.col + 1;
  if (m_isValidLocation(threadData->m, row, col)) {
    ThreadData *newThreadData3 = malloc(sizeof(ThreadData));
    newThreadData3->m = threadData->m;

    pthread_t newThread3;
    newThreadData3->p = (Position){ row, col };

    threadData->m->activeThreadsNum++;
    pthread_create(&newThread3, NULL, walk, newThreadData3);
    pthread_detach(newThread3);    
  }     

  col = threadData->p.col - 1;
  if (m_isValidLocation(threadData->m, row, col)) {
    ThreadData *newThreadData4 = malloc(sizeof(ThreadData));
    newThreadData4->m = threadData->m;

    pthread_t newThread4;
    newThreadData4->p = (Position){ row, col };

    threadData->m->activeThreadsNum++;
    pthread_create(&newThread4, NULL, walk, newThreadData4);
    pthread_detach(newThread4);    
  }

  threadData->m->activeThreadsNum--;

  free(threadData);
  return NULL;
}

bool m_hasSolution(Maze *m) {

  pthread_t startThread;
  ThreadData *threadData = malloc(sizeof(ThreadData));
  
  threadData->m = m;
  threadData->p = m->initial;

  m->activeThreadsNum++;
  pthread_create(&startThread, NULL, walk, threadData);
  pthread_detach(startThread);

  while (!m->foundExit && m->activeThreadsNum > 0) {
    m_print(m);
    sleep_ms(50);
  }

  return m->foundExit;
};