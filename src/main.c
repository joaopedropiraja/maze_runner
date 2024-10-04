#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "array.h"
#include "msgassert.h"

int main(int argc, char* argv[]) {
    msgassert(argc == 2, "Use: %s <mazeFileName>\n", argv[0]);

    Maze *m = m_loadFromFile(argv[1]);
    bool foundExit = m_hasSolution(m);

    if (foundExit) printf("Saída encontrada!\n");
    else printf("Não foi possível encontrar a saída.\n");
    
    m_delete(m);

    return EXIT_SUCCESS;
}
