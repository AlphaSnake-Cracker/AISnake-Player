#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MaxSize 500//������ֵ
#define INF 65535//�����
int direction[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };//��̽ʱ��ѡ����ĸ�����
int position[MaxSize][MaxSize] = { 0 };//1����õ�λ��openlist�У�-1����λ��closelist�У�0����δ�����κ��б�
int sum = 0;//·������
int isOk = 0;
typedef struct
{
	int x;
	int y;
	int parent;
	int G;
	int H;
	int F;//G,H,F��Ѱ·ʱҪ�õļ������������ù���
}Aelem;

typedef struct
{
	int top;
	Aelem list[MaxSize];
	int ElemSituation[MaxSize];//0�������λ��û�з�Ԫ�أ�1������Ԫ�أ�-1�����Ԫ�ر��ƶ���closelist����
}AstarList;

int map[MaxSize][MaxSize] = {
	{1,1,1,1,1,1,1},
	{1,1,0,1,0,1,1},
	{1,0,1,0,1,1,1},
	{1,1,1,0,1,1,1},
	{1,1,1,0,1,1,1},
	{1,1,1,0,1,1,1},
};//1�����ܵ���0������ʯͷ

typedef struct
{
	int x;
	int y;
}Path;

Path path[MaxSize];
void InitList(AstarList **Alist);
void AddOpenList(Aelem *elem, AstarList *Alist);
void AddCloseList(AstarList *OpenList, AstarList *CloseList, int x, int y);
void GetF(AstarList *OpenList, Aelem *elem, int x, int y);

int main()//�÷�ʾ��
{
	int x, y;
	x = 5, y = 5;//��x��y��ֵΪ�յ�
	AstarList *OpenList = NULL;
	AstarList *CloseList = NULL;
	InitList(&OpenList);
	InitList(&CloseList);
	Aelem elem;
	elem.x = 1, elem.y = 1;//elem��Ҫ��ʼ��Ҫ��ֵΪ��ͷ��ʼ��λ��
	elem.parent = -1;
	elem.H = (x - elem.x) * 10 + (y - elem.y) * 10;
	elem.G = 0;
	elem.F = elem.G + elem.H;
	position[elem.x][elem.y] = 1;
	AddOpenList(&elem, OpenList);
	while (isOk==0)
	{
		AddCloseList(OpenList, CloseList, x, y);

	}
	//printf("%d", sum);
	for (int i = sum; i >= 0; i--)
	{
		printf("%d %d\n", path[i].x, path[i].y);
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
}

void AddOpenList(Aelem *elem, AstarList *Alist)
{
	Alist->top++;
	Alist->list[Alist->top] = *elem;
	Alist->ElemSituation[Alist->top] = 1;
}

void AddCloseList(AstarList *OpenList, AstarList *CloseList, int x, int y)//���x��y��ʾҪ������յ������
{
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
	AddOpenList(OpenList->list + j, CloseList);//��Ԫ�ؼ��뵽closelist��
	OpenList->ElemSituation[j] = -1;
	position[OpenList->list[j].x][OpenList->list[j].y] = -1;
	for (int i = 0; i < 4; i++)
	{
		e.x = OpenList->list[j].x+direction[i][0];
		e.y = OpenList->list[j].y + direction[i][1];
		if (0 <= e.x && e.x <= 5 && 0 <= e.y && e.y <= 5)//debug
		{
			if (position[e.x][e.y] == 0 && map[e.x][e.y] == 1)//��������û����̽������������
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
					sum = 0;
					int p = e.parent;
					while (p != -1)
					{
						sum++;
						path[sum].x = OpenList->list[p].x;
						path[sum].y = OpenList->list[p].y;
						p = OpenList->list[p].parent;
					}
					isOk = 1;
					return;
				}
			}
			else if (position[e.x][e.y] == 1 && map[e.x][e.y] == 0)//����������openlist��
			{
				if (OpenList->list[j].G + 10 < e.G)
				{
					e.G = OpenList->list[j].G + 10;
					e.parent = j;
				}
			}
		}
	}
}

void GetF(AstarList *OpenList, Aelem *elem, int x, int y)//���x��y��ʾҪ������յ������
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
