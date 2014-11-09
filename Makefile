
CC=clang
CFLAGS= -g -std=c11 `pkg-config --cflags --libs gtk+-3.0 gtksourceview-3.0` -lpthread

OBJ = main.o gui.o
HDR = gui.h
BIN = lithium

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ): $(HDR)
