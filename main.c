/*
 * main.c
 *
 *  Created on: 18 Aug 2017
 *      Author: ranerez
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ChessParser.h"



int main(int argc , char** argv){
	if(argc > 2){
		printf("Error in main: argc is different than 2\n");
		exit(0);
	}

	else if(argc == 1){
		parser_main();
	}

	else{
		if(strcmp(argv[1], "-c") == 0){
			parser_main();
		}

	}

	return 0;
}
