CPP=g++
CFLAGS=-std=c++11 -g
INCLUDES=-I include/ -I include/impl/ -I/usr/local/include/
FRAMEWORKS=-F /Library/Frameworks/ -framework sfml-graphics -framework sfml-window -framework sfml-system
#SRC=$(wildcard src/**/*.cpp) $(wildcard src/*.cpp) 
SRC=src/sfml-console.cpp src/console.cpp
BIN_DIR=bin


compile-examples:
	$(CPP) $(CFLAGS) $(INCLUDES) $(FRAMEWORKS) $(SRC) examples/basic-example.cpp -o $(BIN_DIR)/basic-example
	chmod u+x $(BIN_DIR)/basic-example
