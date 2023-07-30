CC = gcc
CXX = g++

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
