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
// #define PRT_OPPONENT
// #define KEEP_OPPONENT

#define SHRINK_ALERT (10)

#define ROW_MAX (20 + 5)
#define COL_MAX (20 + 5)

// to modify:
#define MAX_LEN (ROW_MAX + COL_MAX)

#define WALL '#'
#define BLANK '.'

#define FOOD 'o'
#define SHIELD 'O'

// #define PLAYER_A '1'
// #define PLAYER_B '2'
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

typedef struct _snake_info
{
	queuet body;
	int grow;
} snake_info;

coord add(coord A, coord B);
int in_map(coord point, rect size);
int overlap(coord point, queuet body, int len, int qmax); // double evaluate(int distence);
double get_value_by_food(arrayt dists, block foods, int round_to_shrink, rect size);
// int get_dist(coord dest, coord start, char **map, rect size, queuet body, int grow, int round_to_shrink);
int get_dist(coord dest, coord start, char **map, rect size, queuet self_body, int grow, snake_info opponent, int round_to_shrink);
int is_dangerous(coord point, rect size);
int int_pow(int base, int exponent);
void virtual_food_generator(block *foods, rect size, char **mat);
coord subtract(coord A, coord B);
void update_opponent_info(struct Player *player);

#ifdef DEBUG
// void print_queue(queuet queue, const char *s);
#endif

// tables:
coord surround[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}}; // anticlockwise
coord directions[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
// coord directions + 1 [4] = {{2, 1}, {1, 2}, {0, 1}, {1, 0}};
int was_food[4] = {0, 0, 0, 0}; // record food around opponent head
int shrink_value_table[SHRINK_ALERT] = {-1, 6, 4, 4, 2, 2, 1, 1, 1, 1};
rect size = {-1, -1, -1, -1}; // [left, right), [up, down)
int inverse_mapping[3][3] = {
	{9, 2, 9},
	{3, 9, 1},
	{9, 0, 9}}; // a inverse mapping of directions[4]

// record self
char PLAYER_1 = -1;
queuet body = {0, 0};				// record snake body
int max_len = -1, current_len = -1; // record current length and max length
coord head = {-1, -1};				// record snake head
int last_direction = -1;			// record the last direciton choosed

// record opponent
char PLAYER_2 = -1;
queuet opponent_body = {0, 0};
int opponent_max = -1;
int opponent_current = -1;
coord opponent_head = {-1, -1};

void init(struct Player *player)
{
#ifdef PRT_MAP
	puts("================================================================================================================");
#endif

	int m = player->row_cnt;
	int n = player->col_cnt;

	size.down = m;
	size.right = n;
	size.up = size.left = 0;

	max_len = (int)ceil((m + n) * (3.0 / 8.0));
	opponent_max = max_len;
	// shrink_interval = (int)ceil(m * n * 4 / (n < m ? n : m));

	current_len = 1;
	opponent_current = 0;

	head.x = player->your_posx;
	head.y = player->your_posy;

	body.elems[body.rear] = head;
	body.rear = (body.rear + 1) % QMAX;
	PLAYER_1 = player->mat[head.x][head.y];

	opponent_head.x = player->opponent_posx;
	opponent_head.y = player->opponent_posy;
	PLAYER_2 = player->mat[opponent_head.x][opponent_head.y];

	coord tmp = {-1, -1};
	for (int i = 0; i < 4; i++)
	{
		tmp = add(head, directions[i]);
		if (in_map(tmp, size))
		{
			if (player->mat[tmp.x][tmp.y] == FOOD || player->mat[tmp.x][tmp.y] == SHIELD)
			{
				was_food[i] = 1;
				continue;
			}
		}
		was_food[i] = 0;
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
	puts("-MAP--END-");

	printf("head: (%d,%d)\n", head.x, head.y);
	printf("tail: (%d,%d)\n", body.elems[body.front].x, body.elems[body.front].y);
#endif

#ifdef DEBUG
	printf("round_to_shrink: %d\n", player->round_to_shrink);
	// printf("last_direction: %d\n", last_direction);
#endif

	update_opponent_info(player);
#ifdef PRT_OPPONENT
	printf("max_len: %d\n", max_len);
	printf("current_len: %d\n", current_len);
	puts("body:");
	int count = 0;
	for (int i = body.front; count < (body.rear + QMAX - body.front) % QMAX; count++)
	{
		printf("(%d,%d)", body.elems[i].x, body.elems[i].y);
		i = (i + 1) % QMAX;
	}
	puts("");

	printf("opponent_head: (%d,%d)\n", opponent_head.x, opponent_head.y);
	printf("opponent_current: %d\n", opponent_current);
	printf("opponent_max: %d\n", opponent_max);
	puts("opponent_body:");
	// int count = 0;
	count = 0;
	for (int i = opponent_body.front; count < (opponent_body.rear + QMAX - opponent_body.front) % QMAX; count++)
	{
		printf("(%d,%d)", opponent_body.elems[i].x, opponent_body.elems[i].y);
		i = (i + 1) % QMAX;
	}
	puts("");
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
	if (foods.len <= 2)
	{
#ifdef DEBUG
		// printf("Entered, foods.len: %d\n", foods.len);
#endif
		virtual_food_generator(&foods, size, player->mat);
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
				snake_info opponent = {{0, 0}, 0};
				opponent.grow = opponent_max - opponent_current;
				opponent.body = opponent_body;
				dists.elems[dists.len++] = get_dist(foods.elems[i], tmp, player->mat, size, body, max_len - current_len, opponent, player->round_to_shrink);
#ifdef ROUTE_DEBUG
				printf("from(%d,%d),to(%d,%d)is: %d\n", tmp.x, tmp.y, foods.elems[i].x, foods.elems[i].y, dists.elems[dists.len - 1]);
#endif
			}

			double value = -1;
			value = get_value_by_food(dists, foods, player->round_to_shrink, size);

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
	// handle some "trival" ends
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
		size.down--;
		size.up++;
		size.right--;
		size.left++;
	}

#ifdef PRT_MAP
	puts("");
#endif
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

double get_value_by_food(arrayt dists, block foods, int round_to_shrink, rect size)
{
	int dist = -1;
	int inf_count = 0;
	double value = 0;
	double single_value = 0;

	for (int index = 0; index < dists.len; index++)
	{
		dist = dists.elems[index];

		if (dist != INT_MAX && dist != -1)
		{
			dist++;
		}
		else
		{
			inf_count++;
		}

		assert(dist != 0);

		single_value = 1.0 / dist;

		//==
		if (round_to_shrink < SHRINK_ALERT && is_dangerous(foods.elems[index], size))
		{
#ifdef ROUTE_DEBUG
			// printf("Entered\n");
#endif
			single_value = single_value / int_pow(10, shrink_value_table[round_to_shrink]);
		}
		//==

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

// ret INT_MAX if: 1. not valid, 2. unreachable
int get_dist(coord dest, coord start, char **map, rect size, queuet self_body, int grow, snake_info opponent, int round_to_shrink)
{
	typedef struct _spl_queue
	{
		int front, rear;
		coord elems[QMAX];
		int fronts[QMAX];
		int grows[QMAX];
	} spl_queue;
	spl_queue queue = {0, 0};

	queue.grows[queue.rear] = grow;

	int searched[ROW_MAX][COL_MAX] = {0};

	//====================
	// somehow redundant

	if (round_to_shrink == 1)
	{
		if (is_dangerous(dest, size))
		{
			return INT_MAX - 1;
		}

		size.down--;
		size.up++;
		size.right--;
		size.left++;
	}
	round_to_shrink--;

	if (in_map(start, size))
	{
		if (map[start.x][start.y] != WALL &&
			!overlap(start, self_body, (self_body.rear - self_body.front + QMAX) % QMAX - 1, QMAX) &&
			!overlap(start, opponent.body, (opponent.body.rear - opponent.body.front + QMAX) % QMAX, QMAX) &&
			!(start.x == self_body.elems[self_body.front].x &&
			  start.y == self_body.elems[self_body.front].y &&
			  (self_body.rear + QMAX - self_body.front) % QMAX == 2))
		{
			if (start.x == dest.x && start.y == dest.y)
			{
				return 0;
			}
			else
			{
				if (map[start.x][start.y] == FOOD || map[start.x][start.y] == SHIELD)
				{
					queue.grows[queue.rear]++;
				}

				if (grow == 0)
				{
					if (self_body.rear != self_body.front)
					{
						self_body.front = (self_body.front + 1) % QMAX;
					}
				}
				else
				{
					grow--;
				}

				if (opponent.grow == 0)
				{
					if (opponent.body.rear != opponent.body.front)
					{
						opponent.body.front = (opponent.body.front + 1) % QMAX;
					}
				}
				else
				{
					opponent.grow--;
				}
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	//====================
	queue.fronts[queue.rear] = self_body.front;
	queue.elems[queue.rear] = start;
	queue.rear = (queue.rear + 1) % QMAX;

	int count = 0;
	searched[start.x][start.y] = 1;

	coord current, tmp;

	int rear_tmp;
	while (queue.front != queue.rear)
	{
		if (round_to_shrink == 1)
		{
			if (is_dangerous(dest, size))
			{
				return INT_MAX - 1;
			}
			size.down--;
			size.up++;
			size.right--;
			size.left++;
		}
		round_to_shrink--;

		rear_tmp = queue.rear;
		count++;

#ifdef DEBUG
		// print_queue(queue, "search queue");
#endif

		while (queue.front != rear_tmp)
		{
			int front_tmp = queue.front;

			current = queue.elems[queue.front++];
			queue.front = queue.front % QMAX;

			for (int i = 0; i < 4; i++)
			{
				tmp = add(current, directions[i]);
				if (in_map(tmp, size))
				{
					if (map[tmp.x][tmp.y] != WALL &&
						!overlap(tmp, self_body, (self_body.rear - queue.fronts[front_tmp] + QMAX) % QMAX - 1, QMAX) &&
						!overlap(tmp, opponent.body, (opponent.body.rear - opponent.body.front + QMAX) % QMAX, QMAX) &&
						!(tmp.x == self_body.elems[queue.fronts[front_tmp]].x &&
						  tmp.y == self_body.elems[queue.fronts[front_tmp]].y &&
						  (self_body.rear + QMAX - queue.fronts[front_tmp]) % QMAX == 2))
					{
						if (searched[tmp.x][tmp.y] == 0)
						{
							if (map[tmp.x][tmp.y] == FOOD || map[tmp.x][tmp.y] == SHIELD)
							{
								queue.grows[front_tmp]++;
							}

							if (tmp.x == dest.x && tmp.y == dest.y)
							{
								return count;
							}

							if (queue.grows[front_tmp] == 0)
							{
								if (self_body.rear != queue.fronts[front_tmp])
								{
									queue.fronts[queue.rear] = (queue.fronts[front_tmp] + 1) % QMAX;
								}
								else
								{
									queue.fronts[queue.rear] = queue.fronts[front_tmp];
								}
								queue.grows[queue.rear] = queue.grows[front_tmp];
							}
							else
							{
								queue.fronts[queue.rear] = queue.fronts[front_tmp];
								queue.grows[queue.rear] = queue.grows[front_tmp] - 1;
							}

							searched[tmp.x][tmp.y] = 1;
							queue.elems[queue.rear] = tmp;
							queue.rear = (queue.rear + 1) % QMAX;
						}
					}
				}
			}
		}
		if (opponent.grow == 0)
		{
			if (opponent.body.rear != opponent.body.front)
			{
				opponent.body.front = (opponent.body.front + 1) % QMAX;
			}
		}
		else
		{
			opponent.grow--;
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

void virtual_food_generator(block *foods, rect size, char **mat)
{
	coord virtual_foods[4] =
		{{size.up + 1, size.left + 1},
		 {size.up + 1, size.right - 2},
		 {size.down - 2, size.left + 1},
		 {size.down - 2, size.right - 2}};

	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (mat[virtual_foods[i].x][virtual_foods[i].y] == '.')
		{
			foods->elems[foods->len++] = virtual_foods[i];
			count++;
		}
	}

	if (size.right - size.left <= 4 || count <= 1)
	{
		coord center[4] = {{4, 5}, {5, 4}, {4, 4}, {5, 5}};
		for (int i = 0; i < 4; i++)
		{
			foods->elems[foods->len++] = center[i];
		}
	}

	return;
}

coord subtract(coord A, coord B)
{
	coord point = {A.x - B.x, A.y - B.y};
	return point;
}

void update_opponent_info(struct Player *player)
{
	if (player->opponent_status == -1)
	{
		opponent_body.front = opponent_body.rear = 0;
#ifdef KEEP_OPPONENT
		assert(0);
#endif
		return;
	}

	coord tmp = {player->opponent_posx, player->opponent_posy};
	coord direction = {-1, -1};
	direction = subtract(tmp, opponent_head);

#ifdef PRT_OPPONENT
	puts("was food:");
	for (int i = 0; i < 4; i++)
	{
		printf("%d ", was_food[i]);
	}
	puts("");
#endif

	if (0 <= direction.x + 1 && direction.x + 1 < 3 && 0 <= direction.y + 1 && direction.y + 1 < 3)
	{
		if (was_food[inverse_mapping[direction.x + 1][direction.y + 1]] == 1)
		{
			opponent_max++;
		}
	}

	if (opponent_current < opponent_max)
	{
		opponent_current++;
	}
	else
	{
		opponent_body.front = (opponent_body.front + 1) % QMAX;
	}

	opponent_head.x = player->opponent_posx;
	opponent_head.y = player->opponent_posy;
	opponent_body.elems[opponent_body.rear] = opponent_head;
	opponent_body.rear = (opponent_body.rear + 1) % QMAX;

	for (int i = 0; i < 4; i++)
	{
		tmp = add(opponent_head, directions[i]);
		if (in_map(tmp, size))
		{
			if (player->mat[tmp.x][tmp.y] == FOOD || player->mat[tmp.x][tmp.y] == SHIELD)
			{
				was_food[i] = 1;
				continue;
			}
		}
		was_food[i] = 0;
	}
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