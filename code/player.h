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
#include <assert.h>

#define DEBUG
#define ROUTE_DEBUG
#define PRT_MAP

#define SHRINK_ALERT (8)

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

typedef struct _rect
{
	int left, right, up, down;
} rect;

coord add(coord A, coord B);
int in_map(coord point, rect size);
int overlap(coord point, queuet body, int len, int qmax);
// double evaluate(int distence);
double get_value_by_food(coord point, arrayt dists, block foods, int round_to_shrink, rect size);
int get_dist(coord dest, coord start, char **map, rect size, queuet body, int grow);
int is_dangerous(coord point, rect size);
int int_pow(int base, int exponent);
#ifdef DEBUG
// void print_queue(queuet queue, const char *s);
#endif

// anticlockwise
coord surround[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
coord directions[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
int shrink_value_table[8] = {-1, 6, 4, 4, 3, 3, 2, 2};
// coord size = {-1, -1};
rect size = {-1, -1, -1, -1}; // [left, right),[up, down)

queuet body = {0, 0};				// record snake body
int max_len = -1, current_len = -1; // record current length and max length
coord head = {-1, -1};				// record snake head
int last_direction = -1;			// record the last direciton choosed
int shrink_index = -100;			// record how many times map has shrinked(start from 0)
// int shrink_interval = -1;

void init(struct Player *player)
{
#ifdef PRT_MAP
	puts("================================================================================================================");
#endif

	int m = player->row_cnt;
	int n = player->col_cnt;
	// size.x = m;
	// size.y = n;
	size.down = m;
	size.right = n;
	size.up = size.left = 0;

	max_len = (int)ceil((m + n) * (3.0 / 8.0));
	// shrink_interval = (int)ceil(m * n * 4 / (n < m ? n : m));
	shrink_index = 0;

	current_len = 1;

	// coord point = {-1, -1};

	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			if (player->mat[i][j] == PLAYER_A)
			{
				head.x = i;
				head.y = j;
				body.elems[body.rear++] = head;
			}
		}
	}
}

struct Point walk(struct Player *player)
{
#ifdef PRT_MAP
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			printf("%c", player->mat[i][j]);
		}
		puts("");
	}
	puts("");
#endif

#ifdef DEBUG
	printf("head: (%d,%d)\n", head.x, head.y);
	printf("tail: (%d,%d)\n", body.elems[body.front].x, body.elems[body.front].y);
	printf("round_to_shrink: %d\n", player->round_to_shrink);
	// printf("max_len: %d\n", max_len);
	// printf("current_len: %d\n", current_len);
	// printf("last_direction: %d\n", last_direction);

	// puts("Body:");
	// int count = 0;
	// for (int i = body.front; count < (body.rear + QMAX - body.front) % QMAX; count++)
	// {
	// 	printf("(%d,%d)", body.elems[i].x, body.elems[i].y);
	// 	i = (i + 1) % QMAX;
	// }
	// puts("");
#endif

	coord tmp = {-1, -1};
	// ========================================================================
	// scan the whole map to get foods position
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
	// puts("Foods:");
	// for (int i = 0; i < foods.len; i++)
	// {
	// 	printf("(%d,%d)", foods.elems[i].x, foods.elems[i].y);
	// }
	// puts("");
#endif

	// ========================================================================
	// making decision
	double max_value = INT_MIN;
	coord next = {-1, -1};
	int max_ptr = -1;

	for (int i = 0; i < 4; i++)
	{
		if (i != (last_direction + 2) % 4)
		{
			tmp = add(head, directions[i]);
			arrayt dists = {0};
			for (int i = 0; i < foods.len; i++)
			{
				dists.elems[dists.len++] = get_dist(foods.elems[i], tmp, player->mat, size, body, max_len - current_len);
#ifdef ROUTE_DEBUG
				printf("from(%d,%d),to(%d,%d)is: %d\n", tmp.x, tmp.y, foods.elems[i].x, foods.elems[i].y, dists.elems[dists.len - 1]);
#endif
			}

			double value = -1;
			value = get_value_by_food(tmp, dists, foods, player->round_to_shrink, size);

#ifdef ROUTE_DEBUG
			printf("(%d,%d): %.20lf\n", tmp.x, tmp.y, value);
			puts("");
#endif
			if (max_value <= value)
			{
				max_value = value;
				max_ptr = i;
				next = tmp;
			}
		}
	}
#ifdef DEBUG
	// printf("next: (%d,%d)\n", next.x, next.y);
#endif

	// ========================================================================
	// hand with some "trival" ends
	head = next;
	last_direction = max_ptr;

	if (player->mat[head.x][head.y] == FOOD || player->mat[head.x][head.y] == SHIELD)
	{
		max_len++;
	}

	body.elems[body.rear] = head;
	body.rear = (body.rear + 1) % QMAX;

	if (current_len < max_len)
	{
		current_len++;
	}
	else
	{
		body.front = (body.front + 1) % QMAX;
	}

	if (player->round_to_shrink == 1)
	{
		// shrink_index++;
		// size.x--;
		// size.y--;
		size.down--;
		size.up++;
		size.right--;
		size.left++;
	}

	return initPoint(head.x, head.y);
}

coord add(coord A, coord B)
{
	coord result = {A.x + B.x, A.y + B.y};
	return result;
}

int in_map(coord point, rect size)
{
	if (point.x < size.up || point.x >= size.down)
	{
		return 0;
	}
	if (point.y < size.left || point.y >= size.right)
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
// return -1 if any dist is INT_MAX
double get_value_by_food(coord point, arrayt dists, block foods, int round_to_shrink, rect size)
{
	int dist = -1;
	int inf_count = 0;
	double value = 0;
	double single_value = 0;

	for (int index = 0; index < dists.len; index++)
	{
		dist = dists.elems[index];
		if (dist != INT_MAX)
		{
			dist++;
		}
		else
		{
			inf_count++;
		}

		assert(dist > 0);

		single_value = 1.0 / dist;
		//
		if (round_to_shrink < SHRINK_ALERT && is_dangerous(foods.elems[index], size))
		{
#ifdef ROUTE_DEBUG
			printf("Entered\n");
#endif
			single_value = single_value / int_pow(10, shrink_value_table[round_to_shrink]);
		}
		//
		value += single_value;
	}
#ifdef DEBUG
	// printf("value before return: %lf\n", value);
#endif
	return value / int_pow(10, inf_count);
}

int int_pow(int base, int exponent)
{
	int result = 1;
	assert(exponent >= 0);
	for (int i = 0; i < exponent; i++)
	{
		result *= base;
	}
	return result;
}
// ret INT_MAX if: 1. not valid 2. unreachable
int get_dist(coord dest, coord start, char **map, rect size, queuet body, int grow)
{
	if (in_map(start, size))
	{
		if (map[start.x][start.y] != WALL &&
			map[start.x][start.y] != PLAYER_B &&
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

#ifdef DEBUG
		// print_queue(queue, "search queue");
#endif

		// int flag = 0;
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
						map[tmp.x][tmp.y] != PLAYER_B &&
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
							// flag = 1;
							searched[tmp.x][tmp.y] = 1;
							queue.elems[queue.rear++] = tmp;
							queue.rear = queue.rear % QMAX;
						}
					}
				}
			}
		}
		if (grow == 0)
		{
			if (body.rear != body.front)
			{
				body.front = (body.front + 1) % QMAX;
			}
		}
		else
		{
			grow--;
		}
	}
	return INT_MAX;
}

// modify?
int is_dangerous(coord point, rect size)
{
	if ((point.x == size.up ||
		 point.x == size.down - 1) &&
		size.left <= point.y &&
		point.y <= size.right - 1)
	{

		return 1;
	}
	if ((point.y == size.left ||
		 point.y == size.right - 1) &&
		size.up <= point.x &&
		point.x <= size.down - 1)
	{

		return 1;
	}
	return 0;
}

#ifdef DEBUG
// void print_queue(queuet queue, const char *s)
// {
// 	int count = 0;
// 	int l = (queue.rear + QMAX - queue.front) % QMAX;

// 	printf("%s: ", s);
// 	for (int i = queue.front; count < l; count++)
// 	{
// 		printf("(%d,%d)", queue.elems[i].x, queue.elems[i].y);
// 		i = (i + 1) % QMAX;
// 	}
// 	puts("");
// }
#endif