#ifndef HEADDOTC
#define HEADDOTC

#include <limits.h>

#define ROW 5
#define COL 7

#define MAX_LEN (ROW + COL)

typedef struct _coord
{
    int x;
    int y;
} coord;

typedef struct _block
{
    int len;
    coord dots[MAX_LEN];
} block;

typedef struct _rect
{
    int up;
    int down;
    int right;
    int left;
} rect;

// anticlockwise
coord surround[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

coord add(coord A, coord B)
{
    coord result = A;
    result.x += B.x;
    result.y += B.y;
    return result;
}

#endif