#ifndef MAZE_H
#define MAZE_H

#include "utils.h"
#include <stdbool.h>

typedef struct maze_s Maze;

void m_delete(Maze *m);

Maze *m_loadFromFile(char *fileName);
bool m_hasSolution(Maze *m);

void m_print(Maze *m);

#endif // MAZE_H