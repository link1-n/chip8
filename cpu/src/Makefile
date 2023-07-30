include ../../build/common_flags.mk

PROJECT = chip8

INC_DIR = ../include
LIB_DIR = ../lib

all: $(LIB_DIR)/$(PROJECT).o 

$(LIB_DIR)/$(PROJECT).o: $(PROJECT).cpp $(INC_DIR)/$(PROJECT).h
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(PROJECT).cpp \
		-o $(LIB_DIR)/$(PROJECT).o

.PHONY: clean
clean:
	rm $(LIB_DIR)/$(PROJECT).o
