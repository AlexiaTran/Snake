/*=============================================================================
*
*    oO  Header file of fonctions' code  Oo
*
*==============================================================================
*
* File : main.c
* Date : 3 Décembre 2019
* Author : Tran Alexia
*
*==============================================================================
*
*
*
*
*
*
*/
#ifndef __DEPLACE_H__
#define __DEPLACE_H__

/* 0 si il n'y a ni snake ni mur
1 si il y a un mur mais pas de snake
2 si il y a un snake mais pas de mur
3 si il y a snake et mur*/

typedef struct{
	int n;
	int s;
	int w;
	int e;
}t_case;

typedef struct s_element{
  int x;
  int y;
  struct s_element* suivant;
}t_snake_body;

typedef struct{
	int count;
	int moi;
	int test;
	t_snake_body* tail;
}t_snake;


typedef struct s_truc{
	int x;
	int y;
	int longeur;
	struct s_truc* suivant;
}t_length;

/*prototypes*/
/*t_move bouge()*/
void initArene(t_case** arene, int sizeX, int sizeY);
void insertwall(t_case** arene, int x1,int y1, int x2, int y2);
void fillArena(t_case** arene, int* walls,int nbWalls);
void initPosition(t_case** arene, t_snake* player1, t_snake* player2, int sizeX, int sizeY);
t_snake_body* getPosition(t_snake* mon_snake);
t_length* pile(t_length* sommet, int y, int x, int r);
t_length* depile(t_length* sommet);
int** depth(t_case** arene,int** path, t_snake_body* position, int sizeX, int sizeY);
void max_tab(int** path, int* max, int sizeX, int sizeY);
t_move moveToMake(t_case** arene, int** path, int* max, t_snake_body* position, int sizeX, int sizeY);
void snake_leave(t_case** arene, t_snake_body* entity, int sizeX, int sizeY);
void sub_case_snake(t_case** arene, t_snake* snake, int sizeX, int sizeY);
void snake_arrive(t_case** arene, t_snake_body* entity, int sizeX, int sizeY);
void add_case_snake(t_case** arene, t_snake* snake, int sizeX, int sizeY);
void growth(t_snake* snake, t_move move);
void moveSnake(t_case** arene, t_snake* snake, t_move move, int sizeX, int sizeY);
void emptySnake(t_snake* snake);

#endif
