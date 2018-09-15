/*
 * ChessIO.h
 *
 *  Created on: 24 Aug 2017
 *      Author: ranerez
 */

#ifndef CHESSIO_H_
#define CHESSIO_H_

/*
 * This Module includes all the relevant functions for saving and loading games to xml files.
 */

/*
 * Function to load a new game from an xml file.
 * @param- char* - a string that is the path of the file (full or relative)
 * @ret- GAME* a pointer to the game created from the file or NULL in case of an error loading the file
 */
GAME* loadGame(char*);


/*
 * Function to save a new game to an xml file.
 * @param- char* - a string that is the path of the file (full or relative)
 * @param - GAME* a pointer to the game saved to the file
 * @ret-  1 in case of an error saving the file, 0 -if successful
 */
int saveGame(GAME* game, char* filepath);



#endif /* CHESSIO_H_ */
