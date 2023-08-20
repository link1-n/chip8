include ../../build/common_flags.mk

EMU = emulator
DIS = disassembler

MODULES = cpu gpu

INC_FLAGS = -I ../../cpu/include/ \
	    -I ../../gpu/include/

INC_FLAGS += -I /usr/include/SDL2/

LIB_FLAGS = ../../cpu/lib/chip8.o \
	    ../../gpu/lib/display.o -lSDL2

BIN_DIR = ../bin

all: $(BIN_DIR)/$(EMU) $(BIN_DIR)/$(DIS) 

$(BIN_DIR)/$(EMU): emulator.cpp
	@echo -e ${YELLOW}BUILDING $(EMU)${NC}
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) $(LIB_FLAGS) \
		$(EMU).cpp -o $(BIN_DIR)/$(EMU)

$(BIN_DIR)/$(DIS): disassembler.cpp
	@echo -e ${YELLOW}BUILDING $(DIS)${NC}
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) $(LIB_FLAGS) \
		$(DIS).cpp -o $(BIN_DIR)/$(DIS)

.PHONY: clean dependency clean-dependency
clean:
	@echo -e ${RED}CLEAN $(EMU) $(DIS)${NC}
	rm $(BIN_DIR)/$(EMU)
	rm $(BIN_DIR)/$(DIS)

dependency:
	@echo -e ${GREEN}BUILDING DEPENDENCIES${NC}
	@for dir in $(MODULES); do \
		$(MAKE) -C ../../$$dir/src; \
	done

clean-dependency:
	@echo -e ${RED}CLEANING DEPENDENCIES${NC}
	@for dir in $(MODULES); do \
		$(MAKE) clean -C ../../$$dir/src; \
	done


