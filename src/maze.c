#include <stdlib.h>
#include <stdio.h>

#include "maze.h"
#include "stack.h"
#include "msgassert.h"

struct maze_s {
    char **data;
    unsigned rowsNum;
    unsigned colsNum;
    Position initial;
};

Maze *m_create(unsigned rowsNum, unsigned colsNum) {
    Maze *m = (Maze*)malloc(sizeof(Maze));
    msgassert(m != NULL, "Memory allocation failed for maze: { rowsNum: %u, colsNum: %u }\n", rowsNum, colsNum);

    m->rowsNum = rowsNum;
    m->colsNum = colsNum;

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

bool m_isValidLocation(Maze *m, int row, int col) {
    if (
        row < 0 || row > (int)m->rowsNum || 
        col < 0 || col > (int)m->colsNum
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

bool m_hasSolution(Maze *m) {
    Stack *s = st_create(2);
    st_insert(s, m->initial);

    while (!st_empty(s)) {
        Position sn = st_pop(s);
        if (m->data[sn.row][sn.col] == 's') 
            return true;

        if (m->data[sn.row][sn.col] != 'e') 
            m->data[sn.row][sn.col] = '.';

        m_print(m);
        sleep_ms(100);

        int row = sn.row + 1;
        int col = sn.col;
        if (m_isValidLocation(m, row, col)) 
            st_insert(s, (Position){ row, col });

        row = sn.row - 1;
        if (m_isValidLocation(m, row, col))
            st_insert(s, (Position){ row, col });

        row = sn.row;
        col = sn.col + 1;
        if (m_isValidLocation(m, row, col)) 
            st_insert(s, (Position){ row, col });        

        col = sn.col - 1;
        if (m_isValidLocation(m, row, col))
            st_insert(s, (Position){ row, col });
    }

    st_delete(s);
    
    return false;
};