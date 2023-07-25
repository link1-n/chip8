#include "chip8.h"

const unsigned int START_ADDRESS = 0x200;

Chip8::Chip8() {
	pc = START_ADDRESS;
}

void Chip8::loadRom(char const* fileName) {
	std::ifstream file(fileName, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		std::streampos fileSize = file.tellg();
		char* buffer = new char[fileSize];

		file.seekg(0, std::ios::beg);
		file.read(buffer, fileSize);
		file.close();

		for (int i = 0; i < fileSize; i++) {
			memory[START_ADDRESS + i] = buffer[i];
		}

		delete[] buffer;
	}
}

