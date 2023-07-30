#ifndef CHIP8_HEADER
#define CHIP8_HEADER

#include <fstream>
#include <random>
#include <chrono>
#include <cstring>

const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

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

	typedef void (Chip8::*Chip8Func) ();
	Chip8Func table_[0xF + 1];
	Chip8Func table0_[0xE + 1];
	Chip8Func table8_[0xE + 1];
	Chip8Func tableE_[0xE + 1];
	Chip8Func tableF_[0x65 + 1];

public:
	/*
	 * Class Functions
	 */
	Chip8(); //Constructor
	void loadRom(char const* fileName);
	void setFunctionPtrTable();
	void Table0();
	void Table8();
	void TableE();
	void TableF();

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
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();
	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();
	void OP_Ex9E();
	void OP_ExA1();
	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();
	void OP_NULL();
};

#endif
