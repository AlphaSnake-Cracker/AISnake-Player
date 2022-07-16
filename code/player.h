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
#include <bases.h>
// #include <unistd.h>

queuet body = {0, 0}; // not circular queue
void init(struct Player *player)
{
	coord point = {-1, -1};

	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			if (player->mat[i][j] == PLAYER_A)
			{
				body.elems[body.rear++] = (point.x = i, point.y = j, point);
			}
		}
	}
}

// int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
struct Point walk(struct Player *player)
{
	coord point = {-1, -1};

	// get foods
	block foods = {0};
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
		{
			if (player->mat[i][j] == FOOD || player->mat[i][j] == SHIELD)
			{
				foods.elems[foods.len++] = (point.x = i, point.y = j, point);
			}
		}
	}

	// return initPoint(, );
}
