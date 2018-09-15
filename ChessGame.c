/*
 * ChessGame.c
 *
 *  Created on: 24 Aug 2017
 *      Author: ranerez
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ChessGame.h"
#include "ChessUtils.h"
#include "ChessArrayList.h"
#include "ChessMiniMax.h"
#include "ChessParams.h"
#include "ChessMoves.h"

/*
 * Function to initialize the board to the starting state
 * @param game board
 * @ret void
 */
void initBoard(char board[MAX_ROW][MAX_COL]){
	char b[MAX_ROW][MAX_COL] = {{'r','n','b','q','k','b','n','r'},
								{'m','m','m','m','m','m','m','m'},
								{'_','_','_','_','_','_','_','_'},
								{'_','_','_','_','_','_','_','_'},
								{'_','_','_','_','_','_','_','_'},
								{'_','_','_','_','_','_','_','_'},
								{'M','M','M','M','M','M','M','M'},
								{'R','N','B','Q','K','B','N','R'}
								};

	for (int i = 0; i < MAX_ROW; i++){
		for (int j = 0; j < MAX_COL; j++){
			board[i][j] = b[i][j];
		}
	}
}

/*
 * Function to clean a board to the empty symbol
 * @param game board
 * @ret void
 */

void cleanBoard(char board[MAX_ROW][MAX_COL]){
	for (int i = 0; i < MAX_ROW; i++){
		for (int j = 0; j < MAX_COL; j++){
			board[i][j] = '_';
		}
	}
}


/*
 * Function to copy an array of fixed size
 * @param 2 arrays, number of elements to be copied
 * @ret void
 */

void copyArray(int from[], int to[],int size){
	for(int i = 0 ; i < size; i++){
		to[i] = from[i];
	}
}


/*
 * Function to create a new game with a number of possible undo moves
 * @param int history- number of undo moves possible
 * @ret GAME* pointer to the game
 */

GAME* createGame(int history){
	GAME* res = NULL;
	res = (GAME*)malloc(sizeof(GAME));
	if (res == NULL){
		printf("Error: createGame has failed");
		free(res);
		return NULL;
	}
	SETTING* set = NULL;
	set = createSetting(DEF_MODE,DEF_DIFF,DEF_COL);

	res->set = set;
	res->currPlayer = 1;
	for (int i = 0; i < MAX_ROW; i++){
		for (int j = 0; j < MAX_COL; j++){
			res->board[i][j] = EMPTY;
		}
	}
	res->gameArrayList = NULL;
	res->gameArrayList = spArrayListCreate(history);
	initGame(res);
	return res;
}

/*
 * Function to destroy a game and free all allocated memory
 * @param GAME* pointer to the game
 * @ret void
 */

void destroyGame(GAME* game){
	if(game == NULL){
		return;
	}
	spArrayListDestroy(game->gameArrayList);
	destroySetting(game->set);
	free(game);
}

/*
 * Function to copy a game and allocate memory
 * @param GAME* pointer to the game being copied
 * @ret GAME* pointer to a new copied game
 */

GAME* copyGame(GAME* game){
	GAME* res = createGame(HISTORY_SIZE);
	spArrayListDestroy(res->gameArrayList);
	if(res==NULL){
		return NULL;
	}
	res->currPlayer = game->currPlayer;

	res->gameArrayList = spArrayListCopy(game->gameArrayList);
	res->gameStatus = game->gameStatus;
	updateSetting(game->set,res->set);
	for(int i = 0 ; i < MAX_ROW ; i++){
		for(int j = 0 ; j < MAX_COL ; j++){
			(res->board)[i][j]=(game->board)[i][j];
		}
	}
	copyArray(game->whiteArr,res->whiteArr,NUM_PIECES);
	copyArray(game->blackArr,res->blackArr,NUM_PIECES);
	return res;
}

/*
 * Function to update a game from another game (used mainly for file loading)
 * @param GAME* pointer to the game being updated ,pointer to a game we are updating from
 * @ret void
 */

void updateGame(GAME* from, GAME* to){
	updateSetting(from->set,to->set);
	spArrayListDestroy(to->gameArrayList);
	to->gameArrayList = spArrayListCopy(from->gameArrayList);
	to->gameStatus = from->gameStatus;
	to->currPlayer = from->currPlayer;
	for(int i = 0 ; i < MAX_ROW ; i++){
		for(int j = 0 ; j < MAX_COL ; j++){
			to->board[i][j] = from->board[i][j];
		}
	}
	copyArray(from->whiteArr,to->whiteArr,NUM_PIECES);
	copyArray(from->blackArr,to->blackArr,NUM_PIECES);
}



/*
 * Function to initialize a new game
 * @param GAME* pointer to the game being initialized
 * @ret void
 */

void initGame(GAME* game){
	int arr[] = {8,2,2,2,1,1};
	initBoard(game->board);
	spArrayListClear(game->gameArrayList);
	setDefault(game->set);
	game->gameStatus = 0;
	copyArray(arr,game->whiteArr,NUM_PIECES);
	copyArray(arr,game->blackArr,NUM_PIECES);


}

void printUndo(MOVE mv, int player){
	char* p_b = "black";
	char* p_w="white";
	if(player){
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",p_w,
				(mv.src.x + 1),
				(char)(mv.src.y + 'A'),
				(mv.dst.x + 1),
				(char)(mv.dst.y + 'A')
				);
	}
	else{
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",p_b,
				(mv.src.x + 1),
				(char)(mv.src.y + 'A'),
				(mv.dst.x + 1),
				(char)(mv.dst.y + 'A')
				);
	}

}

void updateArrays(GAME* game, char piece_dst,int sign){
	char* pieces = islower(piece_dst) ? "mnbrqk" : "MNBRQK";
	int loc = 0 ;
	const char* ptr = strchr(pieces,piece_dst);
	if(ptr){
		loc = ptr - pieces;
		if(islower(piece_dst)){
			(game->whiteArr)[loc] += sign;
		}
		else{
			(game->blackArr)[loc] += sign;
		}

	}
}

void undoMove(GAME* game,int player){
	MOVE mv;
	if(game->set->gameMode==2){
		printf(E_UNDO_2P);
	}
	else{
		if(spArrayListIsEmpty(game->gameArrayList)){
			if(player == game->set->color){
				printf(E_UNDO_EMPTY);
			}
			switchPlayers(game);

		}
		else{
			mv = spArrayListGetLast(game->gameArrayList);
			game->board[mv.src.x][mv.src.y] = mv.piece;
			game->board[mv.dst.x][mv.dst.y] = mv.piece_dst;
			if(mv.piece_dst!=EMPTY){
				updateArrays(game,game->board[mv.dst.x][mv.dst.y],+1);
			}
			printUndo(mv, player);
			spArrayListRemoveLast(game->gameArrayList);
			switchPlayers(game);
		}
	}
}

void undoMoveNoPrint(GAME* game,int player){
	MOVE mv;
	if(game->set->gameMode==2){
	}
	else{
		if(spArrayListIsEmpty(game->gameArrayList)){
			if(player == game->set->color){
			}
			switchPlayers(game);

		}
		else{
			mv = spArrayListGetLast(game->gameArrayList);
			game->board[mv.src.x][mv.src.y] = mv.piece;
			game->board[mv.dst.x][mv.dst.y] = mv.piece_dst;
			if(mv.piece_dst!=EMPTY){
				updateArrays(game,game->board[mv.dst.x][mv.dst.y],+1);
			}
			spArrayListRemoveLast(game->gameArrayList);
			switchPlayers(game);
		}
	}
}

void computerTurn(GAME* game){
	MOVE* mv = NULL;
	mv =  getBestMove(game, game->set->difficulty);
	printComputerMove(game,mv);
	if(!isTie(game) && existsGameWinner(game) == 0){
	makeMove(game,mv);
	}
	destroyMove(mv);

}

