include /home/ayan/code/common_flags.mk
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

PROJECT = main

MODULES = cpu gpu

INC_FLAGS = -I ../cpu/include/ \
	    -I ../gpu/include/

INC_FLAGS += -I /usr/include/SDL2/

LIB_FLAGS = ../cpu/lib/chip8.o \
	    ../gpu/lib/display.o -lSDL2

all: $(PROJECT) 

$(PROJECT): main.cpp
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) $(LIB_FLAGS) \
		$(PROJECT).cpp -o $(PROJECT)

.PHONY: clean dependency clean-dependency
clean:
	rm main

dependency:
	@echo -e ${GREEN}BUILDING DEPENDENCIES${NC}
	@for dir in $(MODULES); do \
		$(MAKE) -C ../$$dir/src; \
	done

clean-dependency:
	@echo -e ${RED}CLEANING DEPENDENCIES${NC}
	@for dir in $(MODULES); do \
		$(MAKE) clean -C ../$$dir/src; \
	done


