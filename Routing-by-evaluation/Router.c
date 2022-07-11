#define DEBUG

#include "../includes/head.h"
#include <string.h>

#ifdef DEBUG
#define VALUE_ROW 7
#define VALUE_COL 15
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
int get_value(coord point, block foods, block wall, coord size);
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

    coord current = start;

    // !=================================================!
    coord last = add(current, directions[Left]);

    coord tmp = {0, 0};

// #define TRYING
#ifdef TRYING
    int times = 0;
    while (times <= 7 * 15)
#else
    // while dest not reached
    while (!included_in(current, foods))
#endif
    {
        int max = INT_MIN;
        int ptr = -1;
        int value_got;
        for (int i = 0; i < 4; i++)
        {
            tmp = add(current, directions[i]);
            if (in_map(tmp, size))
            {
                if (!(tmp.x == last.x && tmp.y == last.y))
                {
                    value_got = get_food_value(current, foods);
                    if (max < value_got)
                    {
                        max = value_got;
                        ptr = i;
                    }
                }
            }
        }
        last = current;

        assert(ptr != -1);
        current = add(current, directions[ptr]);
        printf("%d %d\n", current.x, current.y);
        int position = included_in(current, foods);
        if (position != -1)
        {
            // foods;
        }
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

int get_value(coord point, block foods, block wall, coord size)
{
    int value = 0;
    value += get_food_value(point, foods);
    return value;
}

// must called before getting wall value
int get_food_value(coord point, block foods)
{
    int len;
    int value;
    int value_tmp;
    coord size = {VALUE_ROW, VALUE_COL};

    for (int index = 0; index < foods.len; index++)
    {
        len = ABSOLUTE(point.x - foods.elems[index].x) + ABSOLUTE(point.y - foods.elems[index].y);

        value_tmp = EVALUATE(len, size);

        if (value != INT_MAX)
        {
            if (value_tmp != INT_MAX)
            {
                value += 3 * value;
            }
            else
            {
                value = INT_MAX;
            }
        }
    }
    return value;
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

// return position of "point" in "any" if included
// return -1 if not included
int included_in(coord point, block any)
{
    for (int i = 0; i < any.len; i++)
    {
        if (point.x == any.elems[i].x && point.y == any.elems[i].y)
        {
            return i;
        }
    }
    return -1;
}

block block_del(block any, int position)
{
}

int get_wall_value()
{
    // for (int index = 0; index < wall.len; index++)
    // {
    //     len = ABSOLUTE(i - wall.elems[index].x) + ABSOLUTE(j - wall.elems[index].y);
    //     value = EVALUATE(len, size);

    //     if (values.elems[i][j] != INT_MIN && values.elems[i][j] != INT_MAX)
    //     {
    //         if (value != INT_MAX)
    //         {
    //             values.elems[i][j] -= value;
    //         }
    //         else
    //         {
    //             values.elems[i][j] = INT_MIN;
    //         }
    //     }
    // }
}
