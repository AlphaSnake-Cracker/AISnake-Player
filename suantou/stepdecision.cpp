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

struct point snake_body[maxsize];

int init_food(struct Player *player , struct node food[])
{ 
	int count = 0;
	int x = player->your_posx;
	int y = player->your_posy;

	  for(int i=0;i<player->row_cnt;i++)
		  for (int j = 0; i < player->col_cnt; j++)
		  {
			  if (player->mat[i][j] == 'o'||player->mat[i][j]=='O')
			  {
				  food[count].x = i;
				  food[count].y = j;
				  int manhadun = fabs(x - i) + fabs(y - j);
				  food[count].distance = manhadun;
				  count++;
				  if (j + 1 < player->col_cnt)
					  if (player->mat[i][j + 1] != '#' && player->mat[i][j + 1] != '1' && player->mat[i][j + 1] != '2')
						  food[count].du++;
				  if (j-1>=0)
					  if (player->mat[i][j-1] != '#' && player->mat[i][j-1] != '1' && player->mat[i][j-1] != '2')
						  food[count].du++;
				  if (i+1< player->row_cnt)
					  if (player->mat[i+1][j] != '#' && player->mat[i+1][j] != '1' && player->mat[i+1][j] != '2')
						  food[count].du++;
				  if (i-1>=0)
					  if (player->mat[i-1][j] != '#' && player->mat[i-1][j] != '1' && player->mat[i-1][j] != '2')
						  food[count].du++;
			  }
		  }
	  return count;
};

int judgeapple(struct Player* player, int* xe, int* ye)  //遍历food数组，是否有最近的且度符合苹果
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


int judgetail()   //是否能找到尾巴，从苹果出开始，用A*，看有没有路径
void updatesnake()   //新开一个数组存储移动后的蛇身


//每次决策前要更新墙
void decision()       //当前头部位置
{
	//扫描地图，用Astar初始化到每个Apple和盾的距离；
	init_food();
	  

	if (judgeapple() == 1)  //存过存在度符合的最近的Apple 
	{
		       updatesnake()  //new 个snake1 探路

		if (judgetail()==1)   
		{
		      return   //	astar最近的路上path走一步
		}
		else   //can't find tail
		{
			  //按到Apple距离递减来检索头部周围可走的格子
			{
				    updatesnake()//new snake2探路,移动一步
				if(judgetail()==1)  //可以找到尾巴  跳出检索并return这一步
				    	
			}
		}
	}
	else
	{
		            //检索头部周围可走的格子
		{
			          updatesnake()   //new snake1探路
			if (judgetail()==1)  //可以找到尾巴  跳出检索并return这一步
				return   //坐标
		}
	}

}


