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


#ifndef __API_CLIENT_SNAKE__
#define __API_CLIENT_SNAKE__
#include "ret_type.h"

typedef enum {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
} t_move;



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
void connectToServer( char* serverName, int port, char* name);



/* ----------------------------------
 * Close the connection to the server
 * Has to be done, because we are polite
 */
void closeConnection();


/* -------------------------------------------------------------------------
 * Wait for a Game, and retrieve its name, its sizes and the number of walls
 *
 * Parameters:
 * - gameType: (char*) string (max 150 characters)
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
 *        - 'start': allows to set who starts (0 or 1)
 *        - 'difficulty': between 0 (no walls) and 3 (a lot of walls) - 2 is the default value
 * gameType could also be : "TOURNAMENT name" where name is the name of the tournament
 */
void waitForSnakeGame( char* gameType, char* gameName, int* sizeX, int* sizeY, int* nbWalls);


/* -------------------------------------
 * Get the data and tell who starts
 *
 * Parameters:
  * - walls: (int*) array of nbWalls*4 integers (x1, y1, x2 , y2 for a wall between (x1,y1) and (x2,y2))
 *   (the pointer data MUST HAVE allocated with the right size !!)
 *
 * Returns 0 if you begin, or 1 if the opponent begins
 */
int getSnakeArena(int* walls);



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
t_return_code getMove(t_move* move );



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
t_return_code sendMove(t_move move );



/* ----------------------
 * Display the Game
 * in a pretty way (ask the server what to print)
 */
void printArena();



/* ----------------------------
 * Send a comment to the server
 *
 * Parameters:
 * - comment: (string) comment to send to the server (max 100 char.)
 */
void sendComment(char* comment);



#endif
