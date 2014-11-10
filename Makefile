
CC=clang
CFLAGS= -g -std=c11 `pkg-config --cflags --libs gtk+-3.0 gtksourceview-3.0` -lpthread

OBJ = main.o gui.o lithium_ui.o
HDR = gui.h lithium_ui.h
BIN = lithium

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ): $(HDR)
