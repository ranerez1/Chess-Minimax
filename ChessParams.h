/*
 * ChessParams.h
 *
 *  Created on: 16 Sep 2017
 *      Author: ranerez
 */

#ifndef CHESSPARAMS_H_
#define CHESSPARAMS_H_

/*
 * Constants
 */

#define SP_MAX_LINE_LENGTH 1024
#define DEF_COL 1
#define DEF_DIFF 2
#define DEF_MODE 1
#define MAX_COL 8
#define MAX_ROW 8
#define HISTORY_SIZE 6
#define MAX_SCORE 1000
#define MIN_SCORE -1000
#define INT_MIN -100000
#define INT_MAX 100000
#define EMPTY '_'
#define NUM_PIECES 6
#define ALL_PIECES 16
#define MAX_POS_MOVES 27


/*
 * Messages for printing
 */

//ERROR MESSAGES
#define	E_GAME_MODE "Wrong game mode\n"
#define	E_DIFF_EXP "Expert level not supported, please choose a value between 1 to 4:\n"
#define E_DIFF_LVL "Wrong difficulty level. The value should be between 1 to 5\n"
#define E_FOPEN	"Error: File doesn’t exist or cannot be opened\n"
#define E_FCREATE "File cannot be created or modified\n"
#define E_MOVE_POS	"Invalid position on the board\n"
#define E_MOVE_PIECE "The specified position does not contain your piece\n"
#define E_MOVE_LGL "Illegal move\n"
#define E_UNDO_2P "Undo command not available in 2 players mode\n"
#define E_UNDO_EMPTY "Empty history, move cannot be undone\n"
#define E_GET_MOVE_PIECE_B "The specified position does not contain black player piece\n"
#define E_GET_MOVE_PIECE_W "The specified position does not contain white player piece\n"
#define E_INV_CM "ERROR: Invalid Command\n"

//CONSOLE MESSAGES
#define C_INIT "Specify game setting or type 'start' to begin a game with the current setting:\n"
#define C_SET_1P "Game mode is set to 1 player\n"
#define C_SET_2P "Game mode is set to 2 players\n"
#define C_QUIT	"Exiting...\n"
#define C_RESET "Restarting...\n"
#define C_WP_TURN	"white player - enter your move:\n"
#define C_BP_TURN	"black player - enter your move:\n"

//GAME MESSAGES
#define G_CHECK_B "Check: black King is threatened!\n"
#define G_CHECK_W "Check: white King is threatened!\n"
#define G_CHECK_AI "Check!\n"
#define G_CM_B "Checkmate! black player wins the game\n"
#define G_CM_W "Checkmate! white player wins the game\n"
#define G_TIE "The game is tied\n"
#define G_TIE_AI "The game ends in a tie\n"


#endif /* CHESSPARAMS_H_ */
