/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include <stdlib.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <math.h>

#define DEBUG

typedef struct Queue_Element
{
	// Custom Data Structure
	int x;
	int y;
	int step;
}
Elem;

typedef struct Path_Info
{
	int prev_x;
	int prev_y;
}
Path;

struct Snake_Info
{
	int length;
	int max_length;
	Elem body[400];
}
snake;

int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};

int Is_Legal(struct Player *player, int visit[20][20], int nx, int ny, int ds)
{
	if (nx <= -1 || nx >= player -> row_cnt)
		return 0;
	if (ny <= -1 || ny >= player -> col_cnt)
		return 0;
	if (player -> mat[nx][ny] == '#' || player -> mat[nx][ny] == '1' || player -> mat[nx][ny] == '2')
		return 0;
	if (visit[nx][ny] == 1)
		return 0;
	int i;
	for(i = 0; i < snake.max_length; i ++)
	{
		if(nx == snake.body[i].x && ny == snake.body[i].y)
		{
			if(ds > snake.max_length - i)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

void BFS_Pathing(struct Player *player, Path path[20][20]) // Pathing with BFS algorithm
{
	int visit[20][20] = {0};
	Elem queue[400];
	int front = 0;
	int rear = -1;
	int dx = player -> your_posx;
	int dy = player -> your_posy;
	int ds = 0;
	rear += 1;
	queue[rear].x = dx;
	queue[rear].y = dy;
	queue[rear].step = ds;
	visit[dx][dy] = 1;
	path[dx][dy].prev_x = -1;
	path[dx][dy].prev_y = -1;
	while (front <= rear)
	{
		dx = queue[front].x;
		dy = queue[front].y;
		ds = queue[front].step;
		front += 1;
		for (int i = 0; i < 4; i += 1)
		{
			int nx = dx + step[i][0];
			int ny = dy + step[i][1];
			int ns = ds + 1;
			if (Is_Legal(player, visit, nx, ny, ns))
			{
				rear += 1;
				queue[rear].x = nx;
				queue[rear].y = ny;
				queue[rear].step = ns;
				visit[nx][ny] = 1;
				path[nx][ny].prev_x = dx;
				path[nx][ny].prev_y = dy;
			}
		}
	}
}

void Weighting(struct Player *player, Path path[20][20], int weight[20][20])
{
	for (int i = 0; i < player -> row_cnt; i += 1)
	{
		for (int j = 0; j < player -> col_cnt; j += 1)
		{
			if (player -> mat[i][j] == 'o')
			{
				int x = i;
				int y = j;
				if (path[x][y].prev_x == 0 && path[x][y].prev_y == 0)
					continue;
				while (path[x][y].prev_x != -1 && path[x][y].prev_y != -1)
				{
					weight[x][y] += 1;
					int temp_x = path[x][y].prev_x;
					int temp_y = path[x][y].prev_y;
					x = temp_x;
					y = temp_y;
				}
			}
			else if (player -> mat[i][j] == 'O')
			{
				int x = i;
				int y = j;
				if (path[x][y].prev_x == 0 && path[x][y].prev_y == 0)
					continue;
				while (path[x][y].prev_x != -1 && path[x][y].prev_y != -1)
				{
					weight[x][y] += 1;
					int temp_x = path[x][y].prev_x;
					int temp_y = path[x][y].prev_y;
					x = temp_x;
					y = temp_y;
				}
			}
		}
	}
}

void init(struct Player *player)
{
	// This function will be executed at the begin of each game, only once.
	snake.length = 1;
	snake.max_length = ceil(3.0 * (player -> col_cnt + player -> row_cnt) / 8);
	snake.body[0].x = player -> your_posx;
	snake.body[0].y = player -> your_posy;
	#ifdef DEBUG
	printf("%d %d %d\n", snake.max_length, snake.body[0].x, snake.body[0].y);
	#endif
}

int Cblock(struct Player * player, int nx, int ny)              //this function will be used to find the current connected blocks contain the blocks number
{
	int l, r;
	int sum = 0;
	int visit[20][20] = {0};
	Elem q[400];
	l = 0;
	r = 0;
	int ds = 0;
	Elem head;
	head.x = nx;
	head.y = ny;
	head.step = ds;
	q[r ++] = head;
	visit[head.x][head.y] = 1;
	while(l < r)
	{
		Elem now = q[l++];
		for (int i = 0; i < 4; i++) 
		{
            int tx = now.x + step[i][0];
            int ty = now.y + step[i][1];
			int ns = ds + 1;
			if(Is_Legal(player, visit, tx, ty, ns))
			{
				visit[tx][ty] = 1;
				Elem t;
				t.x = tx;
				t.y = ty;
				q[r ++] = t;
				sum ++;
			}
		}
	}
	return sum;
}

void Snake_Move(struct Player *player, int x, int y)
{
	if (player -> mat[x][y] == 'o' || player -> mat[x][y] == 'O')
		snake.max_length += 1;
	for (int i = snake.length - 1; i >= 0; i -= 1)
		snake.body[i + 1] = snake.body[i];
	snake.body[0].x = x;
	snake.body[0].y = y;
	if (snake.length < snake.max_length)
		snake.length += 1;
}

struct Point walk(struct Player *player)
{
	#ifdef DEBUG
	for (int i = 0; i < player->row_cnt; i++)
	{
		for (int j = 0; j < player->col_cnt; j++)
			printf("%c", player->mat[i][j]);
		printf("\n");
	}
	for (int i = 0; i < snake.length; i += 1)
		printf("%d %d\n", snake.body[i].x, snake.body[i].y);
	#endif
	// This function will be executed in each round.
	Path path[20][20];
	BFS_Pathing(player, path);
	int weight[20][20] = {0};
	Weighting(player, path, weight);
	int nx, ny;
	int max_w = 0;
	int x = player -> your_posx;
	int y = player -> your_posy;
	for (int i = 0; i < 4; i += 1)
	{
		nx = player -> your_posx + step[i][0];
		ny = player -> your_posy + step[i][1];
		if (nx <= -1 || nx >= player -> row_cnt || ny <= -1 || ny >= player -> col_cnt || player -> mat[nx][ny] == '#' || player -> mat[nx][ny] == '1' || player -> mat[nx][ny] == '2' || Cblock(player, nx, ny) < snake.length)
			continue;
		if (max_w < weight[nx][ny])
		{
			max_w = weight[nx][ny];
			x = nx;
			y = ny;
		}
		#ifdef DEBUG
		printf("%d\n", Cblock(player, nx, ny));
		#endif
	}
	while (x == player -> your_posx && y == player -> your_posy)
	{
		int r = rand();
		nx = player -> your_posx + step[r % 4][0];
		ny = player -> your_posy + step[r % 4][1];
		if (nx <= -1 || nx >= player -> row_cnt || ny <= -1 || ny >= player -> col_cnt || player -> mat[nx][ny] == '#' || player -> mat[nx][ny] == '1' || player -> mat[nx][ny] == '2')
			continue;
		x = nx;
		y = ny;
	}
	Snake_Move(player, x, y);
	return initPoint(x, y);
}
