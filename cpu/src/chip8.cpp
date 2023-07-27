#include "chip8.h"

const unsigned int FONTSET_SIZE = 80;
const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() : randGen_(std::chrono::system_clock::now().time_since_epoch().
		count()) {
	pc_ = START_ADDRESS;
	randByte_ = std::uniform_int_distribution<uint8_t>(0, 255U);

	/* Load fontset into memory */
	for (size_t i = 0; i < FONTSET_SIZE; i++) {
		memory_[FONTSET_START_ADDRESS + i] = fontset[i];
	}
}

void Chip8::loadRom(char const* fileName) {
	std::ifstream file(fileName, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		std::streampos fileSize = file.tellg();
		char* buffer = new char[fileSize];

		file.seekg(0, std::ios::beg);
		file.read(buffer, fileSize);
		file.close();

		/* Load buffer into memory */
		for (int i = 0; i < fileSize; i++) {
			memory_[START_ADDRESS + i] = buffer[i];
		}
		delete[] buffer;
	}
}

/* CLS */
void Chip8::OP_00E0() {
	memset(screen_, 0, sizeof(screen_));
}

/* RET */
void Chip8::OP_00EE() {
	sp_--;
	pc_ = stack_[sp_];
}

/* JP addr */
void Chip8::OP_1nnn() {
	uint16_t address = opCode_ & 0x0FFFu;
	pc_ = address;
}

/* CALL addr */
void Chip8::OP_2nnn() {
	uint16_t address = opCode_ & 0x0FFFu;
	stack_[sp_++] = pc_;
	pc_ = address;
}

/* SE Vx, byte */
void Chip8::OP_3xkk() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t byte = opCode_ & 0x00FFu;

	if (registers_[Vx] == byte) {
		pc_ += 2;
	}
}

/* SNE Vx, byte */
void Chip8::OP_4xkk() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t byte = opCode_ & 0x00FFu;

	if (registers_[Vx] != byte) {
		pc_ += 2;
	}
}

/* SE Vx, Vy */
void Chip8::OP_5xy0() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	if (registers_[Vx] == registers_[Vy]) {
		pc_ += 2;
	}
}

/* LD Vx, byte */
void Chip8::OP_6xkk() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t byte = opCode_ * 0x00FFu;

	registers_[Vx] = byte;
}

/* ADD Vx, byte */
void Chip8::OP_7xkk() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t byte = opCode_ & 0x00FFu;

	registers_[Vx] += byte;
}

/* LD Vx, Vy */
void Chip8::OP_8xy0() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	registers_[Vx] = registers_[Vy];
}

/* OR Vx, Vy */
void Chip8::OP_8xy1() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;


	registers_[Vx] |= registers_[Vy];
}

/* AND Vx, Vy */
void Chip8::OP_8xy2() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	registers_[Vx] &= registers_[Vy];
}

/* XOR Vx, Vy */
void Chip8::OP_8xy3() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	registers_[Vx] ^= registers_[Vy];
}

