include ../../build/common_flags.mk

PROJECT = display

INC_DIR = ../include
LIB_DIR = ../lib

ifeq ($(UNAME), Darwin)
INC_FLAGS = -I/opt/homebrew/include/SDL2
LIBS = -L/opt/homebrew/lib -lSDL2
endif

ifeq ($(UNAME), Linux)
INC_FLAGS = -I /usr/include/SDL2/
LIBS = -lSDL2
endif

all: $(LIB_DIR)/$(PROJECT).o

$(LIB_DIR)/$(PROJECT).o: $(PROJECT).cpp $(INC_DIR)/$(PROJECT).h | $(LIB_DIR)
	@echo -e ${YELLOW}BUILDING $(PROJECT)${NC}
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(INC_FLAGS) $(LIBS) -c $(PROJECT).cpp \
		-o $(LIB_DIR)/$(PROJECT).o

$(LIB_DIR):
	@echo -e ${YELLOW}LIBS FOLDER DOES NOT EXIST, CREATING IT..${NC}
	mkdir -p $@

.PHONY: clean
clean:
	@echo -e ${RED}CLEAN $(PROJECT)${NC}
	rm $(LIB_DIR)/$(PROJECT).o
