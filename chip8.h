#ifndef CHIP8_HEADER
#define CHIP8_HEADER

#include <fstream>

class Chip8 {
public:
	/*
	 * Class Variables
	 */
	uint8_t registers[16]{};
	uint8_t memory[4096]{};
	uint16_t index{};
	uint16_t pc{};
	uint16_t stack[16]{};
	uint8_t sp{};
	uint8_t soundTimer{};
	uint8_t delayTimer{};
	uint8_t keypad[16]{};
	uint32_t screen[64 * 32]{};
	uint16_t opCode;

	/*
	 * Class Functions
	 */
	Chip8(); //Constructor
	void loadRom(char const* fileName);
};

#endif
