#define DEBUG

#include "../includes/head.h"

#ifdef DEBUG
#include <assert.h>
#include <stdio.h>
#endif

#define WALL 1
#define BLANK 0
#define SEARCHT_WALL -1

// #define EVALUATE(distence) (MAX_LEN - (distence))
#define ABSOLUTE(num) ((2 * ((num) >= 0) - 1) * (num))

int in_map(coord point, coord size);
int route(coord start, block foods, block wall, coord size);
mapt get_value(block foods, block wall, coord size);
int EVALUATE(int distence, coord size);

#ifdef DEBUG

int main()
{
    freopen("Router.in", "r", stdin);
    freopen("Router.out", "w", stdout);

    coord size;
    scanf("%d %d", &size.x, &size.y);
    // mapt map = {size};
    coord start = {-1, -1};
    block foods = {0};
    block wall = {0};

#define HEAD 6
#define BODY 9
#define WALL 1
#define FOOD 2

    int input_tmp;
    coord tmp_point;
    for (int i = 0; i < size.x; i++)
    {
        for (int j = 0; j < size.y; j++)
        {
            scanf("%d", &input_tmp);

            tmp_point.x = i;
            tmp_point.y = j;
            if (input_tmp == HEAD)
            {
                start = tmp_point;
            }
            if (input_tmp == WALL)
            {
                wall.elems[wall.len++] = tmp_point;
            }
            if (input_tmp == FOOD)
            {
                foods.elems[foods.len++] = tmp_point;
            }
        }
    }

    route(start, foods, wall, size);
}
#endif

int route(coord start, block foods, block wall, coord size)
{
    enum
    {
        Down,
        Right,
        Up,
        Left
    };

    // anticlockwise
    coord directions[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    mapt values = get_value(foods, wall, size);
    coord current = start;

    // !=================================================!
    coord last = add(current, directions[Left]);

    coord tmp = {0, 0};

#define TRYING
#ifdef TRYING
    int times = 0;
    while (times <= 7 * 15)
#else
    // while dest not reached
    while (!(current.x == foods.elems[0].x && current.y == foods.elems[0].y))
#endif
    {
        int max = INT_MIN;
        int ptr = -1;
        for (int i = 0; i < 4; i++)
        {
            tmp = add(current, directions[i]);
            if (in_map(tmp, size))
            {
                if (!(tmp.x == last.x && tmp.y == last.y))
                {
                    if (max < values.elems[tmp.x][tmp.y])
                    {
                        max = values.elems[tmp.x][tmp.y];
                        ptr = i;
                    }
                }
            }
        }
        last = current;

        assert(ptr != -1);
        current = add(current, directions[ptr]);
        printf("%d %d\n", current.x, current.y);
#ifdef TRYING
        times++;
#endif
    }
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

mapt get_value(block foods, block wall, coord size)
{
    mapt values = {size};
    for (int i = 0; i < values.size.x; i++)
    {
        for (int j = 0; j < values.size.y; j++)
        {
            int len = 0;
            int value = 0;
            for (int index = 0; index < foods.len; index++)
            {
                len = ABSOLUTE(i - foods.elems[index].x) + ABSOLUTE(j - foods.elems[index].y);
                value = EVALUATE(len, size);

                if (values.elems[i][j] != INT_MIN && values.elems[i][j] != INT_MAX)
                {
                    if (value != INT_MAX)
                    {
                        // values.elems[i][j] += value;
                        values.elems[i][j] += 10 * value;
                    }
                    else
                    {
                        values.elems[i][j] = INT_MAX;
                    }
                }
            }
            for (int index = 0; index < wall.len; index++)
            {
                len = ABSOLUTE(i - wall.elems[index].x) + ABSOLUTE(j - wall.elems[index].y);
                value = EVALUATE(len, size);

                if (values.elems[i][j] != INT_MIN && values.elems[i][j] != INT_MAX)
                {
                    if (value != INT_MAX)
                    {
                        values.elems[i][j] -= value;
                    }
                    else
                    {
                        values.elems[i][j] = INT_MIN;
                    }
                }
            }
        }
    }
    return values;
}

int EVALUATE(int distence, coord size)
{
    if (distence > 0)
    {
        return size.x + size.y - distence;
    }
    else
    {
        return INT_MAX;
    }
}

int included_in(coord point, block any)
{
    for (int i = 0; i < any.len; i++)
    {
        if (point.x == any.elems[i].x && point.y == any.elems[i].y)
        {
            return 1;
        }
    }
    return 0;
}