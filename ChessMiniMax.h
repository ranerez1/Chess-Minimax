/*
 * ChessMiniMaxNode.h
 *
 *  Created on: 28 Sep 2017
 *      Author: ranerez
 */

#ifndef CHESSMINIMAX_H_
#define CHESSMINIMAX_H_

/*
 * The function which executes the minimax algorithm with alpha beta pruning and outputs the best possible move according to the scores
 * or null in case no moves exists. the function receives;
 * @param - GAME* a pointer to the game
 * @param - int depth- the max recursion depth to look forward in the game
 * @ret - NULL if no moves exist or a pointer to a struct MOVE
 */
MOVE* getBestMove(GAME* currentGame, int depth);
#endif /* CHESSMINIMAX_H_ */
