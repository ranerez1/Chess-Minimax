/*
 * ChessIO.c
 *
 *  Created on: 18 Aug 2017
 *      Author: ranerez
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ChessParser.h"
#include "ChessSetting.h"
#include "ChessGame.h"
#include "ChessParams.h"

/*
 * This module deals with reading and writing files
 * adding all the necessary XML headers
 *
 */


int readMode(char* filepath){
	int mode = 0;
	mode = filepath[11]-'0';
	return mode;
}


int readDiff(char* filepath){
	int diff = 0;
	diff = filepath[12]-'0';
	return diff;
}

int readColor(char* filepath){
	int col = 0;
	col = (filepath[12]-'0');
	return col;
}


int readTurn(char* filepath){
	int turn = 0;
	turn = (filepath[14]-'0');
	return turn;
}

int readStatus(char* filepath){
	int status = 0;
	status = (filepath[13]-'0');
	return status;
}



GAME* loadGame(char* filepath){
	GAME* game =NULL;
	game = createGame(HISTORY_SIZE);
	initGame(game);
	char* content =NULL;
	content = (char*)malloc(SP_MAX_LINE_LENGTH*sizeof(char));
	strcpy(content,filepath);
	unsigned int x = strlen(filepath);
	content[x]= '\0';
	char xml[]="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

	FILE* ifp = fopen(content,"r");
	if(ifp==NULL){
		printf(E_FOPEN);
		free(content);
		return NULL;
	}


	fseek(ifp,strlen(xml)*sizeof(char),SEEK_SET);
	fgetc(ifp); //gets the '\n' char
	fscanf(ifp, "%s", content); //reads game

	while (fscanf(ifp, "%s", content) != 0 && strcmp(content, "</game>")) {
		if(strstr(content,"<current_turn>")){
			game->currPlayer = readTurn(content);
		}
		else if(strstr(content,"<game_mode>")){
			game->set->gameMode = readMode(content);
		}
		else if(strstr(content,"<difficulty>")){
			game->set->difficulty = readDiff(content);
		}
		else if(strstr(content,"<user_color>")){
			game->set->color= readColor(content);
		}

		else if(strstr(content, "<board>") != NULL){
			for (int i = 0; i < MAX_ROW; i++) { //read the 8 rows
				fscanf(ifp, "%s", content);
				for (int j = 0; j < MAX_COL; j++) { //read the columns
					game->board[7-i][j]= content[j+7]; //advance beyond row_i
				}
			}
		}
		else if(strstr(content,"<game_status>")){ // 0 for setting 1- active 2- ended
			game->gameStatus= readStatus(content);
		}
	}
	free(content);
	fclose(ifp);
	return game;
}

void saveBoard(GAME* game, FILE* ofp, char* content) {
	fputs("<board>\n", ofp);
	for (int i = 8; i > 0; i--) {
		sprintf(content, "<row_%d>", i);
		fputs(content, ofp);
		for (int j = 0; j < 8; j++) {
			fputc(game->board[i-1][j], ofp);
		}
		sprintf(content, "</row_%d>\n", i);
		fputs(content, ofp);
	}
	fputs("</board>\n", ofp);
}

void saveSettingsToFile(GAME* game, FILE* ofp) {
	char c = '0';

	fputs("<current_turn>", ofp);
	fputc((char)(game->currPlayer+c), ofp);
	fputs("</current_turn>\n", ofp);

	fputs("<game_mode>", ofp);
	fputc((char)(game->set->gameMode+c), ofp);
	fputs("</game_mode>\n", ofp);

	fputs("<difficulty>", ofp);
	fputc((char)(game->set->difficulty+c), ofp);
	fputs("</difficulty>\n", ofp);

	fputs("<user_color>", ofp);
	fputc((char)(game->set->color+c), ofp);
	fputs("</user_color>\n", ofp);
}

void saveGeneralTag(GAME* game, FILE* ofp) {
	char c = '0';
	fputs("<general>\n", ofp);
	fputs("<game_status>", ofp);
	fputc((char)(game->gameStatus + c), ofp);
	fputs("</game_status>\n", ofp);
	fputs("</general>\n", ofp);
}

int saveGame(GAME* game, char* filepath) {
	FILE* ofp = fopen(filepath, "w");
	char* content = (char*)malloc(SP_MAX_LINE_LENGTH * sizeof(char));

	if (ofp == NULL) {
		printf(E_FCREATE);
		return 1;
	}

	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", ofp);
	fputs("<game>\n", ofp);
	saveSettingsToFile(game, ofp);
	saveBoard(game, ofp, content);
	saveGeneralTag(game, ofp);
	fputs("</game>\n", ofp);
	free(content);
	fclose(ofp);

	return 0;
}





void loadGameTest(){
	char* filepath = "test.xml";
//	printf("len is %d\n",strlen(filepath));
	GAME* game = loadGame(filepath);
	//printSetting(game->set);
	//printBoard(game->board);
	printf("Game status is: %d\n",game->gameStatus);
	destroyGame(game);
}

void saveGameTest(){
	char* filepath = "test.xml";
	GAME* game = loadGame(filepath);
	saveGame(game,"output.xml");

}
