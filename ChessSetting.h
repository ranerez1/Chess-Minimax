/*
 * ChessSetting.h
 *
 *  Created on: 19 Aug 2017
 *      Author: ranerez
 */

#ifndef CHESSSETTING_H_
#define CHESSSETTING_H_
#include <stdio.h>
#include <stdlib.h>
#include "ChessParser.h"
#include "ChessParams.h"

/*
 * The ChessSetting module is responsible for updating the setting parameters
 */

/*
 * Function to update the game mode of a setting (1- 1 player mode 2- 2 player mode). The function receives;
 * @param - SETTING* set- a pointer to the setting being updated
 * @param -SPCommand* a pointer the the command that requested to update the setting
 * @ret - void
 */
void setGameMode(SETTING *set, SPCommand *command);


/*
 * Function to update the difficulty of a setting. The function makes sure it is a legal difficulty.
 * The level 5 difficulty is not supported.
 * The function receives;
 * @param - SETTING* set- a pointer to the setting being updated
 * @param -SPCommand* a pointer the the command that requested to update the setting
 * @ret - void
 */
void setDifficulty(SETTING *set, SPCommand *command);

/*
 * Function to update the user color of a setting (0-Black 1-White). The function receives;
 * @param - SETTING* set- a pointer to the setting being updated
 * @param -SPCommand* a pointer the the command that requested to update the setting
 * @ret - void
 */

void setUserColor(SETTING *set, SPCommand *command);

/*
 * Function to restore setting to default as defined in ChessParams.h. The function receives;
 * @param - SETTING* set- a pointer to the setting being updated
 * @ret - void
 */
void setDefault(SETTING *set);

/*
 * Function to update the setting from another setting. The function receives;
 * @param - SETTING* set- a pointer to the setting being updated
 * @param - SETTING* set- a pointer to the setting from which we are updating
 * @ret - void
 */
void updateSetting(SETTING* from, SETTING* to);




#endif /* CHESSSETTING_H_ */
