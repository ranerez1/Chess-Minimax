/*
 * ChessParser.c
 *
 *  Created on: 18 Aug 2017
 *      Author: ranerez
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "ChessParser.h"
#include "ChessUtils.h"
#include "ChessIO.h"
#include "ChessParams.h"

int gameFlag = 0;
int printB = 1;

char* copyString(const char* str){
	if (!str){
		return NULL;
	}

	unsigned int length = strlen(str) + 1;
	char* res = malloc(sizeof(char)*(length));
	if (res == NULL){
		printf("Error: copyString has failed");
		return NULL;
	}
	strcpy (res,str);
	res[length-1] = '\0';
	return res;

}


void destroySPCommand(SPCommand * command){
	if (command == NULL){
		return;
	}
	free(command);
}


POSITION *createPosition(int x, int y){
	POSITION * res = NULL;
	res = (POSITION*)malloc(sizeof(POSITION));
	if (res == NULL){
		printf("Error: createPosition has failed");
		free(res);
		return NULL;
	}
	res->x = x;
	res->y =y;
	return res;
}

void destroyPosition(POSITION *pos){
	if(pos==NULL){
		return;
	}
	free(pos);
}

void destroyMove(MOVE* mv){
	if(mv ==NULL){
		return;
	}
	free(mv);
}

MOVE* createMove(POSITION src, POSITION dst, char piece,char piece_dst){
	MOVE* res = NULL;
	res = (MOVE*)malloc(sizeof(MOVE));
	if (res == NULL){
		printf("Error: createMove has failed");
		free(res);
		return NULL;
	}
	res->dst = dst;
	res->src =src;
	res->piece = piece;
	res->piece_dst =piece_dst;
	return res;
}

MOVE* copyMove(MOVE* mv){
	MOVE* res = createMove(mv->src,mv->dst,mv->piece,mv->piece_dst);
	return res;
}

SPCommand* createSPCommand(SP_COMMAND cmd, bool ValidArg, int arg, MOVE mv, char* str){
	SPCommand* res = NULL;
	char str_res[SP_MAX_LINE_LENGTH];
	strcpy(str_res,str);
	unsigned int x = strlen(str);
	str_res[x]= '\0';
	res = (SPCommand*)malloc(sizeof(SPCommand));
	if (res == NULL){
		printf("Error: createSPCommand has failed");
		free(res);
		return NULL;
	}

	res->cmd = cmd;
	res->validArg = ValidArg;
	res->arg = arg;
	res->mv = mv;
	res->str = str_res;

	return res;

}

SETTING* createSetting(int gameMode, int difficulty, int color){
	SETTING* res = NULL;
	res = (SETTING*)malloc(sizeof(SETTING));

	if (res == NULL){
		printf("Error: createSetting has failed");
		free(res);
		return NULL;
	}

	res->gameMode = gameMode;
	res->difficulty = difficulty;
	res->color = color;
	return res;

}

void destroySetting(SETTING *set){
	if(set==NULL){
		return;
	}
	free(set);
}

SPCommand* copySPCommad(SPCommand* spcommand){
	return createSPCommand (spcommand->cmd, spcommand->validArg, spcommand->arg,spcommand->mv,spcommand->str);

}

bool hasNextToken(SPCommand* command){
	if(		command->cmd == GAME_MODE ||
			command->cmd == DIFFICULTY ||
			command->cmd == USER_COLOR) {
		return true;
	}
	else{
		return false;
	}
}

bool spParserIsInt(const char* str){
	unsigned int i = 0;
	if(str[0] == '-'){
		i++;
	}
	while(i<strlen(str)-1){
		if(str[i]>= '9' || str[i]<= '0'){
			return false;
		}
		i++;
	}

	return true;
}


SPCommand* settingParseLine(const char* str){
	char strCopy[SP_MAX_LINE_LENGTH];
	strcpy(strCopy, str);
	unsigned int x = strlen(str);
	strCopy[x]= '\0';
	MOVE* mv = (MOVE*)malloc(sizeof(MOVE));
	SPCommand* command = createSPCommand(SP_INVALID_LINE,0,false,*mv," ");
	destroyMove(mv);
	const char delimiter[]=" \t\n\r";

	char* token = strtok(strCopy, delimiter);

	if(strcmp(token,"game_mode")==0){
		command->cmd = GAME_MODE;
	}

	if(strcmp(token,"difficulty")==0){
		command->cmd = DIFFICULTY;
	}

	if(strcmp(token,"user_color")==0){
		command->cmd = USER_COLOR;
	}

	if(strcmp(token,"load")==0){
		command->cmd = LOAD;
	}

	if(strcmp(token,"default")==0){
		command->cmd = DEFAULT;
	}

	if(strcmp(token,"print_setting")==0){
		command->cmd = PRINT_SETTING;

	}

	if(strcmp(token,"quit")==0){
		command->cmd = QUIT;
	}

	if(strcmp(token,"start")==0){
		command->cmd = START;
	}


	token = strtok(NULL,delimiter);

	if(hasNextToken(command)){
		if(token!=NULL && spParserIsInt(token)){
			command->arg = atoi(token);
			command->validArg= true;
		}
		else{
			command->cmd = SP_INVALID_LINE;
		}
	}
	else if(command->cmd == LOAD){
		command->str = token;
	}

	else if(token!=NULL){
		command->cmd = SP_INVALID_LINE;
	}

	return command;

}

bool moveFormatValidation(char* token){
	char* format = "< , > to < , >";
	for(size_t i = 0; i <strlen(token)-1; i++){
		if(i!= 1 && i!=3 && i!=10 && i!=12){
			if(format[i]!=token[i]){
				return false;
			}
		}
	}
	return true;
}


MOVE* parseMove(char* token){
	MOVE m = defaultMove();
	char strCopy[1024];
	strcpy(strCopy, token);
	const char delimiter[]= " <>,to";
	int input[4]={0};
	if(!moveFormatValidation(strCopy)){
		printf(E_MOVE_POS);
		return createMove(m.src,m.dst,m.piece,m.piece_dst);
	}
	char* tk = strtok(strCopy,delimiter);
	if(( tk == NULL || tk[0] < '1' || tk[0]>'8' || strlen(tk)>1)){
		printf(E_MOVE_POS);
		return createMove(m.src,m.dst,m.piece,m.piece_dst);
	}
	else{
		input[0] = tk[0]-'1';
	}


	for(int i = 1 ; i<4 ; i++){
	tk = strtok(NULL,delimiter);
		if(tk == NULL || strlen(tk)>1){
			printf(E_MOVE_POS);
			return createMove(m.src,m.dst,m.piece,m.piece_dst);
		}
		else{
			if(i%2 == 0){
				if((tk[0] < '1' || tk[0]>'8')){
					printf(E_MOVE_POS);
					return createMove(m.src,m.dst,m.piece,m.piece_dst);
				}
				else{
					input[i] = tk[0]-'1';
				}
			}
			if(i%2 == 1){
				if((tk[0] < 'A' || tk[0] > 'H')){
					printf(E_MOVE_POS);
					return createMove(m.src,m.dst,m.piece,m.piece_dst);
				}
				else{
					input[i] = tk[0]-'A';
				}
			}
		}
	}
	POSITION *src = createPosition(input[0],input[1]);
	POSITION *dst = createPosition(input[2],input[3]);
	char piece = '_';
	MOVE *mv = createMove(*src,*dst,piece,piece);
	destroyPosition(src);
	destroyPosition(dst);

	return mv;

}

POSITION* extractPositionForGetMoves(char* token){
	int x = -1;
	int y  = -1;
	if(token[0] == '<' && token[2] == ',' && token[4]== '>'){
		if((token[1] < '1' || token[1]>'8')){
			printf(E_MOVE_POS);
			return NULL;
		}
		else{
			x = token[1]-'1';
		}
		if((token[3] < 'A' || token[3]>'H')){
			printf(E_MOVE_POS);
			return NULL;
		}
		else{
			y = token[3]-'A';
			return createPosition(x,y);
		}

	}
	else{
		printf(E_MOVE_POS);
		return NULL;
	}
}

SPCommand* gamePraseLine(const char* str){
	char strCopy[1024];
	strcpy(strCopy, str);
	MOVE* mv = (MOVE*)malloc(sizeof(MOVE));
	SPCommand* command = createSPCommand(SP_INVALID_LINE,0,false,*mv," ");
	//Command contains a copy of move
	destroyMove(mv);

	const char delimiter[]=" \t\n\r";
	char* token = strtok(strCopy, delimiter);

	if(strcmp(token,"move")==0){
		command->cmd = SET_MOVE;
		token+=5; //advancing the char point to the start of the move source and destination
		MOVE* move = NULL;
		move =parseMove(token);
		command->mv =*move;
		if((command->mv.dst.x) == -1){
			command->cmd = SP_INVALID_LINE;
		}
		destroyMove(move);
	}

	if(strcmp(token,"get_moves")==0){
		command->cmd = GET_MOVES;
		token+=10; //advancing the char point to the start of the move source and destination
		POSITION* pos = extractPositionForGetMoves(token);
		if(pos == NULL){
			command->cmd = SP_INVALID_LINE;
		}else{
			MOVE* tmp = createMove(*pos,*pos,'_','_');
			command->mv = *tmp;
			destroyMove(tmp);
		}
		destroyPosition(pos);


	}

	if(strcmp(token,"save")==0){
		command->cmd = SAVE;
		token = strtok(NULL,delimiter);
		command->str =token;
	}

	if(strcmp(token,"undo")==0){
		command->cmd = UNDO;
	}

	if(strcmp(token,"reset")==0){
		command->cmd = RESET;
	}

	if(strcmp(token,"quit")==0){
		command->cmd = QUIT;
	}
	return command;

}



SPCommand* getCommandFromUser(int gameFlag){
	SPCommand* command = NULL;

	char str[SP_MAX_LINE_LENGTH];
	if((fgets(str,SP_MAX_LINE_LENGTH,stdin)!=NULL) && strlen(str)<=SP_MAX_LINE_LENGTH && strlen(str)>0){
		if(gameFlag){
			command = gamePraseLine(str);
		}
		else{
			command = settingParseLine(str);
		}

	}

	return command;
}

void settingAction(SETTING *set, SPCommand *command, GAME* game){
	char str[SP_MAX_LINE_LENGTH];
	strcpy(str,command->str);
	GAME* load = NULL;
	switch (command->cmd){
	case SP_INVALID_LINE:
		printf(E_INV_CM);
		break;
	case GAME_MODE:
		setGameMode(set,command);
		break;
	case DIFFICULTY:
		setDifficulty(set,command);
		break;
	case USER_COLOR:
		setUserColor(set,command);
		break;
	case LOAD:
		load = loadGame(str);
		updateGame(load,game);
		destroyGame(load);
		break;
	case DEFAULT:
		setDefault(set);
		break;
	case PRINT_SETTING:
		printSetting(set);
		break;
	case QUIT:
		printf(C_QUIT);
		destroyGame(game);
		destroySPCommand(command);
		exit(0);
		break;
	case START:
		game->gameStatus = 1;
		gameFlag=1;
		break;
	default:
		break;
	}
}

void gameAction(GAME* game, SPCommand *command){
	MOVE_LIST* moves = NULL;
	switch (command->cmd){
	case SP_INVALID_LINE:

		command = getCommandFromUser(gameFlag);
		break;

	case SET_MOVE:

		if(legalMove(game,&(command->mv))){
			command->mv.piece = getPiece(game,&command->mv);
			command->mv.piece_dst = game->board[command->mv.dst.x][command->mv.dst.y];
			makeMove(game,&(command->mv));
		}
		else{
			if(!myPiece(game,&command->mv)){
				printf(E_MOVE_PIECE);
				command->cmd = SP_INVALID_LINE;
				printB = 0;

			}
			else{
				printf(E_MOVE_LGL);
				command->cmd = SP_INVALID_LINE;
				printB = 0;

			}

		}

		break;

	case GET_MOVES:

		if(!myPiece(game, &(command->mv))){
			game->currPlayer ? printf(E_GET_MOVE_PIECE_W) : printf(E_GET_MOVE_PIECE_B) ;
		}

		else if(game->set->difficulty >2 || game->set->gameMode != 1){
			printf("Command not supported\n");
		}

		else{
			moves = getPieceMoves(game,&(command->mv.src));
			bubbleSort(moves);
			printListOfMoves(moves,game);
			destroyMoveList(moves);
		}

		break;

	case SAVE:
		saveGame(game,command->str);
		break;

	case UNDO:
		if(game->set->gameMode == 1){
		undoMove(game,(game->currPlayer)^1); // first undo for computer
		undoMove(game,(game->currPlayer)^1);
		}
		else{
			undoMove(game,(game->currPlayer));
		}
		break;

	case RESET:

		gameFlag = 0;
		initGame(game);

		printf(C_RESET);
		printf(C_INIT);
		break;

	case QUIT:
		destroyGame(game);
		destroySPCommand(command);
		printf(C_QUIT);
		exit(0);
		break;

	default:
		break;
	}
}



void twoPlayerGame(GAME* game, SPCommand * cmd){
	if(playerInCheck(game)){
		game->currPlayer ? printf(G_CHECK_W) : printf(G_CHECK_B) ;
	}
	if(printB){
	printBoard(game->board);
		}

	game->currPlayer ? printf(C_WP_TURN): printf(C_BP_TURN);
	cmd =getCommandFromUser(gameFlag);
	if(cmd->cmd == SAVE || cmd->cmd == SP_INVALID_LINE){
		printB = 0;
	}
	else{
		printB = 1;
	}
	while((cmd->cmd) == SP_INVALID_LINE){
		SPCommand* tmp = cmd;
		game->currPlayer ? printf(C_WP_TURN): printf(C_BP_TURN);
		cmd =getCommandFromUser(gameFlag);
		if(cmd->cmd == GET_MOVES || cmd->cmd == SAVE || cmd->cmd == SP_INVALID_LINE){
			printB = 0;
		}
		else{
			printB = 1;
		}
		destroySPCommand(tmp);
	}
	gameAction(game,cmd);
	destroySPCommand(cmd);
}

void onePlayerGame(int playerColor,GAME* game, SPCommand * cmd){

	if(playerColor){
				if(game->currPlayer == playerColor){
					if(playerInCheck(game)){
						printf(G_CHECK_AI);
						}

					if(printB){
					printBoard(game->board);
					}
					printf(C_WP_TURN);
					cmd =getCommandFromUser(gameFlag);
					if(cmd->cmd == GET_MOVES || cmd->cmd == SAVE || cmd->cmd == SP_INVALID_LINE){
						printB = 0;
					}
					else{
						printB = 1;
					}
					while((cmd->cmd) == SP_INVALID_LINE){
						printf(C_WP_TURN);
						cmd =getCommandFromUser(gameFlag);
						if(cmd->cmd == GET_MOVES || cmd->cmd == SAVE || cmd->cmd == SP_INVALID_LINE){
							printB = 0;
						}
						else{
							printB = 1;
						}
					}
					gameAction(game,cmd);
					destroySPCommand(cmd);

				}
				else{
					if(playerInCheck(game)){
						game->currPlayer ? printf(G_CHECK_W) : printf(G_CHECK_B) ;
					}
					computerTurn(game);
				}
			}
	else{
		if(game->currPlayer){
			if(playerInCheck(game)){
				game->currPlayer ? printf(G_CHECK_W) : printf(G_CHECK_B) ;
			}
			computerTurn(game);
			}
		else{
			if(playerInCheck(game)){
				printf(G_CHECK_AI);
				}

			if(printB){
			printBoard(game->board);
			}
			printf(C_BP_TURN);
			cmd =getCommandFromUser(gameFlag);
			if(cmd->cmd == GET_MOVES || cmd->cmd == SAVE || cmd->cmd == SP_INVALID_LINE){
				printB = 0;
			}
			else{
				printB = 1;
			}
			while((cmd->cmd) == SP_INVALID_LINE){
				printf(C_BP_TURN);
				cmd =getCommandFromUser(gameFlag);
				if(cmd->cmd == GET_MOVES || cmd->cmd == SAVE || cmd->cmd == SP_INVALID_LINE){
					printB = 0;
				}
				else{
					printB = 1;
				}
			}
			gameAction(game,cmd);
			destroySPCommand(cmd);
		}
	}
}


void parser_main(){
	SPCommand *cmd = NULL;
	GAME* game = NULL;
	game =  createGame(HISTORY_SIZE);
	initGame(game);
	SETTING* set = NULL;
	set = game->set;
	printf(C_INIT);
	while(true){
		if(existsGameWinner(game)!=0 || isTie(game)){
			gameFlag = 2;
			printEndGame(game);
			destroyGame(game);
			exit(0);
		}
		if(gameFlag == 1){
				if(game->set->gameMode == 1){
					onePlayerGame(game->set->color,game,cmd);
				}
				else if(game->set->gameMode == 2){
					twoPlayerGame(game,cmd);
				}
		}
		else if(gameFlag == 0){
			cmd =getCommandFromUser(gameFlag);
			settingAction(set,cmd,game);

			updateSetting(set,game->set);
			destroySPCommand(cmd);
		}

	}

}






/*
 * TEST SUITE
 */


//TESTED OK
void settingActionTest(){
	MOVE *move = (MOVE*)malloc(sizeof(MOVE));
	SPCommand *cmd =createSPCommand(LOAD,false,0,*move,"output.xml");

	GAME* game;
	game = createGame(6);
	game->set =createSetting(1,2,1);
	SETTING *set = game->set;
	settingAction(set,cmd,game);
	printSetting(game->set);
	printBoard(game->board);
}

void settingParseLineTest(){
	const char* str = "load output.xml";
	GAME* game;
	game = createGame(6);
	char cont[1024];
	SPCommand* command;
	command = settingParseLine(str);
	printf("command str is: %s\n",command->str);
	strcpy(cont,command->str);
	//game = loadGame(cont);
	command->str =cont;
	settingAction(game->set,command,game);
	printSetting(game->set);
	printBoard(game->board);
}


void getCommandFromUserTest(){
	GAME* game;
	game = createGame(6);
	getCommandFromUser(gameFlag);
	printSetting(game->set);
	printBoard(game->board);
}

void gameParserTest(){
	const char* str = "move <1,A> to <3,B>";
	gamePraseLine(str);
}


void parseMoveTest(){
	char* test1 ="<3,E> to <4,D> xxx";
	char* test2 ="<3,B> to <4,c>";
	char* test3 ="<9,D> to <3,C>";
	char* test4 ="<5,A> to <3,D>";
	char* test5 = " ";
	char* test6 = "<3,C> tt <4,D>";
	char * tokens[] = {test1,test2,test3,test4,test5,test6};
	for (int i = 0 ; i < 6 ; i ++){
		MOVE* mv = (MOVE*)malloc(sizeof(MOVE));
		mv = parseMove(tokens[i]);
		if(mv!=NULL){



		}
		else{
		}
	}
}






