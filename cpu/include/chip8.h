#ifndef CHIP8_HEADER
#define CHIP8_HEADER

#include <fstream>
#include <random>
#include <chrono>
#include <cstring>

class Chip8 {
private:
	/*
	 * Class Variables
	 */
	uint8_t registers_[16]{};
	uint8_t memory_[4096]{};
	uint16_t index_{};
	uint16_t pc_{};
	uint16_t stack_[16]{};
	uint8_t sp_{};
	uint8_t soundTimer_{};
	uint8_t delayTimer_{};
	uint8_t keypad_[16]{};
	uint32_t screen_[64 * 32]{};
	uint16_t opCode_;

	std::default_random_engine randGen_;
	std::uniform_int_distribution<uint8_t> randByte_;

public:
	/*
	 * Class Functions
	 */
	Chip8(); //Constructor
	void loadRom(char const* fileName);

	/* Op Codes */
	void OP_00E0();
	void OP_00EE();
	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();
	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
};

#endif
