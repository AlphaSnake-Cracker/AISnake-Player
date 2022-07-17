/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>

typedef struct Queue_Element
{
	//Custom Data Structure
	int x;
	int y;
	int step;
}
Elem;

void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
}

int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
struct Point walk(struct Player *player) {
	for (int i = 0; i < player->row_cnt; i++) {
		for (int j = 0; j < player->col_cnt; j++) {
			printf("%c", player->mat[i][j]);
		}
		printf("\n");
	}
	// This function will be executed in each round.
	int visit[20][20];
	int dx = player -> your_posx;
	int dy = player -> your_posy;
	int ds = 0;
	Elem queue[400];
	int front = 0;
	int rear = -1;
	rear += 1;
	queue[rear].x = dx;
	queue[rear].y = dy;
	queue[rear].step = ds;
	visit[dx][dy] = 1;
	while (front <= rear)
	{
		dx = queue[front].x;
		dy = queue[front].y;
		ds = queue[front].step;
		front += 1;
		if (player -> mat[dx][dy] == 'o' || player -> mat[dx][dy] == 'O')
		{
			printf("%d %d %d\n", dx, dy, ds);
			return initPoint(dx, dy);
		}
		for (int i = 0; i < 4; i += 1)
		{
			int nx = dx + step[i][0];
			int ny = dy + step[i][1];
			int ns = ds + 1;
			if (nx > -1 && nx < player -> row_cnt && ny > -1 && ny < player -> col_cnt && player -> mat[nx][ny] != '#' && player -> mat[nx][ny] != '1' && player -> mat[nx][ny] != '2')
			{
				rear += 1;
				queue[rear].x = nx;
				queue[rear].y = ny;
				queue[rear].step = ns;
				visit[nx][ny] = 1;
				//printf("%d %d %d\n", nx, ny, ns);
			}
		}
	}
	return initPoint(player->your_posx, player->your_posy);
}
