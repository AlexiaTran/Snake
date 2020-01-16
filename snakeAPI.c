/*
* ------------------------ *
|                          |
|   -= snakeAPI =-         |
|                          |
| based on                 |
| Coding Game Server       |
|                          |
* ------------------------ *

Authors: T. Hilaire, T. Gautier
Licence: GPL

File: snkaeAPI.h
	Contains the client API for the Snake game
	-> based on clientAPI.h

Copyright 2019 T. Hilaire, T. Gautier
*/

#include "clientAPI.h"
#include <stdio.h>
#include "snakeAPI.h"

unsigned int nbW; 	/* store the nb of walls, used for getGame (the user do not have to pass them once again */

/* -------------------------------------
 * Initialize connection with the server
 * Quit the program if the connection to the server
 * cannot be established
 *
 * Parameters:
 * - serverName: (char*) address of the server
 *   (it could be "localhost" if the server is run in local,
 *   or "pc4521.polytech.upmc.fr" if the server runs there)
 * - port: (int) port number used for the connection
 * - name: (char*) name of the bot : max 20 characters
 *         (checked by the server)
 */
void connectToServer( char* serverName, int port, char* name)
{
	connectToCGS( __FUNCTION__, serverName, port, name);
}


/* ----------------------------------
 * Close the connection to the server
 * Has to be done, because we are polite
 */
void closeConnection()
{
	closeCGSConnection( __FUNCTION__ );
}


/* -------------------------------------------------------------------------
 * Wait for a Game, and retrieve its name, its sizes and the number of walls
 *
 * Parameters:
 * - gameType: (char*) string (max 50 characters)
 * - gameName: (char*) string (max 50 characters), corresponds to the game name
 * - sizeX, sizeY: (int*) pointers to the dimension of the game
 * - nbWalls: (int*) pointer to the number of walls
 *
 * gameType is a string like "NAME key1=value1 key2=value1 ..."
 * - NAME can be empty. If not, it gives the type of the training player
 * - key=value pairs are used for options
 *   (each training player has its own options)
 *   invalid keys are ignored, invalid values leads to error
 *   the following options are common to every training player
 *   (when NAME is not empty or not TOURNAMENT):
 *        - 'timeout': allows an define the timeout
 *                   when training (in seconds)
 *        - 'seed': allows to set the seed of the random generator
 *        - 'start': allows to set who starts ('0' or '1')
 * gameType could also be : "TOURNAMENT name" where name is the name of the tournament
 */
void waitForSnakeGame( char* gameType, char* gameName, int* sizeX, int* sizeY, int* nbWalls)
{
	char data[128];
	/* wait for a game */
	waitForGame( __FUNCTION__, gameType, gameName, data);

	/* parse the data */
	sscanf( data, "%d %d %d", sizeX, sizeY, nbWalls);

	/* store the nb of walls, so that we can reuse it during getSnakeGameData */
	nbW = *nbWalls;
}


/* -------------------------------------
 * Get the data and tell who starts
 *
 * Parameters:
  * - walls: (int*) array of nbWalls*4 integers (x1, y1, x2 , y2 for a wall between (x1,y1) and (x2,y2))
 *   (the pointer data MUST HAVE allocated with the right size !!)
 *
 * Returns 0 if you begin, or 1 if the opponent begins
 */
int getSnakeArena(int* walls)
{
	char data[4096*8];
	char *p = data;
	int n;
	/* wait for a game */
	int ret = getGameData( __FUNCTION__, data, 4096*8);

	/* copy the data in the array walls
	 * the datas is a readable string of integers
	 * */
	for(int i=0; i<nbW; i++)
	{
		sscanf(p, "%d %d %d %d%n", walls, walls+1, walls+2, walls+3, &n);
		walls += 4;
		p += n;
	}

    return ret;
}



/* ----------------------
 * Get the opponent move
 *
 * Parameters:
 * - move: a move
 *
 * Returns a return_code
 * NORMAL_MOVE for normal move,
 * WINNING_MOVE for a winning move, -1
 * LOOSING_MOVE for a losing (or illegal) move
 * this code is relative to the opponent (WINNING_MOVE if HE wins, ...)
 */
t_return_code getMove( t_move* move )
{

    char data[128];   /* to define */

    /* get the move */
    int ret = getCGSMove( __FUNCTION__, data, 128);

	/* extract move */
	sscanf( data, "%d", (int*) move);
	dispDebug(__FUNCTION__,2,"move: %d, ret: %d",*move,ret);
	return ret;
}



/* -----------
 * Send a move
 *
 * Parameters:
 * - move: a move
 *
 * Returns a return_code
 * NORMAL_MOVE for normal move,
 * WINNING_MOVE for a winning move, -1
 * LOOSING_MOVE for a losing (or illegal) move
 * this code is relative to your programm (WINNING_MOVE if YOU win, ...)
 */
t_return_code sendMove( t_move move )
{
    /* build the string move */
    char data[128];
    sprintf( data, "%d", move);
	dispDebug(__FUNCTION__, 2, "move send : %s",data);
    /* send the move */
	return sendCGSMove( __FUNCTION__, data);
}




/* ----------------------
 * Display the Game
 * in a pretty way (ask the server what to print)
 */
void printArena()
{
    printGame( __FUNCTION__ );
}



/* ----------------------------
 * Send a comment to the server
 *
 * Parameters:
 * - comment: (string) comment to send to the server (max 100 char.)
 */
void sendComment(char* comment)
{
    sendCGSComment( __FUNCTION__, comment);
}
