#ifndef UTILS_H
#define UTILS_H

typedef struct position_s {
    unsigned row;
    unsigned col;
} Position;

void sleep_ms(int milliseconds);

#endif // UTILS_H