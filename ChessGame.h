/*
 * ChessGame.h
 *
 *  Created on: 24 Aug 2017
 *      Author: ranerez
 */

#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#include <stdio.h>
#include <stdlib.h>
#include "ChessSetting.h"
#include "ChessArrayList.h"
#include "ChessParams.h"

/*
 * A structure holding the necssary attributes for the game
 * board[MAX_ROW][MAX_COL] - an 8X8 char array representing the current position of all pieces
 * set- the setting of the current game as defined in setting stage or load
 * currPlayer- who's turn is it- 0- for black 1- for white
 * gameStatus- represents the game's status- 0 - Setting 1-Game 2-Ended
 * whiteArr - represents the number of white pieces from each kind on the board
 * blackArr - represents the number of black pieces from each kind on the board
 */

typedef struct game_t{
	char board[MAX_ROW][MAX_COL];
	SETTING* set;
	SPArrayList* gameArrayList;
	int currPlayer;
	int gameStatus; //0- setting 1-game 2- end
	int whiteArr[NUM_PIECES]; //Counts the number of white pieces on the board for each type
	int blackArr[NUM_PIECES]; //Counts the number of white pieces on the board for each type
}GAME;


/*
 * Function to initialize the board to the starting state
 * @param game board
 * @ret void
 */
void initBoard(char board[MAX_ROW][MAX_COL]);

/*
 * Function to clean a board to the empty symbol
 * @param game board
 * @ret void
 */
void cleanBoard(char board[MAX_ROW][MAX_COL]);

/*
 * Function to create a new game with a number of possible undo moves
 * @param int history- number of undo moves possible
 * @ret GAME* pointer to the game
 */
GAME* createGame(int);

/*
 * Function to copy a game and allocate memory
 * @param GAME* pointer to the game being copied
 * @ret GAME* pointer to a new copied game
 */

GAME* copyGame(GAME*);

/*
 * Function to destroy a game and free all allocated memory
 * @param GAME* pointer to the game
 * @ret void
 */
void destroyGame(GAME*);

/*
 * Function to update a game from another game (used mainly for file loading)
 * @param GAME* pointer to the game being updated ,pointer to a game we are updating from
 * @ret void
 */

void updateGame(GAME* from, GAME* to);

/*
 * Function to initialize a new game
 * @param GAME* pointer to the game being initialized
 * @ret void
 */
void initGame(GAME*);


/*
 * Function for undoing a previous move. The function recieves a pointer to the game and the current player
 * and removes the last move from the ArrayList. The function prints the undo move accordingly
 * @param - GAME* pointer to the current game, int- current player to undo his move
 * @ret void
 */
void undoMove(GAME*, int );

/*
 * Function for undoing a previous move without printing. The function recieves a pointer to the game and the current player
 * and removes the last move from the ArrayList.
 * @param - GAME* pointer to the current game, int- current player to undo his move
 * @ret void
 */
void undoMoveNoPrint(GAME* game,int player);


/*
 * Function to update the number of pieces from each type. White array refers to the white pieces and black array to the black pieces
 * The function recieves a pointer to the game ,the piece to update and a sign to add or remove a piece (+ or -)
 * and removes the last move from the ArrayList. The function prints the undo move accordingly
 * @param - GAME* pointer to the current game, char- piece int- increasing or decreasing the number of pieces from that type
 * @ret void
 */
void updateArrays(GAME* game, char piece_dst,int sign);

/*
 * Function to simulate the computer move and call the minimax algorithm for computing the optimal move.
 * @param - GAME* a pointer to the current game. the function is only called at the computer's turn.
 */
void computerTurn(GAME* game);
#endif /* CHESSGAME_H_ */
