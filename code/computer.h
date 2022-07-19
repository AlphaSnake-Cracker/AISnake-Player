#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>

#define DEBUG_MODE false
#define PRINT       \
	if (DEBUG_MODE) \
	printf

int part_num, n, m, part_num_now;
int ds, dx;
int map[13][13];
struct part_info
{
	int size;
	int map[13][13];
	int dx, dy;
};
part_info part[10];

void init(struct Player *player)
{
	PRINT("init start.\n");
	n = player->row_cnt;
	m = player->col_cnt;
	assert(n == m);
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			if (player->mat[i][j] == '#')
				map[i][j] = 9;
			else
				map[i][j] = 0;
		}
	}
	ds = n;
	dx = 0;
	int i, j;
	part_num = 0;
	while (ds > 0)
	{
		part_num++;
		part[part_num].size = ds;
		part[part_num].dx = dx;
		part[part_num].dy = dx;
		for (i = 0; i < ds; i++)
			for (j = 0; j < ds; j++)
				part[part_num].map[i][j] = map[i + dx][j + dx];
		ds -= 2;
		dx += 1;
	}
	PRINT("part_num=%d\n", part_num);
	part_num_now = 1;
}

int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
int step_num[4] = {0, 2, 1, 3};
struct Point walk(struct Player *player)
{
	PRINT("walk start in round %d!\n", player->round);
	// PRINT("player->round_to_shrink=%d part_num_now=%d\n",player->round_to_shrink,part_num_now);
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			PRINT("%c", player->mat[i][j]);
		}
		PRINT("\n");
	}
	int i;
	if (player->round_to_shrink < 10)
	{
		for (i = part[part_num_now].dx; i <= n - 1 - part[part_num_now].dx; i++)
		{
			map[i][part[part_num_now].dx] = 9;
			map[i][n - 1 - part[part_num_now].dx] = 9;
			map[part[part_num_now].dx][i] = 9;
			map[n - 1 - part[part_num_now].dx][i] = 9;
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			PRINT("%d ", map[i][j]);
		PRINT("\n");
	}
	if (player->round_to_shrink == 1)
	{
		part_num_now++;
	}
	for (int j = 0; j < 4; j++)
	{
		i = step_num[j];
		int dx = player->your_posx + step[i][0], dy = player->your_posy + step[i][1];
		if (dx >= 0 && dx < player->row_cnt && dy >= 0 && dy < player->col_cnt && map[dx][dy] != 9 && (player->mat[dx][dy] == 'o' || player->mat[dx][dy] == 'O'))
		{
			return initPoint(dx, dy);
		}
	}
	for (int j = 0; j < 4; j++)
	{
		i = step_num[j];
		int dx = player->your_posx + step[i][0], dy = player->your_posy + step[i][1];
		if (dx >= 0 && dx < player->row_cnt && dy >= 0 && dy < player->col_cnt && map[dx][dy] != 9 && (player->mat[dx][dy] == '.'))
		{
			return initPoint(dx, dy);
		}
	}
	for (int j = 0; j < 4; j++)
	{
		i = step_num[j];
		int dx = player->your_posx + step[i][0], dy = player->your_posy + step[i][1];
		if (dx >= 0 && dx < player->row_cnt && dy >= 0 && dy < player->col_cnt && (player->mat[dx][dy] == 'o' || player->mat[dx][dy] == 'O' || player->mat[dx][dy] == '.'))
		{
			return initPoint(dx, dy);
		}
	}
	return initPoint(player->your_posx, player->your_posy);
}
