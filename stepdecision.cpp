#include"
#include"include/playerbase.h"
#include<string.h>
#include<stdio.h>
#include<math.h>
#define maxsize 50
struct node {
	int x;
	int y;
	int du;
	int distance;
}food[maxsize];


typedef struct Dot {
	int x;
	int y;
}dot;

struct Snakestatus {
	int length;
	int maxlength;
	dot body[40];
}snake;       

int dir[4][2] = { {0,1},{1,0},{0,-1},{-1,0} };  //�� ��  �� ��

int init_food(struct Player* player, struct node food[])
{
	int count = 0;
	int x = player->your_posx;
	int y = player->your_posy;

	for (int i = 0; i < player->row_cnt; i++)
		for (int j = 0; i < player->col_cnt; j++)
		{
			if (player->mat[i][j] == 'o' || player->mat[i][j] == 'O')
			{
				food[count].x = i;
				food[count].y = j;
				int manhadun = fabs(x - i) + fabs(y - j);
				food[count].distance = manhadun;
				count++;
				if (j + 1 < player->col_cnt)
					if (player->mat[i][j + 1] != '#' && player->mat[i][j + 1] != '1' && player->mat[i][j + 1] != '2')
						food[count].du++;
				if (j - 1 >= 0)
					if (player->mat[i][j - 1] != '#' && player->mat[i][j - 1] != '1' && player->mat[i][j - 1] != '2')
						food[count].du++;
				if (i + 1 < player->row_cnt)
					if (player->mat[i + 1][j] != '#' && player->mat[i + 1][j] != '1' && player->mat[i + 1][j] != '2')
						food[count].du++;
				if (i - 1 >= 0)
					if (player->mat[i - 1][j] != '#' && player->mat[i - 1][j] != '1' && player->mat[i - 1][j] != '2')
						food[count].du++;
			}
		}
	return count;
};

int judge_apple(struct Player* player, int* xe, int* ye, Path path[])  //����food���飬�Ƿ���������Ҷȷ���ƻ��
{
	int min = 200000;
	int num = init_food(player, food);
	int near;
	int x = player->your_posx, y = player->your_posy;
	for (int i = 0; i < num; i++)
	{
		if (Astar(x, y, food[i].x, food[i].y, player->mat, path) && food[i].du >= 2 && food[i].distance < min)
		{
			min = food[i].distance;
			near = i;
		}
	}
	if (min != 200000)
	{
		*xe = food[near].x;
		*ye = food[near].y;
		return 1;
	}
	return 0;
}

int judgetail(int xe, int ye,int tailx,int taily, int map[20][20], dot path_1[]) //�Ƿ����ҵ�β�ͣ���ƻ������ʼ����A*������û��·��
{
	if(Astar_Find_road(xe,ye,tailx,taily,map,path_1))
	return 1;
	else
	return 0;
}

void snake_Move(struct Player* player, int x, int y,struct Snakestatus falsesnake,int map[20][20])
{
	if (player->mat[x][y] == 'o' || player->mat[x][y] == 'O')
		falsesnake.length += 1;

	int tailx = falsesnake.body[falsesnake.length - 1].x;    //β�͵�����
	int taily = falsesnake.body[falsesnake.length - 1].y;

	for (int i = falsesnake.length - 1; i >= 0; i -= 1)
		falsesnake.body[i + 1] = falsesnake.body[i];

	falsesnake.body[0].x = x;
	falsesnake.body[0].y = y;

	if (falsesnake.length < falsesnake.max_length)
		falsesnake.length += 1;

	map[x][y] = 1;    //�µ�ͼˢ��һ��
	map[tailx][taily] = '.';
}

dot NewMap(struct Player* player, int map[20][20], Path path[], int k)
{
	for (int i = 0; i < player->row_cnt; i++)   //����ͼ
		for (int j = 0; j<player->col_cnt; j++)
			map[i][j] = player->mat[i][j];

	Snakestatus falaesnake = snake;
	for (int i = 0; i < k; i++)    //�����ƶ�path��
	{
		snake_Move(player, path[i].x, path[i].y, falaesnake, map);   
	}
	return falaesnake.body[falaesnake.length-1];
}


void decision(struct Player* player, int& xe, int& ye, dot path[])       //��ǰͷ��λ��
{
	//ɨ���ͼ����Astar��ʼ����ÿ��Apple�Ͷܵľ��룻
	int x, y;
	x = player->your_posx;
	y = player->your_posy;
	if (judge_apple(player, &xe, &ye, path) == 1)  //������ڶȷ��ϵ������Apple
	{
		int map[20][20];
		dot tail_dot=NewMap(player, map, path, k);
         dot  path_1[50];

		if (judgetail(xe, ye,tail_dot.x,tail_dot.y,path_1) == 1)
		{
			return path[0]; //	astar�����·��path��һ��
		}
		else   //can't find tail
		{
			for(int i=0;i<4;i++)
			{
				int nextx = x+dir[i][0];
				int nexty = y+dir[i][1];
				if (nextx<= -1 || nextx >= player->row_cnt ||nexty <= -1 ||nexty >= player->col_cnt || player->mat[nextx][nexty] == '#' || player->mat[nextx][nexty] == '1' || player->mat[nextx][nexty] == '2')//���ܻ�ͷײ�Լ�
					continue;
				if (Astar(nextx,nexty,snake.body[snake.length-1],x,snake.body[snake.length-1],player->mat, path))  //�����ҵ�β��  ����������return��һ��
					return ;
			}
		}
	}
	else     //û��ʳ��Ͷܣ������һ��
	{
		int r = rand();
		int nx = player->your_posx + dir[r % 4][0];
		int ny = player->your_posy + dir[r % 4][1];
		if (nx <= -1 || nx >= player->row_cnt || ny <= -1 || ny >= player->col_cnt || player->mat[nx][ny] == '#' || player->mat[nx][ny] == '1' || player->mat[nx][ny] == '2')
			continue;

		if (Astar_(nx,ny,snake.body[snake.length - 1], x, snake.body[snake.length - 1], player->mat, path)  //�����ҵ�β��  ����������return��һ��
					return  ; //����
	}

}

void init(struct Player* Player)
{
	snake.length = 1;
	snake.maxlength = ceil(3.0 * (player->col_cnt + player->row_cnt) / 8);
	snake.body[0].x = player->your_posx;
	snake.body[0].y = player->your_posy;
}



struct Point walk(struct Player* player)
{
	dot path[401];
	int xe, ye;
	decision(player, xe, ye, path);
	int nextx = path[0].x;
	int nexty = path[0].y;
	snake_Move(player,nextx,nexty);
	return InitPoint(nextx,nexty);
}