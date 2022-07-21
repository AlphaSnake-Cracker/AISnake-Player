#ifndef STATE_H_
#define STATE_H_
#include "suan_png.h"
#include "pxl.h"

struct Arc
{
    //Data Structure
    int src; //Sorce
    int dst; //Destination
    int weight; //Weight is decided by the industry of dst
    struct Arc *next_arc; //Next arc with same src, NULL when no such arc
};

struct Vertex
{
    //Date Structre
    int dis; //Pay no attention, used when solving
    struct Arc *first_arc; //The frist arc whose src is this vertex, NULL when no such arc
    int prev_idx; //Pay no attention, used when solving
};

struct State {
    // data structure
    int nv; //Number of vertexes
    int na; //Number of arcs, an edge is considered as two arcs
    struct Vertex v[5000]; //Maximum number of vertexes is 4990
};

struct Path
{
    //Data Structure
    int a;
    int b;
};

// function
void init_State(struct State *s); //Guarantee all first_arc pointers points to NULL
void delete_State(struct State *s);
void assign(struct State *a, struct State *b);
void parse(struct State *s, struct PNG *p);
int solve1(struct State *s);
struct Path *Get_Path(struct State *s);
int solve2(struct State *s);
       
#endif
