#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "maze.h"
#include "msgassert.h"

// Private declarations
typedef struct thread_data_s {
    Maze *m;
    Position p;
} ThreadData;

static Maze *m_create(unsigned rowsNum, unsigned colsNum);
static bool m_isValidLocation(Maze *m, int row, int col);
static void tryThread(Maze *m, int row, int col);
static void *walk(void *data);

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
        row < 0 || (unsigned)row >= m->rowsNum || 
        col < 0 || (unsigned)col >= m->colsNum
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

static void tryThread(Maze *m, int row, int col) {
  if (!m_isValidLocation(m, row, col)) return;
  
  m->activeThreadsNum++;

  ThreadData *threadData = malloc(sizeof(ThreadData));
  threadData->m = m;
  threadData->p = (Position){ row, col };
  
  pthread_t thread;
  pthread_create(&thread, NULL, walk, threadData);
  pthread_detach(thread);
};

static void *walk(void *data) {
  sleep_ms(5);

  ThreadData *threadData = data;

  Maze *m = threadData->m;
  Position p = threadData->p;
  if (m->data[p.row][p.col] == 's') {
    m->foundExit = true;
    m->activeThreadsNum--;

    free(threadData);
    return NULL;
  }

  m->data[p.row][p.col] = '.';

  // Direita
  tryThread(m, p.row, p.col + 1);
  // Esquerda
  tryThread(m, p.row, p.col - 1);
  // Em cima
  tryThread(m, p.row + 1, p.col);
  // Embaixo
  tryThread(m, p.row - 1, p.col);    

  m->activeThreadsNum--;

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