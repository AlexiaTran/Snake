/*=============================================================================
*
*    oO  Codage d'un jeu Snake  Oo
*
*==============================================================================
*
* File : main.c
* Date : 3 Décembre 2019
* Author : Tran Alexia
*
*==============================================================================
*
* Remarque: Le fichier contient
*
*
*
*
*/
#include<stdio.h>
#include<stdlib.h>
#include"clientAPI.h"
#include"snakeAPI.h"
#include"deplace.h"


/*t_move bouge()
{
	char c;
	scanf("%c",&c);
	t_move move;
	switch (c)
	{
		case 'z':
			move=n;
		break;
		case 'q':
			move=w;
		break;
		case 's':
			move=s;
		break;
		case 'd':
			move=e;
		break;
		default:
		break;
	}
	return move;
}*/

void initArene(t_case** arene, int sizeX, int sizeY){
	for (int i=0;i<sizeY;i++){
		arene[i][0].w=1;
	}
	for (int i=0;i<sizeX;i++){
		arene[0][i].n=1;
	}
	for(int i=0;i<sizeY;i++){
		arene[i][sizeX-1].e=1;
	}
	for(int i=0;i<sizeX;i++){
		arene[sizeY-1][i].s=1;
	}
}

void insertwall(t_case** arene, int x1,int y1, int x2, int y2){
	if (x1==x2){
		if (y1<y2){
			arene[y1][x1].s=1;
			arene[y2][x2].n=1;
		}
		else{
			arene[y1][x1].n=1;
			arene[y2][x2].s=1;
		}
	}
	else{
		if (x1<x2){
			arene[y1][x1].e=1;
			arene[y2][x2].w=1;
		}
		else{
			arene[y1][x1].w=1;
			arene[y2][x2].e=1;
		}
	}
}

void fillArena(t_case** arene, int* walls,int nbWalls){
	int x1;
	int y1;
	int x2;
	int y2;
	for (int i=0;i<nbWalls;i++){
		x1=walls[4*i];
		y1=walls[4*i+1];
		x2=walls[4*i+2];
		y2=walls[4*i+3];
		insertwall(arene, x1, y1, x2, y2);
	}
}


void snake_leave(t_case** arene, t_snake_body* entity, int sizeX, int sizeY){
	arene[entity->y][entity->x].n-=2;
	if (entity->y!=0)
		arene[entity->y-1][entity->x].s-=2;
	arene[entity->y][entity->x].s-=2;
	if (entity->y!=sizeY-1)
		arene[entity->y+1][entity->x].n-=2;
	arene[entity->y][entity->x].e-=2;
	if (entity->x!=sizeX-1)
		arene[entity->y][entity->x+1].w-=2;
	arene[entity->y][entity->x].w-=2;
	if (entity->x!=0)
		arene[entity->y][entity->x-1].e-=2;
}

void sub_case_snake(t_case** arene, t_snake* snake, int sizeX, int sizeY){
	t_snake_body* temp=snake->tail;
	if (snake->moi==0){
		while(temp!=NULL){
			snake_leave(arene, temp, sizeX, sizeY);
			temp=temp->suivant;
		}
	}
	else{
		while(temp->suivant!=NULL){
			snake_leave(arene, temp, sizeX, sizeY);
			temp=temp->suivant;
		}
	}
}

void growth(t_snake* snake, t_move move){
	t_snake_body* sommet;
	sommet=(t_snake_body*) malloc(sizeof(t_snake_body));
	t_snake_body* temp=snake->tail;
	while(temp->suivant!=NULL){
		temp=temp->suivant;
	}
	temp->suivant=sommet;
	switch (move){
		case NORTH:
			temp->suivant->y=temp->y-1;
			temp->suivant->x=temp->x;
		break;
		case SOUTH:
			temp->suivant->y=temp->y+1;
			temp->suivant->x=temp->x;
		break;
		case EAST:
			temp->suivant->x=temp->x+1;
			temp->suivant->y=temp->y;
		break;
		case WEST:
			temp->suivant->x=temp->x-1;
			temp->suivant->y=temp->y;
		break;
	}
	temp->suivant->suivant=NULL;
}

void moveSnake(t_case** arene, t_snake* snake, t_move move, int sizeX, int sizeY){
	sub_case_snake(arene, snake, sizeX, sizeY);
	t_snake_body* temp=snake->tail;
	int count=snake->count;
	int x;
	int y;
	if (count!=10){
		snake->count++;
		while(temp->suivant!=NULL){
			temp->x=temp->suivant->x;
			temp->y=temp->suivant->y;
			x=temp->x;
			y=temp->y;
			temp=temp->suivant;
		}
		switch (move)	{
			case NORTH:
				temp->y=y-1;
				temp->x=x;
			break;
			case SOUTH:
				temp->y=y+1;
				temp->x=x;
			break;
			case EAST:
				temp->x=x+1;
				temp->y=y;
			break;
			case WEST:
				temp->x=x-1;
				temp->y=y;
			break;
		}
	}
	else{
		snake->count=1;
		if (snake->test==1){
			snake->count=2;
			snake->test=0;
		}
		growth(snake, move);
	}
	add_case_snake(arene, snake, sizeX, sizeY);
}

void snake_arrive(t_case** arene, t_snake_body* entity, int sizeX, int sizeY){
	if (entity->x>=0 && entity->x<sizeX && entity->y>=0 && entity->y< sizeY){
		arene[entity->y][entity->x].n+=2;
		if (entity->y!=0)
			arene[entity->y-1][entity->x].s+=2;
		arene[entity->y][entity->x].s+=2;
		if (entity->y!=sizeY-1)
			arene[entity->y+1][entity->x].n+=2;
		arene[entity->y][entity->x].e+=2;
		if (entity->x!=sizeX-1)
			arene[entity->y][entity->x+1].w+=2;
		arene[entity->y][entity->x].w+=2;
		if (entity->x!=0)
			arene[entity->y][entity->x-1].e+=2;
	}
}


void add_case_snake(t_case** arene, t_snake* snake, int sizeX, int sizeY){
	t_snake_body* temp=snake->tail;
	if (snake->moi==1){
		while(temp->suivant!=NULL){
			snake_arrive( arene, temp, sizeX, sizeY);
			temp=temp->suivant;
		}
	}
	else{
		while(temp!=NULL){
			snake_arrive( arene, temp, sizeX, sizeY);
			temp=temp->suivant;
		}
	}
}

void initPosition(t_case** arene, t_snake* player1, t_snake* player2, int sizeX, int sizeY){
	player1->tail->x=2;
	player1->tail->y=sizeY/2;
	player2->tail->x=sizeX-3;
	player2->tail->y=sizeY/2;
	if (player1->moi==0){
		arene[player1->tail->y][player1->tail->x].n+=2;
		arene[player1->tail->y-1][player1->tail->x].s+=2;
		arene[player1->tail->y][player1->tail->x].s+=2;
		arene[player1->tail->y+1][player1->tail->x].n+=2;
		arene[player1->tail->y][player1->tail->x].e+=2;
		arene[player1->tail->y][player1->tail->x+1].w+=2;
		arene[player1->tail->y][player1->tail->x].w+=2;
		arene[player1->tail->y][player1->tail->x-1].e+=2;
	}
	if (player2->moi==0){
		arene[player2->tail->y][player2->tail->x].n+=2;
		arene[player2->tail->y-1][player2->tail->x].s+=2;
		arene[player2->tail->y][player2->tail->x].s+=2;
		arene[player2->tail->y+1][player2->tail->x].n+=2;
		arene[player2->tail->y][player2->tail->x].e+=2;
		arene[player2->tail->y][player2->tail->x+1].w+=2;
		arene[player2->tail->y][player2->tail->x].w+=2;
		arene[player2->tail->y][player2->tail->x-1].e+=2;
	}
}

void emptySnake(t_snake* snake){
	t_snake_body* temp=snake->tail;
	t_snake_body* acc=temp;
	while (acc!=NULL){
		acc=temp->suivant;
		free(temp);
		temp=acc;
	}
	snake->tail=NULL;
}

int** depth(t_case** arene,int** path, t_snake_body* position, int sizeX, int sizeY){
	for(int i=0;i<sizeY;i++){
		for (int j=0;j<sizeX;j++){
			path[i][j]=0;
		}
	}
	t_length* temp=malloc(sizeof(t_length*));
	temp->x=position->x;
	temp->y=position->y;
	temp->longeur=1;
	temp->suivant=NULL;
	int x=position->x;
	int y=position->y;
	int r;
	path[y][x]=temp->longeur;
	while(temp!=NULL){
		x=temp->x;
		y=temp->y;
		r=temp->longeur;
		temp=depile(temp);
		if (0<=y && y<sizeY && 0<=x-1 && x-1<sizeX && (arene[y][x].w==0) && path[y][x-1]==0){
			path[y][x-1]=r+1;
			temp=pile(temp, y, x-1, r+1);
		}
		if (0<=y+1 && y+1<sizeY && 0<=x && x<sizeX && (arene[y][x].s==0) && path[y+1][x]==0){
			path[y+1][x]=r+1;
			temp=pile(temp, y+1, x, r+1);
		}
		if (0<=y-1 && y-1<sizeY && 0<=x && x<sizeX && (arene[y][x].n==0) && path[y-1][x]==0){
			path[y-1][x]=r+1;
			temp=pile(temp ,y-1, x, r+1);
		}
		if (0<=y && y<sizeY && 0<=x+1 && x+1<sizeX && (arene[y][x].e==0) && path[y][x+1]==0){
			path[y][x+1]=r+1;
			temp=pile(temp, y, x+1, r+1);
		}
	}
	free(temp);
	return path;
}

t_length* pile(t_length* sommet, int y, int x, int r){
	t_length* temp=malloc(sizeof(t_length*));
	temp->y=y;
	temp->x=x;
	temp->longeur=r;
	temp->suivant=sommet;
	return temp;
}

t_length* depile(t_length* sommet){
	if (sommet!=NULL){
		t_length* temp=sommet->suivant;
		free(sommet);
		return(temp);
	}
	else{
		exit(EXIT_FAILURE);
	}
}

t_snake_body* getPosition(t_snake* snake){
	t_snake_body* temp=snake->tail;
	while(temp->suivant!=NULL){
		temp=temp->suivant;
	}
	return temp;
}

void max_tab(int** path, int* max, int sizeX, int sizeY){
	int temp=0;
	for(int i=0;i<sizeY;i++){
		for (int j=0;j<sizeX;j++){
			if (temp<path[i][j]){
				temp=path[i][j];
				max[0]=i;
				max[1]=j;
			}
		}
	}
}

t_move moveToMake(t_case** arene, int** path, int* max, t_snake_body* position, int sizeX, int sizeY){
	max_tab(path, max, sizeX, sizeY);

	int x=max[1];
	int y=max[0];
	int temp=path[y][x];
	printf("%d\n", temp);
	if (temp==1){
		return NORTH;
	}
	else{
		t_move move;
		while(temp!=1){
			if (0<=y+1 && y+1<sizeY && 0<=x && x<sizeX && path[y+1][x]==temp-1 && (arene[y][x].s==0)){
				y=y+1;
				temp=temp-1;
				path[y][x]=-1;
			}
			else if (0<=y && y<sizeY && 0<=x-1 && x-1<sizeX && (path[y][x-1]==temp-1) && (arene[y][x].w==0)){
				x=x-1;
				temp=temp-1;
				path[y][x]=-1;
			}
			else if (0<=y-1 && y-1<sizeY && 0<=x && x<sizeX && path[y-1][x]==temp-1 && (arene[y][x].n==0)){
				y=y-1;
				temp=temp-1;
				path[y][x]=-1;
			}
			else if (0<=y && y<sizeY && 0<=x+1 && x+1<sizeX && path[y][x+1]==temp-1 && (arene[y][x].e==0)){
				x=x+1;
				temp=temp-1;
				path[y][x]=-1;
			}
		}
		if (0<=y+1 && y+1<sizeY && 0<=x && x<sizeX && path[position->y+1][position->x]==-1 && (arene[y][x].s==0)){
			move=SOUTH;
			printf("s\n");
		}
		else if (0<=y-1 && y-1<sizeY && 0<=x && x<sizeX && path[position->y-1][position->x]==-1 && (arene[y][x].n==0)){
			move=NORTH;
			printf("n\n");
		}
		else if (0<=y && y<sizeY && 0<=x+1 && x+1<sizeX && path[position->y][position->x+1]==-1 && (arene[y][x].e==0)){
			move=EAST;
			printf("e\n");
		}
		else if (0<=y && y<sizeY && 0<=x-1 && x-1<sizeX && path[position->y][position->x-1]==-1 && (arene[y][x].w==0)){
			move=WEST;
			printf("w\n");
		}
		return move;
	}
}
