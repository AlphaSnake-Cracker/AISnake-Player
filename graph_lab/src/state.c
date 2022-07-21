#include "state.h"
#include <stdlib.h>

void init_State(struct State *s) {
    // TODO
    s -> nv = 0;
    s -> na = 0;
    for (int i = 0; i < 5000; i += 1)
        s -> v[i].first_arc = NULL;
}

void delete_State(struct State *s) {
    // TODO
}

void parse(struct State *s, struct PNG *p) {
    // TODO

    int width = p->get_width();
    int height = p->get_height();
    /*for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
            cout<<setw(3)<<(int)p->get_pxl(j,i)->red<<','<<setw(3)<< (int)p->get_pxl(j,i)->green<<','<<setw(3)<< (int)p->get_pxl(j,i)->blue<<'\t';
        cout<<endl;
    }
    return;*/
    int hexagon_h,hexagon_w;
    hexagon_h=height/8;
    hexagon_w=width/8;
    for(int i=0;i<hexagon_h;i++)
    {
        int this_w;
        if(i%2==0)
            this_w=hexagon_w;
        else
            this_w=hexagon_w-1;
        for(int j=0;j<this_w;j++)
        {
            int tmp_h,tmp_w;
            if(i%2==0)
            {
                tmp_h=i/2*16+1;
                tmp_w=4+j*8;
            }
            else
            {
                tmp_h=8*i+1;
                tmp_w=8+j*8;
            }
            pxl* tmp_p = p->get_pxl(tmp_w,tmp_h);//参数顺序
            pic[i][j] = 255*255*3 - pow((int)tmp_p->red,2)- pow((int)tmp_p->green,2) -pow((int)tmp_p->blue,2);
            //cout<<pic[i][j]<<' ';
        }
    }
    
    int even_x[6]{-1,1,0,0,-1,1};
    int even_y[6]{0,0,-1,1,-1,-1};
    
    int odd_x[6]{-1,1,0,0,-1,1};
    int odd_y[6]{0,0,-1,1,1,1};
    //初始化
    dis[0][0]=0;
    s[0][0]=1;
    if(hexagon_h > 0)
        dis[1][0] = pic[1][0];
    if(hexagon_w > 0)
        dis[0][1] = pic[0][1];
     
    for(int i=0;i<hexagon_h;i++)//遍历每个点
    {
        int this_w,_min,u_i,u_j,tmp_i,tmp_j,tmp_w,d_w;
        if(i%2==0)
            this_w=hexagon_w;
        else
            this_w=hexagon_w-1;
        for(int j=0;j<this_w;j++)//遍历每个点
        {                        //找到最小的dis，考察最小dis对应六边形，能到达的点的值，并将该值和dis对应值比较
            _min=_max;
                d_w=hexagon_w-1;
            for(int d_i=0;d_i<hexagon_h;d_i++)
            {
                if(d_i%2==0)
                    d_w=hexagon_w;
                else
                    d_w=hexagon_w-1;
                for(int d_j=0;d_j<d_w;d_j++)
                    if(s[d_i][d_j]==0 && dis[d_i][d_j]<_min)
                    {
                        u_i=d_i;
                        u_j=d_j;
                        _min=dis[d_i][d_j];
                    }
            }
                
            //cout<<"找到最小dis："<<" dis["<<u_i<<"]["<<u_j<<"]:"<<dis[u_i][u_j]<<endl;
            s[u_i][u_j]=1;
            if(u_i%2==0)
            {
                for(int k=0;k<6;k++)
                {
                    tmp_i=u_i+even_x[k];
                    tmp_j=u_j+even_y[k];
                    if(tmp_i%2==0)
                        tmp_w=hexagon_w;
                    else
                        tmp_w=hexagon_w-1;
                    //【>=】
                    if(tmp_i>=0 && tmp_i<hexagon_h && tmp_j>=0 && tmp_j<tmp_w && s[tmp_i][tmp_j]==0 && dis[tmp_i][tmp_j]>dis[u_i][u_j]+pic[tmp_i][tmp_j])
                    {
                        //cout<<"考察周边dis:"<<" dis["<<tmp_i<<"]["<<tmp_j<<"]:"<<dis[tmp_i][tmp_j]<<endl;
                        dis[tmp_i][tmp_j]=dis[u_i][u_j]+pic[tmp_i][tmp_j];
                        //cout<<"更新dis"<<" dis["<<tmp_i<<"]["<<tmp_j<<"]:"<<dis[tmp_i][tmp_j]<<endl<<endl;
                    }
                }
            }
            else
            {
                for(int k=0;k<6;k++)
                {
                    tmp_i=u_i+odd_x[k];
                    tmp_j=u_j+odd_y[k];
                    if(tmp_i%2==0)
                        tmp_w=hexagon_w;
                    else
                        tmp_w=hexagon_w-1;
                    //【>=】
                    if(tmp_i>=0 && tmp_i<hexagon_h && tmp_j>=0 && tmp_j<tmp_w && s[tmp_i][tmp_j]==0 && dis[tmp_i][tmp_j]>dis[u_i][u_j]+pic[tmp_i][tmp_j])
                        dis[tmp_i][tmp_j]=dis[u_i][u_j]+pic[tmp_i][tmp_j];
                }
            }
        }
    }
    if(hexagon_h % 2 == 0){
        res = dis[hexagon_h-1][hexagon_w-2];
    }else{
        res = dis[hexagon_h-1][hexagon_w - 1];
    }
    return ;
}
}

int solve1(struct State *s) {
    // TODO

     

    return 0;
}

struct Path *Get_Path(struct State *s)
{
    //Custom
}

int solve2(struct State *s) {
    // TODO
    return 0;
}
