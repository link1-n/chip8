include ../../build/common_flags.mk

PROJECT = chip8

INC_DIR = ../include
LIB_DIR = ../lib

all: $(LIB_DIR)/$(PROJECT).o 

$(LIB_DIR)/$(PROJECT).o: $(PROJECT).cpp $(INC_DIR)/$(PROJECT).h | $(LIB_DIR)
	@echo -e ${YELLOW}BUILDING $(PROJECT)${NC}
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(PROJECT).cpp \
		-o $(LIB_DIR)/$(PROJECT).o
$(LIB_DIR):
	@echo -e ${YELLOW}LIBS FOLDER DOES NOT EXIST, CREATING IT..${NC}
	mkdir -p $@

.PHONY: clean
clean:
	@echo -e ${RED}CLEAN $(PROJECT)${NC}
	rm $(LIB_DIR)/$(PROJECT).o
