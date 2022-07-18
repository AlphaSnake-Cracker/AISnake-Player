/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>

//================================================
#include <limits.h>
#include <math.h>

#define DEBUG
#define ROUTE_DEBUG

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

typedef struct _arrayt
{
	int len;
	int elems[MAX_LEN];
} arrayt;

// anticlockwise
coord surround[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
coord directions[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

coord add(coord A, coord B);
int in_map(coord point, coord size);
int overlap(coord point, queuet body, int len, int qmax);
int evaluate(int distence, coord size);
int get_food_value(coord point, arrayt dists);
int get_dist(coord dest, coord start, char **map, coord size, queuet body);

queuet body = {0, 0}; // not circular queue
int max_len = -1, current_len = -1;
coord head = {-1, -1};
int last_direction = -1;

void init(struct Player *player)
{
	max_len = (int)ceil((player->row_cnt + player->col_cnt) * (3.0 / 8.0));
	current_len = 1;

	coord point = {-1, -1};

	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			if (player->mat[i][j] == PLAYER_A)
			{
				point.x = i;
				point.y = j;
				body.elems[body.rear++] = point;
				head = point;
			}
		}
	}
}

// int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
struct Point walk(struct Player *player)
{
#ifdef DEBUG
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			printf("%c", player->mat[i][j]);
		}
		puts("");
	}
	puts("");

	printf("head: (%d,%d)\n", head.x, head.y);
	printf("max_len: %d\n", max_len);
	printf("current_len: %d\n", current_len);
	printf("last_direction: %d\n", last_direction);

	puts("Body:");
	int count = 0;
	for (int i = body.front; count < (body.rear + QMAX - body.front) % QMAX; count++)
	{
		printf("(%d,%d)", body.elems[i].x, body.elems[i].y);
		i = (i + 1) % QMAX;
	}
	puts("");
#endif

	coord tmp = {-1, -1};

	// get foods
	block foods = {0};
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			if (player->mat[i][j] == FOOD || player->mat[i][j] == SHIELD)
			{
				foods.elems[foods.len++] = (tmp.x = i, tmp.y = j, tmp);
			}
		}
	}

#ifdef DEBUG
	puts("Foods:");
	for (int i = 0; i < foods.len; i++)
	{
		printf("(%d,%d)", foods.elems[i].x, foods.elems[i].y);
	}
	puts("");
#endif

	int max_value = INT_MIN;
	int max_ptr = -1;
	coord size = {player->row_cnt, player->col_cnt};
	for (int i = 0; i < 4; i++)
	{
		if (i != (last_direction + 2) % 4)
		{
			tmp = add(head, directions[i]);
			arrayt dists = {0};
			for (int i = 0; i < foods.len; i++)
			{
				dists.elems[dists.len++] = get_dist(foods.elems[i], tmp, player->mat, size, body);
#ifdef ROUTE_DEBUG
				printf("from(%d,%d),to(%d,%d)is: %d\n", tmp.x, tmp.y, foods.elems[i].x, foods.elems[i].y, dists.elems[dists.len - 1]);
#endif
			}

			int value = get_food_value(tmp, dists);
#ifdef ROUTE_DEBUG
			printf("(%d,%d): %d\n", directions[i].x, directions[i].y, value);
#endif
			if (max_value <= value)
			{
				max_value = value;
				max_ptr = i;
			}
		}
	}
#ifdef DEBUG
	printf("dir: (%d,%d)\n", directions[max_ptr].x, directions[max_ptr].y);
#endif

	head = add(head, directions[max_ptr]);
	last_direction = max_ptr;

	if (player->mat[head.x][head.y] == FOOD || player->mat[head.x][head.y] == SHIELD)
	{
		max_len++;
	}

	body.elems[body.rear++] = head;
	body.rear = body.rear % QMAX;
	if (current_len < max_len)
	{
		current_len++;
	}
	else
	{
		body.front = (body.front + 1) % QMAX;
	}

	return initPoint(head.x, head.y);
}

coord add(coord A, coord B)
{
	coord result = {A.x + B.x, A.y + B.y};
	return result;
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
int overlap(coord point, queuet body, int len, int qmax)
{
	int count = 0;
	for (int i = body.rear - 1; count < len; count++)
	{
		if (point.x == body.elems[i].x && point.y == body.elems[i].y)
		{
			return 1;
		}

		i--;
		if (i == -1)
		{
			i = qmax - 1;
		}
	}
	return 0;
}

// must called before getting wall value
int get_food_value(coord point, arrayt dists)
{
	int len;
	int value = -1;
	int value_tmp;
	coord size = {ROW_MAX, COL_MAX};

	for (int index = 0; index < dists.len; index++)
	{
		len = dists.elems[index];

		value_tmp = evaluate(len, size);

		if (value != INT_MAX)
		{
			if (value_tmp != INT_MAX)
			{
				value += value_tmp;
			}
			else
			{
				value = INT_MAX;
			}
		}
	}
	return value;
}

int evaluate(int distence, coord size)
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

int get_dist(coord dest, coord start, char **map, coord size, queuet body)
{
	if (in_map(start, size))
	{
		if (map[start.x][start.y] != WALL &&
			!overlap(start, body, (body.rear - body.front + QMAX) % QMAX - 1, QMAX) &&
			!(start.x == body.elems[body.front].x &&
			  start.y == body.elems[body.front].y &&
			  (body.rear + QMAX - body.front) % QMAX == 2))
		{
			if (start.x == dest.x && start.y == dest.y)
			{
				return 0;
			}
			else
			{
				body.front++;
			}
		}
		else
		{
			return INT_MAX;
		}
	}
	else
	{
		return INT_MAX;
	}

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

		int flag = 0;
		while (queue.front != rear_tmp)
		{
			current = queue.elems[queue.front++];
			queue.front = queue.front % QMAX;

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
		if (flag == 1)
		{
			body.front++; //======================================================================
		}
	}
	return INT_MAX;
}
