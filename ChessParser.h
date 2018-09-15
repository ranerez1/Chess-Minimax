/*
 * ChessParser.h
 *
 *  Created on: 18 Aug 2017
 *      Author: ranerez
 */

#ifndef CHESSPARSER_H_
#define CHESSPARSER_H_
#include <stdbool.h>
#include "ChessParams.h"

/*
 * The ChessParser module is responsible for handling all user inputs and storing the commands
 */


/*
 * The SETTING structure holds the attributes for the setting
 * gameMode - an integer representing the game mode: 1 = 1 player , 2 =2 players
 * difficulty - integer representing the maximum moves the minimax algorithm will look forward. between 1-4
 * color- integer representing the user's color. 0- Black 1- White
 */
typedef struct set_t{
	int gameMode; //1 = 1 player , 2 =2 players
	int difficulty;
	int color;
}SETTING;


/*
 * SP_COMMAND is an ENUM to represent the command received from the user
 */
typedef enum {
	SP_INVALID_LINE,
	GAME_MODE,
	DIFFICULTY,
	USER_COLOR,
	LOAD,
	DEFAULT,
	PRINT_SETTING,
	QUIT,
	START,
	//GAME Commands
	SET_MOVE,
	GET_MOVES,
	SAVE,
	UNDO,
	RESET,


} SP_COMMAND;


/*
 * The POSITION struct holds the row and column of a position on a board (Zero Based)
 */
typedef struct pos_t{
	int x;
	int y;
}POSITION;

/*
 * The MOVE struct holds the necessary information for a move
 * POSITION src- the source of the move
 * POSITION dst - the destination of the move
 * char piece - the piece being moves
 * char piece_dst - The piece located in the destination position
 */

typedef struct move_t{
	POSITION src;
	POSITION dst;
	char piece;
	char piece_dst;
}MOVE;

/*
 * the SPCommand structs holds the received information for a command
 * SP_COMMAND cmd - an ENUM representing the command
 * bool validArg- represents if the argument inserted is valid
 * int arg- in case of an integer argument it stores the integer value of the input
 * char* str- in case the argument is a string
 * MOVE- a struct of the move. starts as an invalid move and updates according to the input
 */
typedef struct command_t {
	SP_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
	char* str;
	MOVE mv;
} SPCommand;


/*
 * Declarations
 */

/*
 * The main loop for the console operation.
 * responsible for receiving input and executing the relevant function
 */
void parser_main();

/*
 * Creates an SPCommand and allocates all required resources
 * @param - SP_COMMAND an ENUM representing the commnad
 * @param bool validArg to reperesent if the argument is valid
 * @param int - for the integer argument
 * @param char* for the string argument
 * @ret - SPCommand* a pointer to the created command
 */

SPCommand* createSPCommand(SP_COMMAND , bool , int , MOVE,char* );

/*
 * Destroys an SPCommand and frees all allocated resources
 * @param - SPCommand* - a pointer to the command being destroyed
 * @ret - void
 */
void destroySPCommand(SPCommand *);


/*
 * copies an SPCommand
 * @param - SPCommand* - a pointer to the command being copied from
 * @ret - SPCommand  - a pointer to the command created.
 */
SPCommand* copySPCommad(SPCommand* );

/*
 * Creates a setting and allocates all required resources
 * @param int - for the integer value of game mode
 * @param int - for the integer value of difficulty
 * @param int - for the integer value of user color
 * @ret - SETTING* a pointer to the created setting
 */
SETTING* createSetting(int,int,int);

/*
 * Destroys a SETTING and frees all allocated resources
 * @param - SETTING* - a pointer to the setting being destroyed
 * @ret - void
 */
void destroySetting(SETTING *set);


/*
 * Creates a position and allocates all required resources
 * @param int - for the zero based row (x) location
 * @param int - for the zero based column (y) location
 * @ret - POSITION* a pointer to the created position
 */

POSITION* createPosition(int,int);

/*
 * Destroys a POSITION and frees all allocated resources
 * @param - POSITION* - a pointer to the position being destroyed
 * @ret - void
 */
void destroyPosition(POSITION*);


/*
 * Creates a move and allocates all required resources
 * @param POSITION - the source position of the move
 * @param POSITION - the destination position of the move
 * @param char - the piece from the source move
 */
MOVE* createMove(POSITION,POSITION,char,char);
/*
 * Destroys a MOVE and frees all allocated resources
 * @param - MOVE* - a pointer to the move being destroyed
 * @ret - void
 */
void destroyMove(MOVE *);

/*
 * A function that receives a string and converts it to a valid move or returns NULL if the move is invalid
 * @param char* a string representing a move
 * @ret MOVE* a pointer to the created move
 */
MOVE* parseMove(char*);


/*
 * A function to check if the inserted line is a valid integer
 * @param const char* a string to be checked
 * @ret - true if integer false otherwise
 */
bool spParserIsInt(const char*);


/*
 * A function to receive user input and convert it into a command
 * @ret - SPCommand * a pointer to the created command
 */
SPCommand* getCommandFromUser();




/*
 * A safe strcpy function
 */
char* copyString(const char* );

#endif /* CHESSPARSER_H_ */
