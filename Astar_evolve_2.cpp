#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MaxSize 500//随便设的值
#define INF 65535//无穷大

int N = 0;//行数
int M = 0;//列数

typedef struct
{
	int x;
	int y;
	int parent;
	int G;
	int H;
	int F;//G,H,F是寻路时要用的几个参数，不用管他
}Aelem;

typedef struct
{
	int top;
	Aelem list[MaxSize];
	int ElemSituation[MaxSize];//0代表这个位置没有放元素，1代表有元素，-1代表该元素被移动到closelist中了
	int number;
}AstarList;

typedef struct
{
	int x;
	int y;
}Path;

void InitList(AstarList **Alist);
void AddOpenList(Aelem *elem, AstarList *Alist);
void AddCloseList(AstarList *OpenList, AstarList *CloseList, int x, int y, int position[MaxSize][MaxSize], int direction[4][2], char map[MaxSize][MaxSize], Path path[MaxSize], int *sum, int *isOk);
void GetF(AstarList *OpenList, Aelem *elem, int x, int y);
int Astar(int starx, int starty, int x, int y, char map[MaxSize][MaxSize], Path path[MaxSize]);

/*int main()    //用法示例
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

int Astar(int startx, int starty, int x, int y, char map[MaxSize][MaxSize], Path path[MaxSize])//使用Astar算法时调用的接口
{
	int direction[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };
	int position[MaxSize][MaxSize] = { 0 };
	int sum = 0;
	int isOk = 0;
	AstarList *OpenList = NULL;
	AstarList *CloseList = NULL;
	InitList(&OpenList);
	InitList(&CloseList);
	Aelem elem;
	elem.x = startx, elem.y = starty;//elem需要初始需要赋值为蛇头开始的位置
	elem.parent = -1;
	elem.H = (x - elem.x) * 10 + (y - elem.y) * 10;
	elem.G = 0;
	elem.F = elem.G + elem.H;
	position[elem.x][elem.y] = 1;
	AddOpenList(&elem, OpenList);
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

void AddCloseList(AstarList *OpenList, AstarList *CloseList, int x, int y, int position[MaxSize][MaxSize], int direction[4][2], char map[MaxSize][MaxSize], Path path[MaxSize], int *sum, int *isOk)//这个x，y表示要到达的终点的坐标
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
	AddOpenList(OpenList->list + j, CloseList);//把元素加入到closelist中
	OpenList->ElemSituation[j] = -1;
	position[OpenList->list[j].x][OpenList->list[j].y] = -1;
	OpenList->number--;
	for (int i = 0; i < 4; i++)
	{
		e.x = OpenList->list[j].x+direction[i][0];
		e.y = OpenList->list[j].y + direction[i][1];
		if (0 <= e.x && e.x <= N && 0 <= e.y && e.y <= M)//debug
		{
			if (position[e.x][e.y] == 0 && map[e.x][e.y] != '#' && map[e.x][e.y] != '2' && map[e.x][e.y] != '1')//如果这个点没有试探过而且能这走
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
			else if (position[e.x][e.y] == 1 && map[e.x][e.y] != '#' && map[e.x][e.y] != '2' && map[e.x][e.y] != '1')//如果这个点在openlist中
			{
				for (int k = 0; k <= OpenList->top; k++)//在openlist中找到这个点
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

void GetF(AstarList *OpenList, Aelem *elem, int x, int y)//这个x，y表示要到达的终点的坐标
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
