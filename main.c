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

int main(){

	char* serveur="polydev.cia-polytech-sorbonne.fr";
	char* PlayerName="PetitSerpent";
	connectToServer(serveur, 8080, PlayerName);
	char* typeGame="SUPER_PLAYER difficulty=2 timeout=100 start=1 seed=103";
	char gameName;
	int sizeX;
	int sizeY;
	int nbWalls;
	waitForSnakeGame(typeGame, &gameName, &sizeX, &sizeY, &nbWalls);

	int start;
	int* walls=malloc(4*nbWalls*sizeof(int));

	// définition de la taille de l'arene contenant les positions des murs et des snakes
	t_case** arene;
	arene=malloc(sizeY*sizeof(t_case*));
	for(int i=0;i<sizeY;i++){
		arene[i]=malloc(sizeX*sizeof(t_case));
		for (int j=0;j<sizeX;j++){
			arene[i][j].n=0;
			arene[i][j].e=0;
			arene[i][j].s=0;
			arene[i][j].w=0;
		}
	}

	int** path;
	path=malloc(sizeY*sizeof(int*));
	for(int i=0;i<sizeY;i++){
		path[i]=malloc(sizeX*sizeof(int));
		for (int j=0;j<sizeX;j++){
			path[i][j]=0;
		}
	}

	start=getSnakeArena(walls);

	/*for (int i=0;i<nbWalls;i++){
		printf("%d ", walls[4*i]);
		printf("%d ", walls[4*i+1]);
		printf("%d ", walls[4*i+2]);
		printf("%d ", walls[4*i+3]);
		printf("\n");
	}*/

	// remplissons l'arene avec la position des murs
	initArene(arene, sizeX, sizeY);
	fillArena(arene, walls, nbWalls);

	t_return_code cond1=0;
	t_return_code cond2=0;
	t_move my_move, op_move; /*my and my oponent's move*/

	t_snake_body* position=malloc(sizeof(t_snake_body));

	// initialisation des snakes
	t_snake_body* myS;
	myS=(t_snake_body*) malloc(sizeof(t_snake_body));
	myS->suivant=NULL;
	t_snake my_snake;
	my_snake.count=10;
	my_snake.moi=1;
	my_snake.test=1;
	my_snake.tail=myS;

	t_snake_body* hisS;
	hisS=(t_snake_body*) malloc(sizeof(t_snake_body));
	hisS->suivant=NULL;
	t_snake op_snake; /*my oponent's snake*/
	op_snake.count=10;
	op_snake.moi=0;
	op_snake.test=1;
	op_snake.tail=hisS;

	int max_pos[2]={0};

	printArena();

	// cas ou je commence
	if (start==0){
		initPosition(arene, &my_snake, &op_snake, sizeX, sizeY);
		position=getPosition(&my_snake);
		while((cond1==NORMAL_MOVE) && (cond2==NORMAL_MOVE)){
				position=getPosition(&my_snake);
				depth( arene, path, position, sizeX, sizeY);
				my_move=moveToMake(arene, path, max_pos, position, sizeX, sizeY);
				//affichepath(path, sizeX, sizeY);
				cond1=sendMove(my_move);
				moveSnake(arene, &my_snake, my_move, sizeX, sizeY);
				cond2=getMove(&op_move);
				moveSnake(arene, &op_snake, op_move, sizeX, sizeY);
				if ((cond1==NORMAL_MOVE) && (cond2==NORMAL_MOVE)){
					printArena();
				}
		}
	}
	// cas ou l'adversaire commence
	else if (start==1){
		initPosition(arene, &op_snake, &my_snake, sizeX, sizeY);
		position=getPosition(&my_snake);
		while((cond1==NORMAL_MOVE) && (cond2==NORMAL_MOVE)){
				cond2=getMove(&op_move);
				moveSnake(arene, &op_snake, op_move, sizeX, sizeY);
				position=getPosition(&my_snake);
				depth( arene, path, position, sizeX, sizeY);
				my_move=moveToMake(arene, path, max_pos, position, sizeX, sizeY);
				cond1=sendMove(my_move);
				moveSnake(arene, &my_snake, my_move, sizeX, sizeY);
				if ((cond1==NORMAL_MOVE) && (cond2==NORMAL_MOVE)){
					printArena();

				}
		}
	}

	free(walls);
	// désalocation de l'arene
	for(int i=0;i<sizeY;i++){
		free(arene[i]);
	}
	free(arene);

	// desallocation du tableau pour le parcours en profondeurs
	for(int i=0;i<sizeY;i++){
		free(path[i]);
	}
	free(path);

	free(position);
	/*on supprime les snakes*/
	emptySnake(&my_snake);
	emptySnake(&op_snake);

	closeConnection();
	return 0;
}
