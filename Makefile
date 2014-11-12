
CXX = clang++
CXXFLAGS = -fdiagnostics-color=auto -Wall `pkg-config gtkmm-3.0 --cflags --libs` --std=c++11

OBJ = main.o lithium.o
HDR = lithium.h

BIN = lithium

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJ)

$(OBJ): $(HDR)

clean:
	rm $(OBJ) $(BIN)
