// #define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

#include "../includes/head.h"

#define WALL 1
#define BLANK 0
#define SEARCHT_WALL -1

int route(int map[ROW][COL], coord start, coord dest);
void get_wall(coord point, block *wall, int map[ROW][COL]);
rect get_rect(block wall);
block simple(coord start, coord dest);
int in_map(coord point, int down_limit, int right_limit);

#ifdef DEBUG

#define PRT(block)                                             \
    for (int i = 0; i < block.len; i++)                        \
    {                                                          \
        printf("(%d,%d)", block.elems[i].x, block.elems[i].y); \
    }

int main()
{
    int map[ROW][COL] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}};
    // coord start = {2, 1}, dest = {2, 5};
    coord start = {0, 0}, dest = {4, 6};

    coord blovker = {2, 3};
    // route(map, start, dest);
    // block wall = {3, {{1, 3}, {2, 3}, {3, 3}}};
    block wall = {0};
    get_wall(blovker, &wall, map);
    PRT(wall);
}
#endif

int route(int map[ROW][COL], coord start, coord dest)
{
    block line = simple(start, dest);

#ifdef DEBUG
    for (int i = 0; i < line.len; i++)
    {
        printf("(%d,%d)", line.elems[i].x, line.elems[i].y);
    }
#endif

    for (int i = 0; i < line.len; i++)
    {
        // now only sigle blocker
        if (map[line.elems[i].x][line.elems[i].y] == WALL)
        {
            // get_wall();
            break;
        }
    }
}

block simple(coord start, coord dest)
{
    block line = {0};
    // line.len = 0;
    coord tmp = start;

    line.elems[line.len] = tmp;
    line.len++;

    if (dest.y - start.y >= 0)
    {
        for (int i = 0; i < dest.y - start.y; i++)
        {
            tmp.y++;
            line.elems[line.len] = tmp;
            line.len++;
        }
    }
    else
    {
        for (int i = 0; i < start.y - dest.y; i++)
        {
            tmp.y--;
            line.elems[line.len] = tmp;
            line.len++;
        }
    }

    if (dest.x - start.x >= 0)
    {
        for (int i = 0; i < dest.x - start.x; i++)
        {
            tmp.x++;
            line.elems[line.len] = tmp;
            line.len++;
        }
    }
    else
    {
        for (int i = 0; i < start.x - dest.x; i++)
        {
            tmp.x--;
            line.elems[line.len] = tmp;
            line.len++;
        }
    }
    return line;
}

// only when not connected with edges
void get_wall(coord point, block *wall, int map[ROW][COL])
{
    // block wall = {0};
    (*wall).elems[(*wall).len++] = point;
    map[point.x][point.y] = SEARCHT_WALL;

    coord nearby;
    for (int i = 0; i < 8; i++)
    {
        nearby = add(point, surround[i]);
        if (in_map(nearby, ROW, COL))
        {
            if (map[nearby.x][nearby.y] == WALL)
            {
                get_wall(nearby, wall, map);
            }
        }
    }
    map[point.x][point.y] == WALL;
}

int in_map(coord point, int down_limit, int right_limit)
{
    if (point.x < 0 || point.x >= down_limit)
    {
        return 0;
    }
    if (point.y < 0 || point.y >= right_limit)
    {
        return 0;
    }
    return 1;
}

rect get_rect(block wall)
{
    rect closure = {INT_MAX, INT_MIN, INT_MIN, INT_MAX};

    for (int i = 0; i < wall.len; i++)
    {
        if (wall.elems[i].x > closure.down)
        {
            closure.down = wall.elems[i].x;
        }
        if (wall.elems[i].x < closure.up)
        {
            closure.up = wall.elems[i].x;
        }
        if (wall.elems[i].y > closure.right)
        {
            closure.right = wall.elems[i].y;
        }
        if (wall.elems[i].y < closure.left)
        {
            closure.left = wall.elems[i].y;
        }
    }
    return closure;
}
