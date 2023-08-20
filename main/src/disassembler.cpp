#include <iostream>

#include "chip8.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <ROM>" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	char const* romFile = argv[1];

	Chip8 chip8;
	chip8.loadRom(romFile);

	chip8.disassembleRom();

	return 0;
}
