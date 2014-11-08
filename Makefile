
CC=clang
CFLAGS=`pkg-config --cflags --libs gtk+-3.0`

OBJ = main.o gui.o
HDR = gui.h
BIN = lithium

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ):$(HDR)
