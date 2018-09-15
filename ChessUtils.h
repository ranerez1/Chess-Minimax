/*
 * ChessUtils.h
 *
 *  Created on: 19 Aug 2017
 *      Author: ranerez
 */

#ifndef CHESSUTILS_H_
#define CHESSUTILS_H_


#include "ChessGame.h"
#include "ChessMoves.h"
#include "ChessParams.h"

/*
 * The ChessUtils module handles auxillary functions used to print or calculate
 */


/*
 * The function prints the setting as defined in the requirements
 * The function receives;
 * @param- SETTING* - a pointer to the setting to print
 * @ret - void
 */
void printSetting(SETTING*);

/*
 * The function prints the current board as defined in the requirements
 * The function receives;
 * @param- char[][] - an array of char arrays representing the board
 * @ret - void
 */
void printBoard(char[MAX_ROW][MAX_COL]);

/*
 * The function prints the computer's move as defined in the requirements
 * The function receives;
 * @param- GAME* - a pointer to the current game
 * @param- MOVE* - a pointer to the move required to be printed
 * @ret - void
 */
void printComputerMove(GAME* game, MOVE* mv);

/*
 * The function prints the the end of the game based on a checkmate or a tie and if its 1-player or 2 player game
 * as defined in the requirements
 * The function receives;
 * @param- GAME* - a pointer to the current game
 * @ret - void
 */
void printEndGame(GAME* game);

/*
 * The generic function to implement bubble sort for the get moves command.
 * The function receives;
 * @param- MOVE_LIST* - a pointer to the linked list containing all the possible moves
 * @ret - void (sorts the list in place)
 */
void bubbleSort(MOVE_LIST *start);

/*
 * The comparator function used for bubble sort.
 * @param- void* p1
 * @param - void* p2
 * @ret - 1 if p2 > p1 , 0 if p2 = p1 -1 if p1 > p2
 */

int compareMoves(const void* p1, const void* p2);
#endif /* CHESSUTILS_H_ */
