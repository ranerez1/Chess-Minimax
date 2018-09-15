/*
 * ChessUtils.c
 *
 *  Created on: 18 Aug 2017
 *      Author: ranerez
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ChessParser.h"
#include "ChessGame.h"
#include "ChessMoves.h"
#include "ChessParams.h"



void printEndGame(GAME* game){
	GAME* copy = copyGame(game);
	if(isTie(copy)){
		destroyGame(copy);
		game->set->gameMode == 1 ? printf(G_TIE) : printf(G_TIE_AI);
	}
	else if(isCheckMate(copy)){
		copy->currPlayer ? printf(G_CM_B) : printf(G_CM_W) ;
		destroyGame(copy);
	}
	else{
		switchPlayers(copy);
		if(isCheckMate(copy)){
				copy->currPlayer ? printf(G_CM_W) : printf(G_CM_B) ;
				destroyGame(copy);
			}
	}
}

void printComputerMove(GAME* game, MOVE* mv){
	char p = getPiece(game,mv);
	char* piece = getPieceString(p);
	printf("Computer: move %s at <%d,%c> to <%d,%c>\n",piece, mv->src.x +1,(char)(mv->src.y+'A'),mv->dst.x +1,(char)(mv->dst.y+'A'));
}

void printSetting(SETTING *set){
	printf("SETTINGS:\n");
	printf("GAME_MODE: %d\n",set->gameMode);
	if(set->gameMode==1){
		printf("DIFFICULTY_LVL: %d\n",set->difficulty);
		if(set->color){
			printf("USER_CLR: WHITE\n");
		}
		else{
			printf("USER_CLR: BLACK\n");
		}
	}

}

void printBoard(char board[MAX_ROW][MAX_COL]){
	char line[]= "  -----------------";
	char rows[]= "   A B C D E F G H";
	for(int i = 0; i < MAX_ROW ; i++){
		printf("%d| ",8-i);
		for(int j = 0 ; j < MAX_COL ; j++){
			printf("%c ",board[7-i][j]);
		}
		printf("|\n");
	}
	printf("%s\n",line);
	printf("%s\n",rows);

}


int compareMoves(MOVE_LIST* mv1, MOVE_LIST* mv2){
	if(mv1->mv.dst.x < mv2->mv.dst.x){
		return -1;
	}
	if((mv1->mv.dst.x == mv2->mv.dst.x) && (mv1->mv.dst.y < mv2->mv.dst.y)){
		return -1;
	}
	if((mv1->mv.dst.x == mv2->mv.dst.x) && (mv1->mv.dst.y == mv2->mv.dst.y)){
		return 0;
	}
	else{
		return 1;
	}

}

void swap(MOVE_LIST *a, MOVE_LIST *b)
{
    MOVE temp = a->mv;
    a->mv = b->mv;
    b->mv = temp;
}

void bubbleSort(MOVE_LIST *start)
{
    int swapped;
    MOVE_LIST *ptr1 =start;
    MOVE_LIST *lptr = NULL;

    /* Checking for empty list */
    if (ptr1 == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr)
        {
            if (compareMoves(ptr1,ptr1->next) == 1)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}




void printBoardTest(){
	char board[MAX_ROW][MAX_COL]= {{'r','n','b','q','k','b','n','r'},
									{'m','m','m','m','m','m','m','m'},
									{'_','_','_','_','_','_','_','_'},
									{'_','_','_','_','_','_','_','_'},
									{'_','_','_','_','_','_','_','_'},
									{'_','_','_','_','_','_','_','_'},
									{'M','M','M','M','M','M','M','M'},
									{'R','N','B','Q','K','B','N','R'}
			};
	printBoard(board);
	cleanBoard(board);
	printBoard(board);
}
