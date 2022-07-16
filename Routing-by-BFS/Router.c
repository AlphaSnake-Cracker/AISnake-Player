#ifndef BFS_ROUTER_C
#define BFS_ROUTER_C

#include <stdio.h>
#include <limits.h>

#define MAX (15 + 5)
#define DEST '@'
#define BLOCK '#'
#define HEAD '1'
#define BLANK '.'
#define SEARCHT (0)

typedef struct _coord
{
    int x, y;
} coord;

#define QMAX (MAX * MAX)
typedef struct _queuet
{
    int front, rear;
    coord elems[QMAX];
} queuet;

// full: (queue.rear + 1) % QMAX ==queue.front
// empty: queue.rear == queue.front
// size: (queue.rear + QMAX - queue.front) % QMAX;
int in_map(coord point, coord size);
coord add(coord A, coord B);
int routing(coord start, char map[MAX][MAX], coord size, queuet body);

#ifdef MAIN
int main()
{
    int m = -1, n = -1;
    scanf("%d %d", &m, &n);
    coord size = {m, n};

    char map[MAX][MAX] = {0};
    coord start = {-1, -1};
    queuet body = {INT_MAX, INT_MIN};

    for (int i = 0; i < m; i++)
    {
        scanf("%s", &map[i]);
        for (int j = 0; j < n; j++)
        {
            if ('1' <= map[i][j] && map[i][j] <= '9')
            {
                body.elems['9' - map[i][j]].x = i;
                body.elems['9' - map[i][j]].y = j;
                if (body.rear <= '9' - map[i][j])
                {
                    body.rear = '9' - map[i][j] + 1;
                }
                if (body.front > '9' - map[i][j])
                {
                    body.front = '9' - map[i][j];
                }
                // No %QMAX NEEDED

                if (map[i][j] == '1')
                {
                    map[i][j] = SEARCHT;
                    start.x = i;
                    start.y = j;
                }
                else
                {
                    map[i][j] = BLOCK;
                }
            }
        }
    }

    printf("%d", routing(start, map, size, body));
}
#endif

int routing(coord start, char map[MAX][MAX], coord size, queuet body)
{
    queuet queue = {0, 0};
    queue.elems[queue.rear++] = start;
    queue.rear = queue.rear % QMAX;

    int count = 0;

    coord directions[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

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
                    if (map[tmp.x][tmp.y] != BLOCK || (tmp.x == body.elems[body.front].x && tmp.y == body.elems[body.front].y && (body.rear + QMAX - body.front) % QMAX != 2))
                    {
                        if (map[tmp.x][tmp.y] == DEST)
                        {
                            return count;
                        }

                        if (map[tmp.x][tmp.y] != SEARCHT)
                        {
                            flag = 1;
                            map[tmp.x][tmp.y] = SEARCHT;
                            queue.elems[queue.rear++] = tmp;
                            queue.rear = queue.rear % QMAX;
                        }
                    }
                }
            }
            if (flag && body.front != body.rear)
            {
                if (map[body.elems[body.front].x][body.elems[body.front].y] != SEARCHT)
                {
                    map[body.elems[body.front].x][body.elems[body.front].y] = BLANK;
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