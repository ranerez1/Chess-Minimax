
#include "ChessParams.h"
#include "ChessParser.h"
#include "stdio.h"
#include <ctype.h>
#include "ChessMoves.h"


/*
 * function that returns the maximum of 2 integers
 * @param int a, int b
 * @ret- int - the maximum between a,b
 */
int maximize(int a , int b){
	if (a>b){
		return a;
	}
	else{
		return b;
	}
}


/*
 * function that returns the minimum of 2 integers
 * @param int a, int b
 * @ret- int - the minimum between a,b
 */
int minimize(int a , int b){
	if (a<b){
		return a;
	}
	else{
		return b;
	}
}

int getPieceValue(char piece) {
	char p = tolower(piece);
    switch (p) {
        case EMPTY:
            return 0;
        case ('m'):
            return 1;
        case ('b'):
            return 3;
        case ('n'):
            return 3;
        case ('r'):
            return 5;
        case ('q'):
            return 9;
        case ('k'):
            return 100;
        default:
            return 0;
    }
}

/*
 * A function responsible for calculating a specific board and outputing an integer which is based on
 * the naive scoring function used to calculate a board by the number of pieces from each type and color
 * @param- GAME* - a pointer to the evaluated game
 * @ret - int- an integer of a score of the current board
 */

int calcScore(GAME *game) {
	int vector[] = {1,3,3,5,9,100};
    register int score = 0;
    	for(int i = 0 ; i < 6 ; i++){
    		if(game->currPlayer == 1){
    			score+= (((game->whiteArr)[i]*vector[i])-((game->blackArr)[i]*vector[i]));
    		}
    		else{
    			score+= (((game->blackArr)[i]*vector[i])-((game->whiteArr)[i]*vector[i]));
    		}

    	}
    return score;
}


/*
 * The recursive function to go over all possible moves at a given depth and give a score for each possible course of action
 * the function implements the minimax algorithm with alpha beta pruning to ensure fast running time for depth up to 4
 * The function receives the following parameters
 * 	*GAME* a pointer to the current game
 * 	int depth- the maximum recursion depth
 * 	int alpha and beta- the boundaries for the pruning
 * 	int flag- 0- for minimizing 1 - for maximizing
 */

int alphaBeta(GAME *src, int depth, int flag, int alpha, int beta) {
    int bestScore = flag ? INT_MIN: INT_MAX;
    SPArrayList* allMoves = NULL;
    char piece;
    int winner = 0;
    winner = existsGameWinner(src);
    if(winner!=0){
    		if(winner == 1){
    			return MAX_SCORE;
    		}
    		else{
    			return MIN_SCORE;
    		}
    	}


    if (depth == 0) {
        return calcScore(src);
    }

    MOVE move;
    if (flag == 1) {
        for (register int i = 0; i < MAX_ROW; i++) {
            for (register int j = 0; j < MAX_COL; j++) {
                piece = src->board[i][j];
                if ((src->currPlayer && islower(piece)) || (!(src->currPlayer) && isupper(piece))){
                	POSITION* pos = createPosition(i,j);
                    allMoves = getAllPieceMoves(src, pos);
                    destroyPosition(pos);
                    for (register int k = 0; k < allMoves->actualSize; k++) {
                        move = (spArrayListGetAt(allMoves, k));
                        makeMove(src,&move);

                        bestScore = maximize(bestScore, alphaBeta(src, depth - 1, !flag, alpha, beta));
                        undoMoveNoPrint(src,src->currPlayer);
                        alpha = maximize(bestScore, alpha);
                        if (beta <= alpha) {
                        	break;
                        }
                    }
                    spArrayListDestroy(allMoves);
                }
            }
        }
    }
    else {
        for (register int i = 0; i < MAX_ROW; i++) {
            for (register int j = 0; j < MAX_COL; j++) {
                piece = src->board[i][j];
                if ((src->currPlayer && islower(piece)) || (!(src->currPlayer) && isupper(piece))) {
                	POSITION* pos = createPosition(i,j);
                    allMoves = getAllPieceMoves(src, pos);
                    destroyPosition(pos);

                    for (register int k = 0; k < allMoves->actualSize; k++) {
                        move = (spArrayListGetAt(allMoves, k));
                        makeMove(src, &move);
                        bestScore = minimize(bestScore, alphaBeta(src, depth - 1, !flag, alpha, beta));
                        undoMoveNoPrint(src,src->currPlayer);
                        beta = minimize(bestScore, beta);
                        if (beta <= alpha) {
                        	break;
                        }
                    }
                    spArrayListDestroy(allMoves);
                }
            }
        }
    }
    return bestScore;
}




/*
 * The function which executes the minimax algorithm and outputs the best possible move according to the scores
 * or null in case no moves exists. the function receives;
 * @param - GAME* a pointer to the game
 * @param - int depth- the max recursion depth to look forward in the game
 * @ret - NULL if no moves exist or a pointer to a struct MOVE
 */
MOVE* getBestMove(GAME* game, int depth){
    if (game == NULL || depth <= 0 || depth > 4){
        return NULL;
    }

    if(existsGameWinner(game)!=0 || isTie(game)){
    	return NULL;
    }
    GAME* copy = NULL;
    copy = copyGame(game);
    if (copy == NULL){
    	return NULL;
    }
    MOVE best;
    int bestScore = 0;
	int score = 0;
    SPArrayList* allMoves = NULL;
    char piece;


    MOVE move;

        bestScore = INT_MIN;
        for (int i = 0; i < MAX_ROW; i++) {
            for (int j = 0; j < MAX_COL; j++) {
                piece = copy->board[i][j];
                if ((copy->currPlayer && islower(piece)) || (!(copy->currPlayer) && isupper(piece))) {
                	POSITION* pos = NULL;
                	pos = createPosition(i,j);
                    allMoves = getAllPieceMoves(copy, pos);
                    destroyPosition(pos);
                    for (int k = 0; k < allMoves->actualSize; k++) {
                        move = spArrayListGetAt(allMoves, k);
                        makeMove(copy, &move);
                        score = alphaBeta(copy, depth - 1, 0, INT_MIN, INT_MAX);
                        if (bestScore < score) {
                            best = move;
                            bestScore = score;
                        }
                        undoMoveNoPrint(copy,copy->currPlayer);
                    }
                    spArrayListDestroy(allMoves);
                }
            }
        }


    destroyGame(copy);
    MOVE* bestMove = NULL;
    bestMove = createMove(best.src,best.dst,best.piece,best.piece_dst);
    return bestMove;

}



