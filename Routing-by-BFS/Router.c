#ifndef BFS_ROUTER_C
#define BFS_ROUTER_C

#include "../code/bases.h"

coord directions[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
int route(coord dest, coord start, char map[ROW_MAX][COL_MAX], coord size, queuet body)
{
    queuet queue = {0, 0};
    queue.elems[queue.rear++] = start;
    queue.rear = queue.rear % QMAX;

    int count = 0;
    int searched[ROW_MAX][COL_MAX] = {0};
    searched[start.x][start.y] = 1;

    coord current, tmp;

    int rear_tmp;
    while (queue.front != queue.rear)
    {
        rear_tmp = queue.rear;
        count++;
        while (queue.front != rear_tmp)
        {
            current = queue.elems[queue.front++];
            queue.front = queue.front % QMAX;

            int flag = 0;
            for (int i = 0; i < 4; i++)
            {
                tmp = add(current, directions[i]);
                if (in_map(tmp, size))
                {
                    if (map[tmp.x][tmp.y] != WALL &&
                        !overlap(tmp, body, (body.rear - body.front + QMAX) % QMAX - 1, QMAX) &&
                        !(tmp.x == body.elems[body.front].x &&
                          tmp.y == body.elems[body.front].y &&
                          (body.rear + QMAX - body.front) % QMAX == 2))
                    {
                        if (tmp.x == dest.x && tmp.y == dest.y)
                        {
                            return count;
                        }

                        if (searched[tmp.x][tmp.y] == 0)
                        {
                            flag = 1;
                            searched[tmp.x][tmp.y] = 1;
                            queue.elems[queue.rear++] = tmp;
                            queue.rear = queue.rear % QMAX;
                        }
                    }
                }
            }
        }
        body.front++; //======================================================================
    }
    return -1;
}

coord add(coord A, coord B)
{
    coord point = {A.x + B.x, A.y + B.y};
    return point;
}

int in_map(coord point, coord size)
{
    if (point.x < 0 || point.x >= size.x)
    {
        return 0;
    }
    if (point.y < 0 || point.y >= size.y)
    {
        return 0;
    }
    return 1;
}

#endif // BFS_ROUTER_C