/*
 * ChessMoves.h
 *
 *  Created on: 25 Aug 2017
 *      Author: ranerez
 */
#include "ChessGame.h"
#include "ChessParams.h"

#ifndef CHESSMOVES_H_
#define CHESSMOVES_H_

/*
 * The ChessMoves module is responsible for validating and executing a move
 * The module verifies that a move is legal according to all chess rules
 * The module executes the relevant move
 */

/*
 * the MOVE_LIST struct is a linked list storing all possible moves.
 * it is used for the get moves command
 * MOVE a struct of the move
 * MOVE_LIST* a pointer to the next element in the linked list
 */
typedef struct list_t{
	MOVE mv;
	struct list_t* next;
}MOVE_LIST;

/*
 * The function returns all legal moves for a specific board location
 * The function receives as input:
 * @param - GAME* a pointer to the current game
 * @param - POSITION* a pointer to the evaluated position
 * @ret SPArrayList* a pointer an array of possible moves
 */
SPArrayList* getAllPieceMoves(GAME* game, POSITION* piece_pos);

/*
 * A function to return the char representation of a piece located in the position
 * @param- GAME* a pointer to the current game
 * @param MOVE* a pointer to the move being executed
 * @ret- char the piece located in the source of the move
 */
char getPiece(GAME* game, MOVE* mv);

/*
 * A function for creating a defualt move initialized to -1,-1 for initialization purposes
 * @param - void
 * @ret - a struct of type MOVE
 */
MOVE defaultMove();



/*
 * a function to switch between players in a game
 * @param -GAME* a pointer to the game being updated
 * @ret void
 */
void switchPlayers(GAME*);

/*
 * a function to make a move in a game and update the board
 * @param -GAME* a pointer to the game being updated
 * @param - MOVE* a pointer to the performed move
 * @ret void
 */
void makeMove(GAME* , MOVE* );


/*
 * a function to check if the move executed contains the player's piece
 * @param -GAME* a pointer to the game being updated
 * @param - MOVE* a pointer to the performed move
 * @ret bool - true if yes, false otherwise
 */
bool myPiece(GAME* game, MOVE* mv);


/*
 * a function to check if the move executed is legal
 * @param -GAME* a pointer to the game being updated
 * @param - MOVE* a pointer to the performed move
 * @ret bool - true if yes, false otherwise
 */
bool legalMove(GAME* game, MOVE* mv);



/*
 * The function returns all legal moves for a specific board location  for get moves command
 * The function receives as input:
 * @param - GAME* a pointer to the current game
 * @param - POSITION* a pointer to the evaluated position
 * @ret MOVE_LIST* a pointer an array of possible moves
 */
MOVE_LIST* getPieceMoves(GAME* game, POSITION* piece_pos);

/*
 * The function returns all legal moves for a specific player - for get moves command
 * The function receives as input:
 * @param - GAME* a pointer to the current game
 * @ret MOVE_LIST* a pointer an array of possible moves
 */
MOVE_LIST* getAllMovesForPlayer(GAME* game);

/*
 * a function to check if the current player is under check
 * @param -GAME* a pointer to the game
 * @ret bool - true if yes, false otherwise
 */
bool playerInCheck(GAME* game);
/*
 * a function to check if the current player is under checkmate
 * @param -GAME* a pointer to the game
 * @ret bool - true if yes, false otherwise
 */
bool isCheckMate(GAME* game);
/*
 * a function to check if the game is tied
 * @param -GAME* a pointer to the game
 * @ret bool - true if yes, false otherwise
 */
bool isTie(GAME* game);
/*
 * a function to check if exists a winner to the game
 * @param -GAME* a pointer to the game
 * @ret int - 1- if current player wins, -1 if current player looses, 0- otherwise
 */
int existsGameWinner(GAME* game);

/*
 * a function to check if there is a winner
 * @param -GAME* a pointer to the game
 * @ret bool - true if yes, false otherwise
 */
bool existsGameWinner2(GAME* game);

/*
 * a function for printing all moves for the get moves command
 * @param -GAME* a pointer to the game
 * @param MOVE_LIST* a pointer an array of possible moves
 */
void printListOfMoves(MOVE_LIST*,GAME*);

/*
 * a function to return a string of the char representation of a piece
 * for example b||B = "bishop"
 * @param char - the piece representation
 * @ret char* the name of the piece
 */
char* getPieceString(char);


/*
 * Destroys a MOVE_LIST and frees all allocated resources
 * @param - MOVE_LIST* - a pointer to the list of moves being destroyed
 * @ret - void
 */
void destroyMoveList(MOVE_LIST* head);

#endif /* CHESSMOVES_H_ */
