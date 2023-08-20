UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
CC = gcc
CXX = g++
endif

ifeq ($(UNAME), Darwin)
CC = gcc-13
CXX = g++-13
endif

CFLAGS = -g -Wextra -Wall -pedantic -Wfloat-equal -Wundef -Wshadow \
         -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 \
         -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default \
         -Wswitch-enum -Wconversion -Wunreachable-code
CXXFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
	   -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self \
	   -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs \
	   -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls \
	   -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
	   -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -g \
#	   -Werror

#COLOURS
GREEN='\033[0;32m'                                                              
RED='\033[0;31m'                                                                
YELLOW='\033[0;33m'                                                                
BLUE='\033[0;34m'
NC='\033[0m'
