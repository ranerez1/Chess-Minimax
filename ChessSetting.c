/*
 * ChessSetting.c
 *
 *  Created on: 18 Aug 2017
 *      Author: ranerez
 */
#include <stdio.h>
#include "ChessParser.h"
#include "ChessParams.h"


void updateSetting(SETTING* from, SETTING* to){
	to->color = from->color;
	to->difficulty = from->difficulty;
	to->gameMode = from->gameMode;
}


void setGameMode(SETTING *set, SPCommand *command){
	set->gameMode = 1;
	if(command->validArg && command->cmd == GAME_MODE){
		if (command->arg < 1 || command->arg >2){
			printf(E_GAME_MODE);
		}
		else{
			set->gameMode = command->arg;
			set->gameMode == 1 ? printf(C_SET_1P) : printf(C_SET_2P);
		}

	}

}

void setDifficulty(SETTING *set, SPCommand *command){
	if(set->gameMode==1){
	set->difficulty = 2;
		if((command->validArg) && (command->cmd == DIFFICULTY)){
			if(command->arg < 1 || command->arg >5){
				printf(E_DIFF_LVL);
			}

			else if(command->arg == 5){
				printf(E_DIFF_EXP);
			}

			else{
				set->difficulty = command->arg;
			}
		}
	}
}

void setUserColor(SETTING *set, SPCommand *command){
	if(set->gameMode==1){
	set->color = 1;
		if(command->validArg && command->cmd == USER_COLOR){
			set->color = command->arg;
		}

	}
}

void setDefault(SETTING *set){
	set->color = DEF_COL;
	set->difficulty = DEF_DIFF;
	set->gameMode = DEF_MODE;
}

