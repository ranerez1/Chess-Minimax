/*
 * ChessMoves.c
 *
 *  Created on: 18 Aug 2017
 *      Author: ranerez
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "ChessGame.h"
#include "ChessMoves.h"
#include "ChessParams.h"
#include "ChessUtils.h" //[TODO] REMOVE


MOVE_LIST* createMoveList(MOVE mv, MOVE_LIST* next){
	MOVE_LIST* res = (MOVE_LIST*)malloc(sizeof(MOVE_LIST));
	if(res== NULL){
		printf("ERROR: createMoveList has failed, file:%s line:%d\n",__FILE__,__LINE__);
		return NULL;
	}
	res->next = next;
	res->mv = mv;
	return res;
}

void destroyMoveList(MOVE_LIST* head){
	MOVE_LIST* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }

}


MOVE_LIST* addMoveToList(MOVE_LIST* head, MOVE mv){
	MOVE_LIST* cursor = head;
	if(head == NULL){
		MOVE_LIST* res = createMoveList(mv,NULL);
		return res;
	}
    while(cursor->next != NULL){
    	cursor = cursor->next;
    }

    /* create a new node */
    MOVE_LIST* new_node =  createMoveList(mv,NULL);
    cursor->next = new_node;


    return head;
}


bool validPosition(POSITION *pos){
	if(pos->x > 7 || pos->x < 0 || pos->y> 7 ||pos->y< 0){
		return false;
	}
	else{
		return true;
	}
}

char getPiece(GAME* game, MOVE* mv){
	//printf("%p\n",mv);
	int srcx = mv->src.x;
	int srcy = mv->src.y;

	char currentPiece = (game->board)[srcx][srcy];
	return currentPiece;
}

char* getPieceString(char p){
	switch(p){
			case 'm':
			return "pawn";
			break;

			case 'M':
				return "pawn";
			break;

			case 'b':
			return "bishop";
			break;

			case 'B':
			return "bishop";
			break;

			case 'r':
				return "rook";
			break;

			case 'R':
				return "rook";
			break;

			case 'n':
				return "knight";
			break;

			case 'N':
				return "knight";
			break;

			case 'q':
				return "queen";
			break;

			case 'Q':
				return "queen";
			break;

			case 'k':
				return "king";
			break;

			case 'K':
				return "king";
			break;

			default:
				return "";
				break;

			}

	}


char getToSquare(GAME* game, MOVE* mv){
	int dstx = mv->dst.x;
	int dsty = mv->dst.y;
	char toSquare = (game->board)[dstx][dsty];
	return toSquare;
}

bool legalDestination(GAME* game, MOVE *mv){
	char piece = getPiece(game,mv);
	char toSquare = getToSquare(game,mv);
	if(islower(piece)){
		return ((isupper(toSquare)) || (toSquare==EMPTY));
	}
	else{
		return ((islower(toSquare)) || (toSquare==EMPTY));
	}
	return false;
}

bool legalPawnMove(GAME* game, MOVE* mv){
	int srcx = mv->src.x;
	int srcy = mv->src.y;
	int dstx = mv->dst.x;
	int dsty = mv->dst.y;
	char piece = getPiece(game,mv);
	char toSquare = getToSquare(game,mv);

	//first step
	if(((srcx == 1)&&(piece =='m') && (game->board[2][dsty]==EMPTY) && (game->board[3][dsty]==EMPTY)) || ((srcx == 6)&&(piece =='M')&&(game->board[5][dsty]==EMPTY)&&(game->board[4][dsty]==EMPTY))){
		if(abs(dstx-srcx)==2 && (srcy == dsty)){
			return true;
		}
	}

	switch(piece){

	case 'M':
			if( (srcx == dstx +1 && //moving up one row
				 srcy == dsty &&  // staying in the same column
				 toSquare == EMPTY) //moving to an empty piece
				||
				(srcx == dstx +1 && //moving up one row
				abs(srcy - dsty) == 1 &&  // diagonal - trying to eat an enemy piece
				islower(toSquare))	//there is an enemy piece in the destination
				){
				return true; // legal move

			}
			else{
				return false; // illegal move
			}
		break;
	case 'm':
		if( (srcx == dstx -1 && //moving down one row
			 srcy == dsty &&  // staying in the same column
			 toSquare == EMPTY) //moving to an empty piece
			||
			(srcx == dstx -1 && //moving down one row
			abs(srcy - dsty) == 1 &&  // diagonal - trying to eat an enemy piece
			isupper(toSquare))	//there is an enemy piece in the destination
			){
			return true; // legal move

		}
		else{
			return false; // illegal move
		}
		break;
	default:
		return false;
		break;
	}
}

bool legalBishopMove(GAME* game, MOVE* mv){
	int srcx = mv->src.x;
	int srcy = mv->src.y;
	int dstx = mv->dst.x;
	int dsty = mv->dst.y;
	int rowMove = abs(srcx-dstx);
	int colMove = abs(srcy-dsty);
	bool diagonal = (rowMove == colMove); //making sure it is a diagonal movement
	int upDown = 0;
	int leftRight = 0;

	if(!legalDestination(game,mv)){
		return false;
	}

	if(diagonal){
		if(srcx > dstx){ //checking diagonal of rows
			upDown = -1;
		}
		else{
			upDown = 1;
		}
		if(srcy > dsty){ //checking diagonal of columns
			leftRight = -1;
		}
		else{
			leftRight = 1;
		}

		for(int i = 1 ; i < abs(srcx-dstx); i++){
			int row = srcx + i*upDown;
			int col = srcy + i*leftRight;
			if(game->board[row][col]!=EMPTY){
				return false;
			}
		}
		return true;
	}
	else{
		return false;
	}
}

bool legalRookMove(GAME* game, MOVE* mv){
	int srcx = mv->src.x;
	int srcy = mv->src.y;
	int dstx = mv->dst.x;
	int dsty = mv->dst.y;
	bool colMove = (srcx != dstx) && (srcy == dsty);
	bool rowMove = (srcx == dstx) && (srcy!=dsty);
	bool both = colMove && rowMove;
	bool any = colMove || rowMove;

	if(!validPosition(&(mv->src)) || !validPosition(&(mv->dst))){
		return false;
	}

	if(!legalDestination(game,mv)){
		return false;
	}

	if(any && !both){

		if(srcx< dstx){
			for (int i = srcx + 1; i != dstx; i++){
						if ((game->board)[i][srcy] != EMPTY){
							return false;
						}
					}
			return true;
		}

		else if(srcx > dstx){
			for (int i = srcx - 1; i != dstx; i--){
						if ((game->board)[i][srcy] != EMPTY){
							return false;
						}

					}
			return true;
		}

		if(srcy < dsty){
			for (int i = srcy + 1; i != dsty; i++) {
						if ((game->board)[srcx][i] != EMPTY) {
							return false;
						}
					}
			return true;
		}

		else if(srcy > dsty){
			for (int i = srcy - 1; i != dsty; i--) {
						if ((game->board)[srcx][i] != EMPTY) {
							return false;
						}
					}
			return true;
		}
		return true;
	}
	else{

		return false;
	}


}

bool legalKnightMove(GAME* game, MOVE* mv){
	int srcx = mv->src.x;
	int srcy = mv->src.y;
	int dstx = mv->dst.x;
	int dsty = mv->dst.y;
	bool twoVertOneHor = (abs(srcx-dstx) == 2) && (abs(srcy-dsty)==1);
	bool twoHorOneVer = (abs(srcx-dstx) == 1) && (abs(srcy-dsty)==2);
	return (twoVertOneHor || twoHorOneVer) && (legalDestination(game,mv));
}

bool legalKingMove(GAME* game, MOVE* mv){
	int srcx = mv->src.x;
	int srcy = mv->src.y;
	int dstx = mv->dst.x;
	int dsty = mv->dst.y;
	bool colMove = (abs(srcx-dstx) == 1) && (srcy == dsty);
	bool rowMove = (abs(srcy-dsty) == 1) && (srcx == dstx);
	bool diagonal = (abs(srcx-dstx) == 1) && (abs(srcy-dsty) == 1);
	return (colMove||rowMove||diagonal) && legalDestination(game,mv);
}

bool legalQueenMove(GAME* game, MOVE* mv){
	return (legalRookMove(game,mv) || legalBishopMove(game,mv));
}



POSITION* findPiece(char piece, char board[MAX_ROW][MAX_COL]){
	POSITION* pos = createPosition(-1,-1);
	for(int i = 0 ; i < MAX_ROW ; i++){
		for(int j = 0 ; j < MAX_COL ; j++){
			if(board[i][j]== piece){
				pos->x = i;
				pos->y = j;
			}
		}
	}
	return pos;
}





bool myPiece(GAME* game, MOVE* mv){
	char ch =getPiece(game,mv);
	if(ch== EMPTY){
		return false;
	}
	if(game->currPlayer && islower(ch)){
		return true;
	}
	else if(!(game->currPlayer) && isupper(ch)){
		return true;
	}
	return false;
}

bool legalMove(GAME* game, MOVE* mv){
	//printBoard(game->board); //[TODO] remove
	char piece = getPiece(game,mv);
	//printf("%c\n",piece); //[TODO] remove
	bool legal = false;

	if(!validPosition(&(mv->dst))){
		return false;
	}

	if(!myPiece(game,mv)){

		return legal;
	}

	if(myPiece(game,mv) && legalDestination(game,mv)){
		switch (piece){

		case 'm':
		legal = legalPawnMove(game,mv);
		break;

		case 'M':
		legal = legalPawnMove(game,mv);

		break;

		case 'b':
		legal = legalBishopMove(game,mv);
		break;

		case 'B':
		legal = legalBishopMove(game,mv);
		break;

		case 'r':
		legal = legalRookMove(game,mv);
		break;

		case 'R':
		legal = legalRookMove(game,mv);
		break;

		case 'n':
		legal = legalKnightMove(game,mv);
		break;

		case 'N':
		legal = legalKnightMove(game,mv);
		break;

		case 'q':
		legal = legalQueenMove(game,mv);
		break;

		case 'Q':
		legal = legalQueenMove(game,mv);
		break;

		case 'k':
		legal = legalKingMove(game,mv);
		break;

		case 'K':
		legal = legalKingMove(game,mv);
		break;

		default:
			legal = false;
			break;

		}
	}
	else{
		return legal;
	}
	bool kingDangeredAfterMove(GAME* game, MOVE* mv);
	bool kingNotDangered = !(kingDangeredAfterMove(game,mv));
	if(!legal || !kingNotDangered){
	}
	return (legal && kingNotDangered);

}

bool pawnThreat(POSITION king , GAME *game){
	bool res = false;
	POSITION* pos[4]= {createPosition(king.x+1,king.y+1),
						   createPosition(king.x+1,king.y-1),
						   createPosition(king.x-1,king.y+1),
						   createPosition(king.x-1,king.y-1)};

	char pawns[4] = {'_','_','_','_'};

	for(int i = 0 ; i < 4 ; i++){
		if(validPosition(pos[i])){
			pawns[i] = game->board[pos[i]->x][pos[i]->y];
		}
		destroyPosition(pos[i]);
	}



	if(game->currPlayer){ //white player
		if(pawns[0] =='M' || pawns[1] =='M'){
			res = true;
		}
		else{
			res =  false;
		}
	}
	else{
		if((pawns[2] =='m') || (pawns[3] =='m')){
			res = true;
		}
		else{
			res = false;
		}
	}

	return res;
}

bool rookThreatLTR(int row, int col, char op_r,char op_q  , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	bool ok = false;
	for(int i = 1 ; i < MAX_COL ; i ++){
		tmp->y = pos->y+i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_r || game->board[tmp->x][tmp->y] == op_q)){
			ok =  true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return ok;
}

bool rookThreatRTL(int row, int col, char op_r,char op_q , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_COL ; i ++){
		tmp->y = pos->y-i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_r || game->board[tmp->x][tmp->y] == op_q)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return false;
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return false;
}


bool rookThreatTopDown(int row, int col, char op_r,char op_q  , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = pos->x-i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_r || game->board[tmp->x][tmp->y] == op_q)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return false;
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return false;
}

bool rookThreatBottomUp(int row, int col, char op_r,char op_q , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = pos->x+i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_r || game->board[tmp->x][tmp->y] == op_q)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return false;
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return false;
}


bool rookAndQueenThreat(POSITION king , GAME *game){
	int row = king.x;
	int col = king.y;
	char op_r = (game->currPlayer) == 1 ? 'R' : 'r';
	char op_q = (game->currPlayer) == 1 ? 'Q' : 'q';

	return (rookThreatBottomUp(row,col,op_r,op_q,game) ||
			rookThreatTopDown(row,col,op_r,op_q,game) ||
			rookThreatRTL(row,col,op_r,op_q,game) ||
			rookThreatLTR(row,col,op_r,op_q,game));
}

bool knightThreat(POSITION king , GAME *game){
	int row = king.x;
	int col = king.y;
	bool ok = false;
	char op_knight = (game->currPlayer) == 1 ? 'N' : 'n';
	POSITION* threats[8]={createPosition(row+1,col+2),
						 createPosition(row+1,col-2),
						 createPosition(row+2,col+1),
						 createPosition(row+2,col-1),
						 createPosition(row-1,col+2),
						 createPosition(row-1,col-2),
						 createPosition(row-2,col+1),
						 createPosition(row-2,col-1)};

	for(int i = 0 ; i < 8 ; i++){
		if(validPosition((threats[i])) && game->board[threats[i]->x][threats[i]->y] == op_knight){
			ok= true;
		}
		destroyPosition(threats[i]);
	}
	return ok;
}


bool kingThreat(POSITION king , GAME *game){
	int row = king.x;
	int col = king.y;
	bool ok = false;
	char op_king = (game->currPlayer) == 1 ? 'K' : 'k';
	POSITION* threats[8]={createPosition(row+1,col+1),
						 createPosition(row+1,col),
						 createPosition(row,col+1),
						 createPosition(row-1,col-1),
						 createPosition(row-1,col),
						 createPosition(row,col-1),
						 createPosition(row+1,col-1),
						 createPosition(row-1,col+1)};

	for(int i = 0 ; i < 8 ; i++){
		if(validPosition((threats[i])) && game->board[threats[i]->x][threats[i]->y] == op_king){
			ok = true;
		}
		destroyPosition(threats[i]);
	}
	return ok;
}

bool bishopThreatTopLeftDiag(int row, int col, char op_b,char op_q , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = pos->x+i;
		tmp->y =pos->y-i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_b || game->board[tmp->x][tmp->y] == op_q)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return false;
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return false;
}

bool bishopThreatTopRightDiag(int row, int col, char op_b,char op_q , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = pos->x+i;
		tmp->y =pos->y+i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_b || game->board[tmp->x][tmp->y] == op_q)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return false;
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return false;
}

bool bishopThreatBottomLeftDiag(int row, int col, char op_b,char op_q , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = pos->x-i;
		tmp->y =pos->y-i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_b || game->board[tmp->x][tmp->y] == op_q)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return false;
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return false;
}

bool bishopThreatBottomRightDiag(int row, int col, char op_b,char op_q , GAME *game){
	POSITION* pos = createPosition(row,col);
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = pos->x-i;
		tmp->y =pos->y+i;
		if(validPosition(tmp) && (game->board[tmp->x][tmp->y] == op_b || game->board[tmp->x][tmp->y] == op_q)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return true;
		}
		else if(validPosition(tmp) && (game->board[tmp->x][tmp->y] != EMPTY)){
			destroyPosition(tmp);
			destroyPosition(pos);
			return false;
		}
	}
	destroyPosition(tmp);
	destroyPosition(pos);
	return false;
}

bool bishopAndQueenThreat(POSITION king , GAME *game){
	int row = king.x;
	int col = king.y;
	char op_b = (game->currPlayer) == 1 ? 'B' : 'b';
	char op_q = (game->currPlayer) == 1 ? 'Q' : 'q';
	return (bishopThreatTopLeftDiag(row,col,op_b,op_q,game) ||
			bishopThreatTopRightDiag(row,col,op_b,op_q,game) ||
			bishopThreatBottomLeftDiag(row,col,op_b,op_q,game) ||
			bishopThreatBottomRightDiag(row,col,op_b,op_q,game));
}

bool kingThreatened(POSITION king , GAME *game){
	if(bishopAndQueenThreat(king,game) ||
	   kingThreat(king,game) ||
	   knightThreat(king,game)||
	   rookAndQueenThreat(king,game)||
	   pawnThreat(king,game)){
		return true;
	}
	else{
		return false;
	}
}

bool playerInCheck(GAME* game){
	char king = game->currPlayer==1 ? 'k' : 'K' ;
	POSITION*  king_pos = findPiece(king,game->board);
	bool ok = false;
	if(kingThreatened(*king_pos,game)){
		ok = true;
	}
	destroyPosition(king_pos);
	return ok;
}


void switchPlayers(GAME* game){
	int tmp = game->currPlayer;
	if(tmp == 1){
		game->currPlayer = 0;
	}
	else{
		game->currPlayer = 1;
	}

}

void makeMove(GAME* game, MOVE* mv){

	int srcx = mv->src.x;
	int srcy = mv->src.y;
	int dstx = mv->dst.x;
	int dsty = mv->dst.y;
	char p = getPiece(game,mv);
	if(game->board[dstx][dsty]!=EMPTY){
		updateArrays(game,game->board[dstx][dsty],-1);
	}

	(game->board)[srcx][srcy]= EMPTY;
	(game->board)[dstx][dsty]= p;

    if (spArrayListIsFull(game->gameArrayList) == true){
        spArrayListRemoveFirst(game->gameArrayList);
        spArrayListAddLast(game->gameArrayList, *mv);
    }
    else{
    	spArrayListAddLast(game->gameArrayList,*mv);
    }

	switchPlayers(game);




}



bool kingDangeredAfterMove(GAME* game, MOVE* mv){
	GAME* tmp = copyGame(game);
	makeMove(tmp,mv);
	switchPlayers(tmp);
	if(playerInCheck(tmp)){
		destroyGame(tmp);
		return true;
	}
	else{
		destroyGame(tmp);
		return false;
	}
}

MOVE defaultMove(){
	POSITION src;
	POSITION dst;
	src.x = src.y = dst.x = dst.y = -1;
	MOVE mv;
	mv.dst = dst;
	mv.src = src;
	mv.piece = EMPTY;
	mv.piece_dst = EMPTY;
	return mv;
}

MOVE_LIST* appendMoveLists(MOVE_LIST* a, MOVE_LIST* b ){
	MOVE_LIST* res = a;
	if(a== NULL){
		return b;
	}
	else if(b ==NULL){
		return a;
	}

	while(a->next!=NULL){
		a = a->next;
	}
	a->next = b;
	return res;

}


SPArrayList* getAllPieceMoves(GAME* game, POSITION* piece_pos){
	if(game == NULL || piece_pos ==NULL){
		return NULL;
	}
	else if(!validPosition(piece_pos)){
		return NULL;
	}
	else if(game->board[piece_pos->x][piece_pos->y]== EMPTY){
		return NULL;
	}
    SPArrayList* moves = spArrayListCreate(MAX_POS_MOVES); // change to 27
    if (moves == NULL){
    	return NULL;
    }

    for (int i = 0 ; i < MAX_ROW ; i++){
    	for(int j = 0 ; j < MAX_COL ; j++){
    		POSITION* dst = createPosition(i,j);
    		MOVE* mv = createMove(*piece_pos,*dst,game->board[piece_pos->x][piece_pos->y],game->board[dst->x][dst->y]);
    		if(legalMove(game,mv)){
    			spArrayListAddLast(moves,*mv);
    		}
    		destroyPosition(dst);
    		destroyMove(mv);
    	}
    }
    return moves;


}


MOVE_LIST* getKingMoves(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* pos[8]={createPosition(row-1,col-1),
			          createPosition(row-1,col),
					  createPosition(row-1,col+1),
					  createPosition(row,col-1),
					  createPosition(row,col+1),
					  createPosition(row+1,col),
					  createPosition(row+1,col-1),
					  createPosition(row+1,col+1)
	};

	for(int i = 0; i < 8 ; i++){
		if(!validPosition(pos[i])){
			destroyPosition(pos[i]);
			continue;
		}
		piece_dst = game->board[(*pos[i]).x][(*pos[i]).y];
		MOVE* mv = createMove(*piece_pos,*pos[i],piece,piece_dst);
		if(legalMove(game,mv)){
			head->next = createMoveList(*mv,NULL);
			head = head->next;
		}

		destroyPosition(pos[i]);
		destroyMove(mv);
	}

	MOVE_LIST* tmpm = res->next;
	res->next = NULL;
	free(res);
	return tmpm;


}




MOVE_LIST* rookMoveTopDown(GAME* game, POSITION* piece_pos){

	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];

	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = piece_pos->x-i;
		if(validPosition(tmp)){
			piece_dst = game->board[tmp->x][tmp->y];
			MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
			if(legalMove(game,mv)){
				head->next = createMoveList(*mv,NULL);
				head = head->next;
			}
			destroyMove(mv);
		}

	}
	destroyPosition(tmp);

	MOVE_LIST* tmpm = res->next;
	res->next = NULL;
	free(res);
	return tmpm;
}

MOVE_LIST* rookMoveBottomUp(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_ROW ; i ++){
		tmp->x = piece_pos->x+i;
		piece_dst = game->board[tmp->x][tmp->y];
		MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
		if(legalMove(game,mv)){
			head->next = createMoveList(*mv,NULL);
			head = head->next;
		}
		destroyMove(mv);

	}
	destroyPosition(tmp);

	MOVE_LIST* tmpm = res->next;
	res->next = NULL;
	free(res);
	return tmpm;
}


MOVE_LIST* rookMoveLeftRight(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_COL ; i ++){
		tmp->y = piece_pos->y+i;
		if(validPosition(tmp)){
			piece_dst = game->board[tmp->x][tmp->y];
			MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
			if(legalMove(game,mv)){
				head->next = createMoveList(*mv,NULL);
				head = head->next;
			}
			destroyMove(mv);
			}

	}
	destroyPosition(tmp);

	MOVE_LIST* tmpm = res->next;
	res->next = NULL;
	free(res);
	return tmpm;
}



MOVE_LIST* rookMoveRightLeft(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* tmp = createPosition(row,col);
	for(int i = 1 ; i < MAX_COL ; i ++){
		tmp->y = piece_pos->y-i;
		if(validPosition(tmp)){
		piece_dst = game->board[tmp->x][tmp->y];
		MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
		if(legalMove(game,mv)){
			head->next = createMoveList(*mv,NULL);
			head = head->next;
		}
		destroyMove(mv);
		}
	}
	destroyPosition(tmp);
	MOVE_LIST* tmpm = res->next;
	res->next = NULL;
	free(res);
	return tmpm;
}

MOVE_LIST* getRookMoves(GAME* game, POSITION* piece_pos){
	MOVE_LIST* res;
	res = rookMoveBottomUp(game,piece_pos);
	res = appendMoveLists(rookMoveTopDown(game,piece_pos),res);
	res = appendMoveLists(rookMoveLeftRight(game,piece_pos),res);
	res = appendMoveLists(rookMoveRightLeft(game,piece_pos),res);
	return res;
}

MOVE_LIST* getBishopMovesUpRight(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* tmp = createPosition(row,col);
		for(int i = 1 ; i < MAX_COL ; i ++){
			tmp->x = piece_pos->x+i;
			tmp->y = piece_pos->y+i;
			piece_dst = game->board[tmp->x][tmp->y];
			MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
			if(legalMove(game,mv)){
				head->next = createMoveList(*mv,NULL);
				head = head->next;
			}
			destroyMove(mv);

		}
		destroyPosition(tmp);

		MOVE_LIST* tmpm = res->next;
		res->next = NULL;
		free(res);
		return tmpm;

}

MOVE_LIST* getBishopMovesUpLeft(GAME* game, POSITION* piece_pos){

	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* tmp = createPosition(row,col);
		for(int i = 1 ; i < MAX_COL ; i ++){
			tmp->x = piece_pos->x+i;
			tmp->y = piece_pos->y-i;
			piece_dst = game->board[tmp->x][tmp->y];
			MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
			if(legalMove(game,mv)){
				head->next = createMoveList(*mv,NULL);
				head = head->next;
			}
			destroyMove(mv);

		}
		destroyPosition(tmp);

		MOVE_LIST* tmpm = res->next;
		res->next = NULL;
		free(res);
		return tmpm;
}


MOVE_LIST* getBishopMovesDownRight(GAME* game, POSITION* piece_pos){

	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* tmp = createPosition(row,col);
		for(int i = 1 ; i < MAX_COL ; i ++){
			tmp->x = piece_pos->x-i;
			tmp->y = piece_pos->y+i;
			if(validPosition(tmp)){
			piece_dst = game->board[tmp->x][tmp->y];
			MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
			if(legalMove(game,mv)){
				head->next = createMoveList(*mv,NULL);
				head = head->next;
			}
			destroyMove(mv);
			}
		}
		destroyPosition(tmp);

		MOVE_LIST* tmpm = res->next;
		res->next = NULL;
		destroyMoveList(res);
		return tmpm;

}

MOVE_LIST* getBishopMovesDownLeft(GAME* game, POSITION* piece_pos){

	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* tmp = createPosition(row,col);
		for(int i = 1 ; i < MAX_COL ; i ++){
			tmp->x = piece_pos->x-i;
			tmp->y = piece_pos->y-i;
			if(validPosition(tmp)){
			piece_dst = game->board[tmp->x][tmp->y];
			MOVE* mv = createMove(*piece_pos,*tmp,piece,piece_dst);
			if(legalMove(game,mv)){
				head->next = createMoveList(*mv,NULL);
				head = head->next;
			}

			destroyMove(mv);
			}
		}
		destroyPosition(tmp);

		MOVE_LIST* tmpm = res->next;
		res->next = NULL;
		free(res);
		return tmpm;
}

MOVE_LIST* getBishopMoves(GAME* game, POSITION* piece_pos){
	MOVE_LIST* res;
	res = getBishopMovesUpRight(game,piece_pos);
	res = appendMoveLists(getBishopMovesUpLeft(game,piece_pos),res);
	res = appendMoveLists(getBishopMovesDownRight(game,piece_pos),res);
	res = appendMoveLists(getBishopMovesDownLeft(game,piece_pos),res);
	return res;
}


MOVE_LIST* getQueenMoves(GAME* game, POSITION* piece_pos){
	MOVE_LIST* res;
	res = getBishopMoves(game,piece_pos);
	res = appendMoveLists(getRookMoves(game,piece_pos),res);
	return res;
}

MOVE_LIST* getPawnMoves(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* pos[]={createPosition(row+1,col), // white pawn up
						 createPosition(row+1,col+1), // white pawn diag
						 createPosition(row+1,col-1), // whit pawn diag
						 createPosition(row-1,col),	// black pawn down
						 createPosition(row-1,col+1), // black pawn diag
						 createPosition(row-1,col-1), // black pawn diag
						 createPosition(row+2,col),  //white pawn first step
						 createPosition(row-2,col)}; //black pawn first step
	for(int i = 0; i < 8 ; i++){
		if(!validPosition(pos[i])){
			destroyPosition(pos[i]);
			continue;
		}
		piece_dst = game->board[(*pos[i]).x][(*pos[i]).y];
		POSITION p;
		p.x = pos[i]->x;
		p.y = pos[i]->y;
		MOVE* mv = createMove(*piece_pos,p,piece,piece_dst);
		if(legalMove(game,mv)){
			head->next = createMoveList(*mv,NULL);
			head = head->next;
		}
		destroyPosition(pos[i]);
		destroyMove(mv);
	}

	MOVE_LIST* tmp = res->next;
	res->next = NULL;
	destroyMoveList(res);
	return tmp;
}

MOVE_LIST* getKnightMoves(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* head = createMoveList(m,NULL);
	MOVE_LIST* res = head;
	char piece_dst = EMPTY;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	POSITION* pos[8]={createPosition(row-2,col-1),
					  createPosition(row-2,col+1),
					  createPosition(row-1,col-2),
					  createPosition(row-1,col+2),
					  createPosition(row+1,col-2),
					  createPosition(row+1,col+2),
					  createPosition(row+2,col-1),
					  createPosition(row+2,col+1)

						 };

	for(int i = 0; i < 8 ; i++){
		if(!validPosition(pos[i])){
			destroyPosition(pos[i]);
			continue;
		}
		piece_dst = game->board[(*pos[i]).x][(*pos[i]).y];
		MOVE* mv = createMove(*piece_pos,*pos[i],piece,piece_dst);
		if(legalMove(game,mv)){
			head->next = createMoveList(*mv,NULL);
			head = head->next;
		}
		destroyPosition(pos[i]);
		destroyMove(mv);
	}

	MOVE_LIST* tmp = res->next;
	res->next = NULL;
	free(res);
	return tmp;
}

void printListOfMoves(MOVE_LIST* res,GAME* game){
	char* threat = "";
	char* eat = "";

	if(res == NULL){
		printf("No moves\n");
	}
	while(res!=NULL){
		if(kingThreatened((res->mv.dst),game)){
			threat = "*";
		}
		if(res->mv.piece_dst!=EMPTY){
			eat="^";
		}

		printf("<%d,%c>%s%s\n", (res->mv.dst.x)+1,(char)(res->mv.dst.y + 'A'),threat,eat);
		threat = "";
		eat="";
		res = res->next;
	}
}

MOVE_LIST* getPieceMoves(GAME* game, POSITION* piece_pos){
	MOVE_LIST* res = NULL;
	int row=piece_pos->x;
	int col=piece_pos->y;
	char piece = game->board[row][col];
	switch (piece){

			case 'm':
			res = getPawnMoves(game,piece_pos);
			break;

			case 'M':
			res = getPawnMoves(game,piece_pos);

			break;

			case 'b':
			res = getBishopMoves(game,piece_pos);
			break;

			case 'B':
			res = getBishopMoves(game,piece_pos);
			break;

			case 'r':
			res = getRookMoves(game,piece_pos);
			break;

			case 'R':
			res = getRookMoves(game,piece_pos);
			break;

			case 'n':
			res = getKnightMoves(game,piece_pos);
			break;

			case 'N':
			res = getKnightMoves(game,piece_pos);
			break;

			case 'q':
			res = getQueenMoves(game,piece_pos);
			break;

			case 'Q':
			res = getQueenMoves(game,piece_pos);
			break;

			case 'k':
			res = getKingMoves(game,piece_pos);
			break;

			case 'K':
			res = getKingMoves(game,piece_pos);
			break;

			default:
				res = NULL;
				break;

			}
	return res;
}




MOVE_LIST* getAllMovesForPlayer(GAME* game){
	char* pieces = game->currPlayer ? "mnrbqk": "MNRBQK";
	MOVE_LIST* res = NULL;

	for(int i = 0 ; i < MAX_ROW ; i++){
		for(int j = 0 ; j < MAX_COL ; j++){

			if(game->board[i][j]==EMPTY){
				continue;
			}
			else if((strchr(pieces,game->board[i][j])!='\0')){
				POSITION* new_pos = createPosition(i,j);
				res = appendMoveLists(res,getPieceMoves(game,new_pos));
				if((res!=NULL) && (res->mv.src.x == -1)){
					res = res->next;
				}
				destroyPosition(new_pos);
			}

		}

	}
	return res;
}

bool isCheckMate2(GAME* game){
	SPArrayList* mv;
	bool ok = false;

	for(int i = 0 ; i < MAX_ROW ; i++){
		for(int j = 0 ; j < MAX_COL ; j++){
			 if ((game->currPlayer && islower(game->board[i][j])) || (!(game->currPlayer) && isupper(game->board[i][j]))){
				 POSITION* pos = createPosition(i,j); // [TODO] free
				 mv =getAllPieceMoves(game,pos);
				 destroyPosition(pos);
				 if(mv!=NULL){
					 spArrayListDestroy(mv);
					 return false;
				 }
			 }
		}
	}

	if((mv == NULL) && (playerInCheck(game))){
		ok= true;
	}
	else{
		ok = false;
	}
	spArrayListDestroy(mv);
	return ok;
}

bool isCheckMate(GAME* game){
	MOVE_LIST* mv =getAllMovesForPlayer(game);
	bool ok = false;
	if((mv == NULL) && (playerInCheck(game))){
		ok= true;
	}
	else{
		ok = false;
	}
	destroyMoveList(mv);
	return ok;
}

bool isTie(GAME* game){
	MOVE_LIST* mv = getAllMovesForPlayer(game);
	bool ok =false;
	if((mv == NULL) && (!playerInCheck(game))){
		ok= true;
	}
	else{
		ok= false;
	}
	destroyMoveList(mv);
	return ok;
}

bool existsGameWinner2(GAME* game){
	GAME* copy = copyGame(game);
	if(isCheckMate(copy)){
		destroyGame(copy);
		return true;
	}
	switchPlayers(copy);
	if(isCheckMate(copy)){
		destroyGame(copy);
		return true;
	}
	destroyGame(copy);
	return false;

}

int existsGameWinner(GAME* game){
	GAME* copy = copyGame(game);
	if(isCheckMate(copy)){
		destroyGame(copy);
		return -1;
	}
	switchPlayers(copy);
	if(isCheckMate(copy)){
		destroyGame(copy);
		return 1;
	}
	destroyGame(copy);
	return 0;

}



void printMoveList(GAME* game, POSITION* piece_pos){
	MOVE m = defaultMove();
	MOVE_LIST* res = createMoveList(m,NULL);
	res = getPieceMoves(game,piece_pos);

	if(res == NULL){
		printf("No moves\n");
	}
	while(res!=NULL){
		printf("Move is: from <%d,%d> to <%d,%d>\n", res->mv.src.x,res->mv.src.y,res->mv.dst.x,res->mv.dst.y);
		res = res->next;
	}
}

int compareMovesForMVV(MOVE_LIST* a , MOVE_LIST* b){
	char* pieces = "pnbrq";
	if(a->mv.piece_dst == EMPTY){
		return 0;
	}
	else{
		if(strchr(pieces,a->mv.piece_dst) > strchr(pieces,b->mv.piece_dst)){
			return -1;
		}
	}
	return 1;
}


/*
 * TEST SUITE
 *
 *
 *
 */

void printMoveListTest(){
	GAME* game = createGame(6);
		initGame(game);

		char b[MAX_ROW][MAX_COL] = 	   {{'r','n','b','q','k','b','n','r'},
										{'m','m','m','m','m','m','m','m'},
										{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
										{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'B'},
										{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
										{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
										{'M','M','M','M','M','M','M','M'},
										{'R','N','B','Q','K','B','N','R'}
										};


		for(int i = 0 ; i < 8 ; i++){
			for(int j = 0 ; j < 8 ; j++){
				game->board[i][j] = b[i][j];
			}
		}
		//game->currPlayer = 0;
		POSITION* piece_pos = createPosition(1,5);

		printMoveList(game,piece_pos);

}

//TESTED OK
void getPieceTest(){
	GAME* game = createGame(6);
	initGame(game);

	for(int i = 0 ; i < 8 ; i++){
		MOVE* mv = createMove(*(createPosition(i,0)),*(createPosition(i,0)),EMPTY,EMPTY);
		char c = getPiece(game,mv);
		printf("Piece is: %c\n",c);
	}

}

//TESTED OK
void getToSquareTest(){
	GAME* game = createGame(6);
	initGame(game);

	for(int i = 0 ; i < 8 ; i++){
		MOVE* mv = createMove(*(createPosition(i,0)),*(createPosition(0,i)),EMPTY,EMPTY);
		char c = getToSquare(game,mv);
		printf("Piece is: %c\n",c);
	}
}

//TESTED OK
void findPieceTest(){
	GAME* game = createGame(6);
	initGame(game);
	char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
							   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
							   {EMPTY,EMPTY,EMPTY,EMPTY,'q',EMPTY,EMPTY,EMPTY},
							   {EMPTY,'N',EMPTY,'Q',EMPTY,EMPTY,EMPTY,EMPTY},
						   	   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
							   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'k',EMPTY,EMPTY},
							   {EMPTY,'n',EMPTY,EMPTY,'K',EMPTY,EMPTY,EMPTY},
							   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}
							  };
	char* str = "mMnNqQkK";
	for(int i = 0 ; i < 8 ; i++){
		POSITION* pos = createPosition(-1,-1);
		pos = findPiece(str[i],b);
		printf("Piece is: %c, position is <%d,%d>\n",str[i],pos->x,pos->y);
	}

}

//TESTED OK
void myPieceTest(){
	GAME* game = createGame(6);
	initGame(game);

	for(int i = 0 ; i < 8 ; i++){
		bool ans = false;
		MOVE* mv = createMove(*(createPosition(0,i)),*(createPosition(0,i)),EMPTY,EMPTY);
		ans = myPiece(game,mv);
		if(ans){
		}
		else{
		}

	}

}

//TESTED OK
void legalDestinationTest(){
	GAME* game = createGame(6);
	initGame(game);

	for(int i = 0 ; i < 8 ; i++){
		bool ans = false;
		MOVE* mv = createMove(*(createPosition(6,i)),*(createPosition(5,i)),EMPTY,EMPTY);
		ans = legalDestination(game,mv);
		if(ans){
			printf("True\n");
		}
		else{
			printf("False\n");
		}

	}
}

//TESTED OK
void legalPawnMoveTest(){
	GAME* game = createGame(6);
		initGame(game);
		char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
								   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,EMPTY},
								   {EMPTY,'N','n','Q',EMPTY,EMPTY,EMPTY,EMPTY},
							   	   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'k',EMPTY,EMPTY},
								   {EMPTY,'n',EMPTY,EMPTY,'K',EMPTY,EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}
								  };
		for(int i = 0 ; i < 8 ; i++){
			for(int j = 0 ; j < 8 ; j++){
				game->board[i][j] = b[i][j];
			}
		}


		char* test1 ="<2,F> to <3,F>"; //legal move
		char* test2 ="<3,C> to <4,B>"; // legal side move
		char* test3 ="<3,C> to <4,C>"; // illegal pawn move (override my piece)
		char* test4 ="<3,C> to <6,E>"; // illegal pawn move
		char* test5 = "<3,C> to <3,D>"; // illegal pawn move
		char* test6 = "<2,B> to <3,B>"; // illegal pawn move
		char * tokens[] = {test1,test2,test3,test4,test5,test6};


		for(int i = 0 ; i < 6 ; i++){
			MOVE* mv = parseMove(tokens[i]);
			//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
			bool ans = legalPawnMove(game,mv);
			destroyMove(mv);
			if(ans){
				printf("legal Move\n");
			}
			else{
				printf("illegal\n");
			}

		}
}

//TESTED OK
void legalKingMoveTest(){
	GAME* game = createGame(6);
		initGame(game);
		char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
								   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,EMPTY},
								   {EMPTY,'N',EMPTY,'Q',EMPTY,EMPTY,EMPTY,EMPTY},
							   	   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'k',EMPTY,EMPTY},
								   {EMPTY,'n',EMPTY,EMPTY,'K',EMPTY,EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}
								  };
		for(int i = 0 ; i < 8 ; i++){
			for(int j = 0 ; j < 8 ; j++){
				game->board[i][j] = b[i][j];
			}
		}


		char* test1 ="<6,F> to <7,F>"; //legal 'k' move
		char* test2 ="<6,F> to <5,F>"; // illegal 'k' move
		char* test3 ="<6,F> to <7,E>"; // legal 'k' move
		char* test4 ="<7,E> to <6,D>"; // legal 'K' move
		char* test5 = "<6,F> to <3,D>"; // illegal 'k' move
		char* test6 = "<7,E> to <3,B>"; // illegal pawn move
		char * tokens[] = {test1,test2,test3,test4,test5,test6};


		for(int i = 0 ; i < 6 ; i++){
			MOVE* mv = parseMove(tokens[i]);
			//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
			bool ans = legalKingMove(game,mv);
			destroyMove(mv);
			if(ans){
				printf("legal Move\n");
			}
			else{
				printf("illegal\n");
			}

		}
}

//TESTED OK
void legalRookMoveTest(){
	GAME* game = createGame(6);
		initGame(game);
		char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
								   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,EMPTY},
								   {'r','N',EMPTY,'Q',EMPTY,EMPTY,'R',EMPTY},
							   	   {'r',EMPTY,EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'k',EMPTY,EMPTY},
								   {EMPTY,'n',EMPTY,EMPTY,'K',EMPTY,EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}
								  };
		for(int i = 0 ; i < 8 ; i++){
			for(int j = 0 ; j < 8 ; j++){
				game->board[i][j] = b[i][j];
			}
		}


		char* test1 ="<4,A> to <1,A>"; //legal 'r' move upwards
		char* test2 ="<4,A> to <4,B>"; // legal 'r' move eating enemy
		char* test3 ="<5,A> to <5,E>"; // legal 'r' move right
		char* test4 ="<5,A> to <8,A>"; // legal 'r' move down
		char* test5 = "<5,A> to <5,F>"; // illegal 'r' move right my piece
		char* test6 = "<4,G> to <4,E>"; // legal 'R' move
		char* test7 = "<4,G> to <4,D>"; // illegal 'R' move
		char* test8 = "<4,G> to <8,G>"; // legal 'R' move
		char * tokens[] = {test1,test2,test3,test4,test5,test6,test7,test8};


		for(int i = 0 ; i < 8 ; i++){
			MOVE* mv = parseMove(tokens[i]);
			//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
			bool ans = legalRookMove(game,mv);
			destroyMove(mv);
			if(ans){
				printf("legal Move\n");
			}
			else{
				printf("illegal\n");
			}

		}
}

//TESTED OK
void legalBishopMoveTest(){
	GAME* game = createGame(6);
		initGame(game);
		char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
								   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,EMPTY},
								   {'r','N',EMPTY,'Q',EMPTY,EMPTY,'R',EMPTY},
							   	   {'r',EMPTY,EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'k',EMPTY,EMPTY},
								   {EMPTY,'n',EMPTY,'B','K',EMPTY,EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'b'}
								  };
		for(int i = 0 ; i < 8 ; i++){
			for(int j = 0 ; j < 8 ; j++){
				game->board[i][j] = b[i][j];
			}
		}


		char* test1 ="<8,H> to <1,A>"; //illegal 'b' blocked leftUp
		char* test2 ="<7,D> to <8,C>"; // legal 'B' move leftDown
		char* test3 ="<7,D> to <5,F>"; // legal 'B' move eating enemy 'm'
		char* test4 ="<7,D> to <4,A>"; // legal 'b' move eating enemy 'r
		char* test5 = "<8,H> to <6,F>"; // illegal 'b' move  my piece
		char* test6 = "<7,D> to <8,D>"; // illegal 'B' move
		char* test7 = "<8,H> to <1,H>"; // illegal 'b' move
		char* test8 = "<8,H> to <8,H>"; // illegal 'b' move
		char * tokens[] = {test1,test2,test3,test4,test5,test6,test7,test8};


		for(int i = 0 ; i < 8 ; i++){
			MOVE* mv = parseMove(tokens[i]);
			//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
			bool ans = legalBishopMove(game,mv);
			destroyMove(mv);
			if(ans){
				printf("legal Move\n");
			}
			else{
				printf("illegal\n");
			}

		}
}
//TESTED OK
void legalKnightMoveTest(){
	GAME* game = createGame(6);
		initGame(game);
		char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
								   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,EMPTY},
								   {'r','N',EMPTY,'Q',EMPTY,EMPTY,'R',EMPTY},
							   	   {'r',EMPTY,'R',EMPTY,EMPTY,'m',EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'k',EMPTY,EMPTY},
								   {EMPTY,'n',EMPTY,'B','K',EMPTY,EMPTY,EMPTY},
								   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'b'}
								  };
		for(int i = 0 ; i < 8 ; i++){
			for(int j = 0 ; j < 8 ; j++){
				game->board[i][j] = b[i][j];
			}
		}


		char* test1 ="<7,B> to <5,C>"; //legal 'n' enemy piece 'R'
		char* test2 ="<7,B> to <5,A>"; // illegal 'n' move my piece
		char* test3 ="<4,B> to <2,C>"; // legal 'N' move upright
		char* test4 ="<4,B> to <6,C>"; // legal 'N' move downright with block
		char* test5 = "<8,H> to <6,F>"; // illegal not a 'n' move
		char* test6 = "<7,D> to <6,F>"; // legal 'n' move, no knight in position but shouldn't fail this test

		char * tokens[] = {test1,test2,test3,test4,test5,test6};


		for(int i = 0 ; i < 6 ; i++){
			MOVE* mv = parseMove(tokens[i]);
			//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
			bool ans = legalKnightMove(game,mv);
			destroyMove(mv);
			if(ans){
				printf("legal Move\n");
			}
			else{
				printf("illegal\n");
			}

		}
}

//TESTED OK
void pawnThreatTest(){
	GAME* game = createGame(6);
			initGame(game);
			char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
									   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,EMPTY},
									   {'r','N',EMPTY,'Q',EMPTY,EMPTY,'R',EMPTY},
								   	   {'r',EMPTY,'R',EMPTY,'M','m',EMPTY,EMPTY},
									   {EMPTY,EMPTY,EMPTY,'m','m','k',EMPTY,EMPTY},
									   {EMPTY,'n',EMPTY,'B','K',EMPTY,EMPTY,EMPTY},
									   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'b'}
									  };
			for(int i = 0 ; i < 8 ; i++){
				for(int j = 0 ; j < 8 ; j++){
					game->board[i][j] = b[i][j];
				}
			}

			POSITION* test1 =createPosition(5,5); // white k
			POSITION* test2 =createPosition(6,4); //black K


			POSITION * tokens[] = {test1,test2};


			for(int i = 0 ; i < 2 ; i++){
				//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
				bool ans = pawnThreat(*(tokens[i]),game);
				game->currPlayer ^= 1;
				//destroyMove(mv);
				if(ans){
					printf("king is threatened\n");
				}
				else{
					printf("no threat\n");
				}

			}
}
//TESTED OK
void kingThreatTest(){
	GAME* game = createGame(6);
			initGame(game);
			char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
									   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,EMPTY},
									   {'r','N',EMPTY,'Q',EMPTY,EMPTY,'R',EMPTY},
								   	   {'r',EMPTY,'R',EMPTY,'M','m','k',EMPTY},
									   {EMPTY,EMPTY,EMPTY,'m','m',EMPTY,EMPTY,EMPTY},
									   {EMPTY,'n',EMPTY,'B','K',EMPTY,EMPTY,EMPTY},
									   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'b'}
									  };
			for(int i = 0 ; i < 8 ; i++){
				for(int j = 0 ; j < 8 ; j++){
					game->board[i][j] = b[i][j];
				}
			}

			POSITION* test1 =createPosition(4,6); // white k
			POSITION* test2 =createPosition(6,4); //black K


			POSITION * tokens[] = {test1,test2};


			for(int i = 0 ; i < 2 ; i++){
				//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
				bool ans = kingThreat(*(tokens[i]),game);
				game->currPlayer ^= 1;
				destroyPosition(tokens[i]);
				if(ans){
					printf("king is threatened\n");
				}
				else{
					printf("no threat\n");
				}

			}
}

//TESTED OK
void rookAndQueenThreatTest(){
	GAME* game = createGame(6);
				initGame(game);
				char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'Q',EMPTY},
										   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
										   {EMPTY,EMPTY,'m',EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
										   {'r','N',EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   	   {'r',EMPTY,'R',EMPTY,'M','m','k',EMPTY},
										   {EMPTY,EMPTY,EMPTY,'m','m',EMPTY,EMPTY,EMPTY},
										   {EMPTY,'n',EMPTY,'B','K',EMPTY,EMPTY,EMPTY},
										   {EMPTY,EMPTY,EMPTY,EMPTY,'r',EMPTY,EMPTY,'b'}
										  };
				for(int i = 0 ; i < 8 ; i++){
					for(int j = 0 ; j < 8 ; j++){
						game->board[i][j] = b[i][j];
					}
				}

				POSITION* test1 =createPosition(4,6); // white k
				POSITION* test2 =createPosition(6,4); //black K


				POSITION * tokens[] = {test1,test2};


				for(int i = 0 ; i < 2 ; i++){
					//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
					bool ans = rookAndQueenThreat(*(tokens[i]),game);
					game->currPlayer ^= 1;
					destroyPosition(tokens[i]);
					if(ans){
						printf("king is threatened\n");
					}
					else{
						printf("no threat\n");
					}

				}
}

//TESTED OK
void knightThreatTest(){
	GAME* game = createGame(6);
			initGame(game);
			char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
									   {EMPTY,EMPTY,'m',EMPTY,'q',EMPTY,EMPTY,'N'},
									   {'r','N',EMPTY,'Q',EMPTY,EMPTY,'R',EMPTY},
								   	   {'r',EMPTY,'R',EMPTY,'M','m','k',EMPTY},
									   {EMPTY,EMPTY,'n','m','m',EMPTY,EMPTY,EMPTY},
									   {EMPTY,'n',EMPTY,'B','K',EMPTY,EMPTY,EMPTY},
									   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'n',EMPTY,'b'}
									  };
			for(int i = 0 ; i < 8 ; i++){
				for(int j = 0 ; j < 8 ; j++){
					game->board[i][j] = b[i][j];
				}
			}

			POSITION* test1 =createPosition(4,6); // white k
			POSITION* test2 =createPosition(6,4); //black K


			POSITION * tokens[] = {test1,test2};


			for(int i = 0 ; i < 2 ; i++){
				//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
				bool ans = knightThreat(*(tokens[i]),game);
				game->currPlayer ^= 1;
				destroyPosition(tokens[i]);
				if(ans){
					printf("king is threatened\n");
				}
				else{
					printf("no threat\n");
				}

			}
}


void bishopAndQueenThreatTest(){
	GAME* game = createGame(6);
				initGame(game);
				char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,'Q',EMPTY,EMPTY,EMPTY,'Q',EMPTY},
										   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
										   {EMPTY,EMPTY,'m',EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
										   {'r','N',EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   	   {'r',EMPTY,'q',EMPTY,'M','m','k',EMPTY},
										   {EMPTY,EMPTY,EMPTY,'m','m',EMPTY,EMPTY,EMPTY},
										   {EMPTY,'n',EMPTY,'B','K',EMPTY,EMPTY,EMPTY},
										   {EMPTY,EMPTY,EMPTY,EMPTY,'r','q',EMPTY,'b'}
										  };
				for(int i = 0 ; i < 8 ; i++){
					for(int j = 0 ; j < 8 ; j++){
						game->board[i][j] = b[i][j];
					}
				}

				POSITION* test1 =createPosition(4,6); // white k
				POSITION* test2 =createPosition(6,4); //black K


				POSITION * tokens[] = {test1,test2};


				for(int i = 0 ; i < 2 ; i++){
					//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
					bool ans = bishopAndQueenThreat(*(tokens[i]),game);
					game->currPlayer ^= 1;
					destroyPosition(tokens[i]);
					if(ans){
						printf("king is threatened\n");
					}
					else{
						printf("no threat\n");
					}

				}
}


void kingThreatenedTest(){
	GAME* game = createGame(6);
				initGame(game);
				char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,'Q',EMPTY,EMPTY,EMPTY,'Q',EMPTY},
										   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
										   {EMPTY,EMPTY,'m',EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
										   {'r','N',EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   	   {'r',EMPTY,EMPTY,'q','M','m','k',EMPTY},
										   {EMPTY,EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY,EMPTY},
										   {EMPTY,'n',EMPTY,EMPTY,'K',EMPTY,EMPTY,EMPTY},
										   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'b'}
										  };
				for(int i = 0 ; i < 8 ; i++){
					for(int j = 0 ; j < 8 ; j++){
						game->board[i][j] = b[i][j];
					}
				}

				POSITION* test1 =createPosition(4,6); // white k
				POSITION* test2 =createPosition(6,4); //black K


				POSITION * tokens[] = {test1,test2};


				for(int i = 0 ; i < 2 ; i++){
					//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
					bool ans = kingThreatened(*(tokens[i]),game);
					game->currPlayer ^= 1;
					destroyPosition(tokens[i]);
					if(ans){
						printf("king is threatened\n");
					}
					else{
						printf("no threat\n");
					}

				}
}


void playerInCheckTest(){
	GAME* game = createGame(6);
			initGame(game);
			char b[MAX_ROW][MAX_COL]= {{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   {EMPTY,'M',EMPTY,EMPTY,EMPTY,'m',EMPTY,EMPTY},
									   {EMPTY,EMPTY,'m',EMPTY,'Q',EMPTY,EMPTY,EMPTY},
									   {EMPTY,'N','n','Q',EMPTY,EMPTY,EMPTY,EMPTY},
								   	   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,'k',EMPTY},
									   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									   {EMPTY,'n',EMPTY,EMPTY,'K',EMPTY,EMPTY,EMPTY},
									   {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}
									  };
			for(int i = 0 ; i < 8 ; i++){
				for(int j = 0 ; j < 8 ; j++){
					game->board[i][j] = b[i][j];
				}
			}
			playerInCheck(game);


}


//TESTED OK
void legalMoveTest(){
	GAME* game = createGame(6);
	initGame(game);

	char b[MAX_ROW][MAX_COL] = 	   {{'r','n','b','q','k','b','n','r'},
									{'m','m','m','m','m','m','m','m'},
									{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
									{'M','M','M','M','M','M','M','M'},
									{'R','N','B','Q','K','B','N','R'}
									};


	for(int i = 0 ; i < 8 ; i++){
		for(int j = 0 ; j < 8 ; j++){
			game->board[i][j] = b[i][j];
		}
	}

	char* test1 ="<1,G> to <3,F>"; //knight move
		char* test2 ="<1,G> to <3,H>"; // knight move
		char* test3 ="<2,D> to <3,D>"; // pawn move
		char* test4 ="<2,D> to <3,E>"; // illegal pawn move
		char* test5 = "<7,D> to <6,D>"; // illegal queen move
		char* test6 = "<A,1> to <A,1>"; // illegal non move
		char * tokens[] = {test1,test2,test3,test4,test5,test6};
		for (int i = 0 ; i < 6 ; i ++){
			//printf("%s\n",tokens[i]);
			MOVE* mv = parseMove(tokens[i]);
			//printf("Move is: from <%d,%d> to <%d,%d>\n", mv->src.x,mv->src.y,mv->dst.x,mv->dst.y);
			if(mv!=NULL && legalMove(game,mv)){
				printf("Move is legal!\n");


			}
			else{
			}
		}
}
