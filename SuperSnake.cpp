#include"../include/playerbase.h"
#include<string.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define MaxSize 500
#define INF 65535
#define maxsize 50
int N = 0; //行
int M = 0; //列
struct node {
	int x;
	int y;
	int du;
	int distance;
}food[maxsize];//道具信息

typedef struct {
	int x;
	int y;
}Path;

struct Snakestatus {
	int length;//蛇的长度
	int maxlength;//蛇的最大长度
	Path body[40];//身体的位置
}snake;

int dir[4][2] = { {0,1},{1,0},{0,-1},{-1,0} };

char map[500][500] = { 0 };

typedef struct
{
	int x;
	int y;
	int parent;
	int G;
	int H;
	int F;
}Aelem;

typedef struct
{
	int top;
	Aelem list[MaxSize];
	int ElemSituation[MaxSize];
	int number;
}AstarList;

void InitList(AstarList **Alist);
void AddOpenList(Aelem *elem, AstarList *Alist);
void AddCloseList(AstarList *OpenList, AstarList *CloseList, int x, int y, int position[MaxSize][MaxSize], int direction[4][2], char map[MaxSize][MaxSize], Path path[MaxSize], int *sum, int *isOk);
void GetF(AstarList *OpenList, Aelem *elem, int x, int y);
int Astar(int starx, int starty, int x, int y, char map[MaxSize][MaxSize], Path path[MaxSize]);



/*int main()    // ÷ ʾ
{
	char map[MaxSize][MaxSize] = {
	{'.','.','.','.','.','.','.'},
	{'.','.','#','.','#','.','.'},
	{'.','#','.','#','.','#','.'},
	{'.','.','.','#','#','.','.'},
	{'.','.','.','#','.','.','.'},
	{'.','.','.','#','.','.','.'},
	};
	Path path[500];
	int a = 0;
	N = 5;
	M = 6;
	a=Astar(1, 1, 5, 5, map, path);
	printf("%d", a);
}*/

int Astar(int startx, int starty, int x, int y, char map[MaxSize][MaxSize], Path path[MaxSize])
{
	//printf("enter Astar\n");
	int direction[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };
	int position[MaxSize][MaxSize] = { 0 };
	int sum = 0;
	int isOk = 0;
	AstarList *OpenList = NULL;
	AstarList *CloseList = NULL;
	InitList(&OpenList);
	InitList(&CloseList);
	Aelem elem;
	elem.x = startx, elem.y = starty;
	elem.parent = -1;
	elem.H = (x - elem.x) * 10 + (y - elem.y) * 10;
	elem.G = 0;
	elem.F = elem.G + elem.H;
	position[elem.x][elem.y] = 1;
	AddOpenList(&elem, OpenList);
	if (x == startx && y == starty)
	{
		return 0;
	}
	while (1)
	{
		AddCloseList(OpenList, CloseList, x, y, position, direction, map, path, &sum, &isOk);
		if (isOk == 1)
		{
			return sum;
		}
		if (isOk == -1)
		{
			return -1;
		}
	}
}

void InitList(AstarList **Alist)
{
	(*Alist) = (AstarList *)malloc(sizeof(AstarList));
	(*Alist)->top = -1;
	for (int i = 0; i < MaxSize; i++)
	{
		(*Alist)->ElemSituation[i] = 0;
	}
	(*Alist)->number = 0;
}

void AddOpenList(Aelem *elem, AstarList *Alist)
{
	Alist->top++;
	Alist->list[Alist->top] = *elem;
	Alist->ElemSituation[Alist->top] = 1;
	Alist->number++;
}

void AddCloseList(AstarList *OpenList, AstarList *CloseList, int x, int y, int position[MaxSize][MaxSize], int direction[4][2], char map[MaxSize][MaxSize], Path path[MaxSize], int *sum, int *isOk)//   x  y  ʾҪ      յ      
{
	if (OpenList->number == 0)
	{
		*isOk = -1;
		return;
	}
	int Fmin = INF;
	int j;
	Aelem e;
	for (int i = 0; i <= OpenList->top; i++)
	{
		if (OpenList->ElemSituation[i] == 1)
		{
			if (Fmin > OpenList->list[i].F)
			{
				Fmin = OpenList->list[i].F;
				j = i;
			}
		}
	}
	AddOpenList(OpenList->list + j, CloseList);//  Ԫ ؼ  뵽closelist  
	OpenList->ElemSituation[j] = -1;
	position[OpenList->list[j].x][OpenList->list[j].y] = -1;
	OpenList->number--;
	for (int i = 0; i < 4; i++)
	{
		e.x = OpenList->list[j].x + direction[i][0];
		e.y = OpenList->list[j].y + direction[i][1];
		if (0 <= e.x && e.x <= N && 0 <= e.y && e.y <= M)//debug
		{
			if (position[e.x][e.y] == 0 && map[e.x][e.y] != '#' && map[e.x][e.y] != '2' && map[e.x][e.y] != '1')//        û    ̽            
			{
				e.parent = j;
				GetF(OpenList, &e, x, y);
				AddOpenList(&e, OpenList);
				position[e.x][e.y] = 1;
				//printf("%d %d\n", e.x, e.y);//debug
				if (e.x == x && e.y == y)
				{
					path[0].x = x;
					path[0].y = y;
					*sum = 0;
					int p = e.parent;
					while (p != -1)
					{
						(*sum)++;
						path[*sum].x = OpenList->list[p].x;
						path[*sum].y = OpenList->list[p].y;
						p = OpenList->list[p].parent;
					}
					*isOk = 1;
					return;
				}
			}
			else if (position[e.x][e.y] == 1 && map[e.x][e.y] != '#' && map[e.x][e.y] != '2' && map[e.x][e.y] != '1')//          openlist  
			{
				for (int k = 0; k <= OpenList->top; k++)//  openlist   ҵ      
				{
					if (e.x == OpenList->list[k].x && e.y == OpenList->list[k].y)
					{
						e.G = OpenList->list[k].G;
						break;
					}
				}
				if (OpenList->list[j].G + 10 < e.G)
				{
					e.G = OpenList->list[j].G + 10;
					e.parent = j;
				}
			}
		}
	}
}

void GetF(AstarList *OpenList, Aelem *elem, int x, int y)//   x  y  ʾҪ      յ      
{
	if (elem->parent == -1)
	{
		elem->G = 0;
	}
	else
	{
		elem->G = OpenList->list[elem->parent].G + 10;
	}
	elem->H = (x - elem->x) * 10 + (y - elem->y) * 10;
	elem->F = elem->G + elem->H;
}

int init_food(struct Player *player, struct node food[])
{
	printf("enter init_food\n");
	int count = 0;
	int x = player->your_posx;
	int y = player->your_posy;

	for (int i = 0; i < player->row_cnt; i++)//row_cnt行数，col_cnt列数
	{
		//printf("i\n");
		for (int j = 0; j < player->col_cnt; j++)
		{
			//printf("%d\n",player->col_cnt);
			if (player->mat[i][j] == 'o' || player->mat[i][j] == 'O')
			{
				food[count].x = i;
				food[count].y = j;
				int manhadun = (int)(fabs((double)(x - i)) + fabs((double)(y - j)));
				food[count].distance = manhadun;
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
				count++;
			}
		}
	}
	printf("out init_food\n");
	//printf("%d",count);
	return count;
}

int judge_apple(struct Player *player, int *applex, int *appley, Path path[], int *k)  //    food   飬 Ƿ         Ҷȷ   ƻ  
{
	printf("enter judge_apple\n");
	int min = 200000;
	int num = init_food(player, food);
	int near;
	int x = player->your_posx, y = player->your_posy;
	N = player->row_cnt;
	M = player->col_cnt;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			map[i][j] = player->mat[i][j];
	printf("num = %d\n", num);
	for (int i = 0; i < num; i++)
	{
		if (food[i].distance < min && Astar(x, y, food[i].x, food[i].y, map, path) >= 0 && food[i].du >= 2)
		{
			*k = Astar(x, y, food[i].x, food[i].y, map, path);
			//printf("%d\n",*k);
			min = food[i].distance;
			near = i;
		}
	}
	int nearr = Astar(x, y, food[near].x, food[near].y, map, path);
	//printf("path:\n");
	//printf("k = %d\n",*k);
	//for(int i=0;i<*k;i++)
	//{
		//printf("%d %d\n",path[i].x,path[i].y);
	//}
	//printf("near = %d\n",near);
	if (min != 200000)
	{
		*applex = food[near].x;
		*appley = food[near].y;
		return 1;
	}
	return 0;
}

int judgetail(int xe, int ye, int tailx, int taily, Path path_1[]) // Ƿ    ҵ β ͣ   ƻ      ʼ    A*      û  ·  
{
	printf("enter judgetail\n");
	if (Astar(xe, ye, tailx, taily, map, path_1) >= 0)
		return 1;
	else
		return 0;
}

void snake_Move(struct Player *player, int x, int y, struct Snakestatus falsesnake)
{
	if (player->mat[x][y] == 'o' || player->mat[x][y] == 'O')
		falsesnake.maxlength++;

	if (falsesnake.length < falsesnake.maxlength)
	{
		falsesnake.length += 1;

		for (int i = falsesnake.length - 1; i > 0; i -= 1)
			falsesnake.body[i] = falsesnake.body[i - 1];

		falsesnake.body[0].x = x;
		falsesnake.body[0].y = y;

		map[x][y] = '1';
	}
	else
	{
		int tailx = falsesnake.body[falsesnake.length - 1].x;
		int taily = falsesnake.body[falsesnake.length - 1].y;

		for (int i = falsesnake.length - 1; i >= 0; i -= 1)
			falsesnake.body[i + 1] = falsesnake.body[i];
		falsesnake.body[0].x = x;
		falsesnake.body[0].y = y;

		map[tailx][taily] = '.';    //update map
		map[x][y] = '1';
	}
}

Path NewMap(struct Player *player, Path path[], int k)
{
	// for (int i = 0; i < player->row_cnt; i++)   //    ͼ
	// 	for (int j = 0; j<player->col_cnt; j++)
	// 		map[i][j] = player->mat[i][j];

	struct Snakestatus falsesnake = snake;
	for (int i = k - 1; i >= 0; i--)    //     ƶ path  
	{
		snake_Move(player, path[i].x, path[i].y, falsesnake);
	}
	int Tailx=falsesnake.body[falsesnake.length-1].x,Taily=falsesnake.body[falsesnake.length-1].y;
	map[Tailx][Taily]='3';
	
	return falsesnake.body[falsesnake.length - 1];
}


void decision(struct Player *player, int *xe, int *ye, Path path[])       //  ǰͷ  λ  
{
	printf("enter decision\n");
	int x, y;
	x = player->your_posx;
	y = player->your_posy;
	int k;
	int applex, appley;
	if (judge_apple(player, &applex, &appley, path, &k) == 1)  //      ڶȷ  ϵ      Apple
	{
		//printf("applex = %d appley = %d\n", applex, appley);
		// char map[20][20];
		Path tail_path = NewMap(player, path, k);
		//printf("tail_path.x = %d tail_path.y = %d\n",tail_path.x,tail_path.y);
		Path  path_1[50];

		if (judgetail(applex, appley, tail_path.x, tail_path.y, path_1) == 1)
		{
			*xe = path[k - 1].x;   *ye = path[k - 1].y;
			printf("xe = %d ye = %d\n",*xe,*ye);
			for (int i = 0; i < k; i++)
			{
				printf("%d %d\n", path[i].x, path[i].y);
			}
			return; //	astar     ·  path  һ  
		}
		else   //can't find tail
		{

			/* for (int i = 0; i < player->row_cnt; i++)   //    ͼ
			 for (int j = 0; j<player->col_cnt; j++)
			 map[i][j] = player->mat[i][j];*/
			for (int i = 0; i < 4; i++)
			{
				int nextx = x + dir[i][0];
				int nexty = y + dir[i][1];
				if (nextx <= -1 || nextx >= player->row_cnt || nexty <= -1 || nexty >= player->col_cnt || player->mat[nextx][nexty] == '#' || player->mat[nextx][nexty] == '1' || player->mat[nextx][nexty] == '2')//   ܻ ͷײ Լ 
					continue;
				if (Astar(nextx, nexty, snake.body[snake.length - 1].x, snake.body[snake.length - 1].y, map, path) >= 0)  //     ҵ β              return  һ  
				{
					*xe = nextx;
					*ye = nexty;
					return;
				}
			}
		}
	}
	else     //û  ʳ  Ͷܣ      һ  
	{
		/* for (int i = 0; i < player->row_cnt; i++)   //    ͼ
		 for (int j = 0; j<player->col_cnt; j++)
		 map[i][j] = player->mat[i][j];*/


		while (1)
		{
			int r = rand();
			int nx = player->your_posx + dir[r % 4][0];
			int ny = player->your_posy + dir[r % 4][1];
			if (nx <= -1 || nx >= player->row_cnt || ny <= -1 || ny >= player->col_cnt || player->mat[nx][ny] == '#' || player->mat[nx][ny] == '1' || player->mat[nx][ny] == '2')
				continue;
			if (Astar(nx, ny, snake.body[snake.length - 1].x, snake.body[snake.length - 1].y, map, path) >= 0)  //     ҵ β              return  һ  
			{
				*xe = nx;  *ye = ny;
				return; //    
			}
		}
	}

}

void init(struct Player *player)
{
	snake.length = 1;
	snake.maxlength = ceil(3.0 * (player->col_cnt + player->row_cnt) / 8);
	snake.body[0].x = player->your_posx;
	snake.body[0].y = player->your_posy;

}


struct Point walk(struct Player *player)
{
	Path path[401];
	int xe, ye;
	decision(player, &xe, &ye, path);

	snake_Move(player, xe, ye, snake);//
	printf("%d %d\n", xe, ye);
	return initPoint(xe, ye);
}