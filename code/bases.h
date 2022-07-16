#ifndef BASE_H
#define BASE_H

#include <limits.h>

#define ROW_MAX (20 + 5)
#define COL_MAX (20 + 5)

// to modify:
#define MAX_LEN (ROW_MAX + COL_MAX)

#define WALL '#'
#define BLANK '.'

#define FOOD 'o'
#define SHIELD 'O'

#define PLAYER_A '1'
#define PLAYER_B '2'
#define PLAYER_OVERLAP '3'

typedef struct _coord
{
    int x;
    int y;
} coord;

typedef struct _block
{
    int len;
    coord elems[MAX_LEN];
} block;

typedef struct _mapt
{
    coord size;
    int elems[ROW_MAX][COL_MAX];
} mapt;

#define QMAX (ROW_MAX * COL_MAX + 5)
typedef struct _queuet
{
    int front, rear;
    coord elems[QMAX];
} queuet;

// full: (obj.rear + 1) % QMAX ==obj.front
// empty: obj.rear == obj.front
// size: (obj.rear + QMAX - obj.front) % QMAX;

// anticlockwise
coord surround[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

coord add(coord A, coord B)
{
    coord result = {A.x + B.x, A.y + B.y};
    return result;
}

// int overlap(coord point, coord *points, int len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         if (point.x == points[i].x && point.y == points[i].y)
//         {
//             return 1;
//         }
//     }
//     return 0;
// }

int overlap(coord point, queuet body, int len, int qmax)
{
    int count = 0;
    for (int i = body.front; count < len; count++)
    {
        i = i % qmax;
        if (point.x == body.elems[i].x && point.y == body.elems[i].y)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

#endif // BASE_H