
CC=clang
CFLAGS=`pkg-config --cflags --libs gtk+-2.0`

OBJ=main.o
HDR=gui.h
BIN=lithium

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ):$(HDR)
