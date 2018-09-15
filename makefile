
CC= gcc
C_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main


OBJ_FILES = ChessArrayList.o ChessMiniMax.o ChessGame.o ChessIO.o ChessMoves.o ChessParser.o ChessSetting.o ChessUtils.o main.o 
OBJ_FILES_GUI = GUImain.o GameWin.o GUIManager.o LoadWin.o MainWin.o NewGameWin.o
ALL_OBJS = $(OBJ_FILES) $(OBJ_FILES_GUI)
TARGET = chessprog
VPATH = graphics

all: $(ALL_OBJS)
	$(CC) $(ALL_OBJS) $(SDL_LIB) -o $(TARGET)

$(TARGET): $(ALL_OBJS)
	$(CC) $(ALL_OBJS) $(SDL_LIB) -o $@


main.o: main.c ChessParser.h GUImain.h
	$(CC) $(C_FLAG) $(SDL_COMP_FLAG) -c $*.c

ChessArrayList.o: ChessArrayList.c ChessArrayList.h ChessParams.h ChessParser.h
	$(CC) -c $(C_FLAG) $*.c

ChessGame.o: ChessGame.c ChessGame.h ChessArrayList.h ChessUtils.h ChessMoves.h ChessParams.h ChessMiniMax.h 
	$(CC) -c $(C_FLAG) $*.c

ChessIO.o: ChessIO.c ChessIO.h ChessParser.h ChessSetting.h ChessParams.h ChessGame.h
	$(CC) -c $(C_FLAG) $*.c

ChessMiniMax.o: ChessMiniMax.c ChessMiniMax.h ChessGame.h ChessMoves.h ChessParams.h ChessParser.h
	$(CC) -c $(C_FLAG) $*.c

ChessMoves.o: ChessMoves.c ChessMoves.h ChessGame.h ChessMoves.h ChessParams.h ChessParser.h
	$(CC) -c $(C_FLAG) $*.c
	
ChessParser.o: ChessParser.c ChessParser.h ChessUtils.h ChessIO.h ChessParams.h
	$(CC) -c $(C_FLAG) $*.c
	
ChessSetting.o: ChessSetting.c ChessSetting.h ChessParser.h ChessParams.h
	$(CC) -c $(C_FLAG) $*.c
	
ChessUtils.o: ChessUtils.c ChessUtils.h ChessParser.h ChessMoves.h ChessGame.h ChessParams.h
	$(CC) -c $(C_FLAG) $*.c

graphics/GUImain.o: GUImain.c GUImain.h GUIManager.h
	$(CC) $(C_FLAG) $(SDL_COMP_FLAG) -c $*.c

graphics/LoadWin.o: LoadWin.c LoadWin.h ChessGame.h
	$(CC) $(C_FLAG) $(SDL_COMP_FLAG) -c $*.c 

graphics/GameWin.o: GameWin.c GameWin.h LoadWin.h ChessParser.h ChessGame.h ChessMoves.h ChessSetting.h ChessIO.h ChessMiniMax.h 
	$(CC) $(C_FLAG) $(SDL_COMP_FLAG) -c $*.c

graphics/GUIManager.o:  GUIManager.c GUIManager.h MainWin.h LoadWin.h GameWin.h NewGameWin.h GUImain.h
	$(CC) $(C_FLAG) $(SDL_COMP_FLAG) -c $*.c

graphics/MainWin.o: MainWin.c MainWin.h 
	$(CC) $(C_FLAG) $(SDL_COMP_FLAG) -c $*.c 

graphics/NewGameWin.o: NewGameWin.c NewGameWin.h GameWin.h
	$(CC) $(C_FLAG) $(SDL_COMP_FLAG) -c $*.c 
	


clean:
	-rm $(TARGET) $(ALL_OBJS)