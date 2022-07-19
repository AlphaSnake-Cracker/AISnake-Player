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
};
int judgeapple()  //����food���飬�Ƿ���������Ҷȷ���ƻ��
int judgetail()   //�Ƿ����ҵ�β�ͣ���ƻ������ʼ����A*������û��·��
void updatesnake()   //�¿�һ������洢�ƶ��������


//ÿ�ξ���ǰҪ����ǽ
void decision()       //��ǰͷ��λ��
{
	//ɨ���ͼ����Astar��ʼ����ÿ��Apple�Ͷܵľ��룻
	init_food();
	  

	if (judgeapple() == 1)  //������ڶȷ��ϵ������Apple 
	{
		       updatesnake()  //new ��snake1 ̽·

		if (judgetail()==1)   
		{
		      return   //	astar�����·��path��һ��
		}
		else   //can't find tail
		{
			  //����Apple����ݼ�������ͷ����Χ���ߵĸ���
			{
				    updatesnake()//new snake2̽·,�ƶ�һ��
				if(judgetail()==1)  //�����ҵ�β��  ����������return��һ��
				    	
			}
		}
	}
	else
	{
		            //����ͷ����Χ���ߵĸ���
		{
			          updatesnake()   //new snake1̽·
			if (judgetail()==1)  //�����ҵ�β��  ����������return��һ��
				return   //����
		}
	}

}


