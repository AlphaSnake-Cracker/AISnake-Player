#include"src/Astar.cpp"
#include"include/playerbase.h"
#include<string.h>
#include<stdio.h>
#include<math.h>

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

int dir[4][2] = { {0,1},{1,0},{0,-1},{-1,0} };  //右 下  左 上

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

int judgeapple(struct Player* player, int* xe, int* ye, Path path[])  //遍历food数组，是否有最近的且度符合苹果
{
	int min = 200000;
	int num = initfood(player, food);
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

int judgetail(int xe, int ye, int map[20][20], Path path[], int k, struct Player* player) //是否能找到尾巴，从苹果出开始，用A*，看有没有路径
{
	int x, y;
	if (player->your_score + 2 > k)
		x = queue[front + k].x; y = queue[front + k].y;
	else
		x = path[k - (player->your_score + 1)].x; y = path[k - (player->your_score + 1)].y;
	Path path0[401];
	if (Astar(xe, ye, x, y, map, path0))
		return 1;
	else return 0;
}

void snake_Move(struct Player* player, int x, int y,struct Snakestatus falsesnake,int map[20][20])
{
	if (player->mat[x][y] == 'o' || player->mat[x][y] == 'O')
		falsesnake.length += 1;

	int tailx = falsesnake.body[falsesnake.length - 1].x;    //尾巴的坐标
	int taily = falsesnake.body[falsesnake.length - 1].y;

	for (int i = falsesnake.length - 1; i >= 0; i -= 1)
		falsesnake.body[i + 1] = falsesnake.body[i];

	falsesnake.body[0].x = x;
	falsesnake.body[0].y = y;

	if (falsesnake.length < falsesnake.max_length)
		falsesnake.length += 1;

	map[x][y] = 1;    //新地图刷新一步
	map[tailx][taily] = '.';
}

void NewMap(struct Player* player, int map[20][20], Path path[], int k)
{
	for (int i = 0; i < player->row_cnt; i++)   //复制图
		for (int j = 0; j<player->col_cnt; j++)
			map[i][j] = player->mat[i][j];

	Snakestatus falaesnake = snake;
	for (int i = 0; i < k; i++)    //假蛇移动path长
	{
		snake_Move(player, path[i].x, path[i].y, falaesnake, map);   
	}
}


void decision(struct Player* player, int& xe, int& ye, dot path[])       //当前头部位置
{
	//扫描地图，用Astar初始化到每个Apple和盾的距离；
	int x, y;
	x = player->your_posx;
	y = player->your_posy;
	if (judgeapple(player, &xe, &ye, path) == 1)  //存过存在度符合的最近的Apple
	{
		int map[20][20];
		NewMap(player, map, path, k);

		if (judgetail(xe, ye, map, path, k, player) == 1)
		{
			return path[0]; //	astar最近的路上path走一步
		}
		else   //can't find tail
		{
			for(int i=0;i<4;i++)
			{
				int nextx = x+dir[i][0];
				int nexty = y+dir[i][1];
				if (nextx<= -1 || nextx >= player->row_cnt ||nexty <= -1 ||nexty >= player->col_cnt || player->mat[nextx][nexty] == '#' || player->mat[nextx][nexty] == '1' || player->mat[nextx][nexty] == '2')//不能回头撞自己
					continue;
				if (Astar(nextx,nexty,snake.body[snake.length-1],x,snake.body[snake.length-1],player->mat, path))  //可以找到尾巴  跳出检索并return这一步
					return ;
			}
		}
	}
	else     //没有食物和盾，随机走一步
	{
		int r = rand();
		int nx = player->your_posx + dir[r % 4][0];
		int ny = player->your_posy + dir[r % 4][1];
		if (nx <= -1 || nx >= player->row_cnt || ny <= -1 || ny >= player->col_cnt || player->mat[nx][ny] == '#' || player->mat[nx][ny] == '1' || player->mat[nx][ny] == '2')
			continue;

		if (Astar(nx,ny,snake.body[snake.length - 1], x, snake.body[snake.length - 1], player->mat, path)  //可以找到尾巴  跳出检索并return这一步
					return  ; //坐标
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