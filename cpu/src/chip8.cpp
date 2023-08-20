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

	setFunctionPtrTable();
}

void Chip8::setFunctionPtrTable() {
	table_[0x0] = &Chip8::Table0;
	table_[0x1] = &Chip8::OP_1nnn;
	table_[0x2] = &Chip8::OP_2nnn;
	table_[0x3] = &Chip8::OP_3xkk;
	table_[0x4] = &Chip8::OP_4xkk;
	table_[0x5] = &Chip8::OP_5xy0;
	table_[0x6] = &Chip8::OP_6xkk;
	table_[0x7] = &Chip8::OP_7xkk;
	table_[0x8] = &Chip8::Table8;
	table_[0xA] = &Chip8::OP_Annn;
	table_[0xB] = &Chip8::OP_Bnnn;
	table_[0xC] = &Chip8::OP_Cxkk;
	table_[0xD] = &Chip8::OP_Dxyn;
	table_[0xE] = &Chip8::TableE;
	table_[0xF] = &Chip8::TableF;

	for (size_t i = 0; i <= 0xE; i++) {
		table0_[i] = &Chip8::OP_NULL;
		table8_[i] = &Chip8::OP_NULL;
		tableE_[i] = &Chip8::OP_NULL;
	}

	table0_[0x0] = &Chip8::OP_00E0;
	table0_[0xE] = &Chip8::OP_00EE;

	table8_[0x0] = &Chip8::OP_8xy0;
	table8_[0x1] = &Chip8::OP_8xy1;
	table8_[0x2] = &Chip8::OP_8xy2;
	table8_[0x3] = &Chip8::OP_8xy3;
	table8_[0x4] = &Chip8::OP_8xy4;
	table8_[0x5] = &Chip8::OP_8xy5;
	table8_[0x6] = &Chip8::OP_8xy6;
	table8_[0x7] = &Chip8::OP_8xy7;
	table8_[0xE] = &Chip8::OP_8xyE;

	tableE_[0x1] = &Chip8::OP_ExA1;
	tableE_[0xE] = &Chip8::OP_Ex9E;

	for (size_t i = 0; i <= 0x65; i++) {
		tableF_[i] = &Chip8::OP_NULL;
	}

	tableF_[0x07] = &Chip8::OP_Fx07;
	tableF_[0x0A] = &Chip8::OP_Fx0A;
	tableF_[0x15] = &Chip8::OP_Fx15;
	tableF_[0x18] = &Chip8::OP_Fx18;
	tableF_[0x1E] = &Chip8::OP_Fx1E;
	tableF_[0x29] = &Chip8::OP_Fx29;
	tableF_[0x33] = &Chip8::OP_Fx33;
	tableF_[0x55] = &Chip8::OP_Fx55;
	tableF_[0x65] = &Chip8::OP_Fx65;
}

void Chip8::Table0() {
	((*this).*(table0_[opCode_ & 0x000Fu])) ();
}

void Chip8::Table8() {
	((*this).*(table8_[opCode_ & 0x000Fu])) ();
}

void Chip8::TableE() {
	((*this).*(tableE_[opCode_ & 0x000Fu])) ();
}

void Chip8::TableF() {
	((*this).*(tableF_[opCode_ & 0x00FFu])) ();
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

void Chip8::cycle() {
	opCode_ = memory_[pc_] << 8u | memory_[pc_ + 1];

	pc_ += 2;

	((*this).*(table_[(opCode_ & 0xF000u) >> 12u])) ();

	if (delayTimer_ > 0) {
		delayTimer_--;
	}
	if (soundTimer_ > 0) {
		soundTimer_--;
	}
}

std::string Chip8::parseOpCode(uint16_t opCode) {
	std::string res = "";

	unsigned int nibble = (opCode & 0xF000u) >> 12u;

	std::cout << std::setw(4) << std::setfill('0')
		<< std::hex << opCode << std::dec;

	switch (nibble) {
		case 0x0u:
		{
			uint16_t lastByte = (opCode & 0x00FFu);
			switch (lastByte) {
				case 0xE0u:
				{
					std::cout << ",CLS";

					break;
				}
				case 0xEEu:
				{
					std::cout << ",RET";

					break;
				}
				default:
				{
					std::cout << ",not found";
					break;
				}
			}
			break;
		}
		case 0x1u:
		{
			uint16_t address = opCode & 0x0FFFu;
			std::cout << ",JP " << std::hex << address << std::dec;

			break;
		}
		case 0x2u:
		{
			uint16_t address = opCode & 0x0FFFu;
			std::cout << ",CALL " << std::hex << address << std::dec; 

			break;
		}
		case 0x3u:
		{
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			uint16_t byte = (opCode & 0x00FFu);

			std::cout << ",LD "
				<< std::hex << Vx << " " << byte << std::dec;

			break;
		}
		case 0x4u:
		{
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			uint16_t byte = (opCode & 0x00FFu);

			std::cout << ",SNE "
				<< std::hex << Vx << " " << byte << std::dec;

			break;
		}
		case 0x5u:
		{
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			unsigned int Vy = (opCode & 0x00F0u) >> 4u;

			std::cout << ",SE " << std::hex
				<< Vx << " " << Vy << std::dec;

			break;

		}
		case 0x6u:
		{
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			uint16_t byte = (opCode & 0x00FFu);

			std::cout << ",LD " << std::hex
				<< Vx << " " << byte << std::dec;

			break;
		}
		case 0x7u:
		{
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			uint16_t byte = (opCode & 0x00FFu);

			std::cout << ",ADD " << std::hex
				<< Vx << " " << byte << std::dec;
			break;
		}
		case 0x8u:
		{
			unsigned int lastNibble = (opCode & 0x000Fu);
			switch (lastNibble) {
				case 0x0u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",LD " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0x1u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",OR " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0x2u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",AND " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0x3u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",XOR " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0x4u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",ADD " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0x5u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",SUB " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0x6u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",SHR " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0x7u:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",SUBN " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				case 0xEu:
				{
					unsigned int Vx = (opCode & 0x0F00u)
						>> 8u;
					unsigned int Vy = (opCode & 0x00F0u)
						>> 4u;

					std::cout << ",SHL " << std::hex
						<< Vx << " " << Vy
						<< std::dec;

					break;
				}
				default:
				{
					std::cout << ",not found";
					break;
				}
			}
			break;
		}
		case 0x9u:
		{
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			unsigned int Vy = (opCode & 0x00F0u) >> 4u;

			std::cout << ",SNE " << std::hex
				<< Vx << " " << Vy << std::dec;

			break;
		}
		case 0xAu:
		{
			uint16_t address = (opCode & 0x0FFFu);

			std::cout << ",LD I " << std::hex
				<< address << std::dec;

			break;
		}
		case 0xBu:
		{
			uint16_t address = (opCode & 0x0FFFu);
			std::cout << ",JP V0 " << std::hex
				<< address << std::dec;

			break;
		}
		case 0xCu:
		{
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			uint16_t byte = (opCode & 0x00FFu);

			std::cout << ",RND " << std::hex
				<< Vx << " " << byte << std::dec;

			break;
		}
		case 0xDu:
		{
			unsigned int Vx = (opCode & 0x0F00) >> 8u;
			unsigned int Vy = (opCode & 0x00F0u) >> 4u;
			unsigned int height = (opCode & 0x000F);

			std::cout << ",DRW " << std::hex
				<< Vx << " " << Vy << " " << height
				<< std::dec;

			break;
		}
		case 0xEu:
		{
			uint8_t lastByte = (opCode & 0x00FFu);
			switch (lastByte) {
				case 0x9Eu:
				{
					uint8_t Vx = (opCode & 0x0F00u)
						>> 8u;
					std::cout << ",SKP " << std::hex
						<< Vx << std::dec;

					break;
				}
				case 0xA1u:
				{
					uint8_t Vx = (opCode & 0x0F00u)
						>> 8u;
					std::cout << ",SKNP " << std::hex
						<< Vx << std::dec;

					break;
				}
				default:
				{
					std::cout << ",not found";
					break;
				}
			}
			break;
		}
		case 0xFu:
		{
			uint8_t lastByte = (opCode & 0x00FFu);
			unsigned int Vx = (opCode & 0x0F00u) >> 8u;
			switch (lastByte) {
				case 0x07:
				{
					std::cout << ",LD " << std::hex
						<< Vx << std::dec << " DT";
					break;
				}
				case 0x0A:
				{
					std::cout << ",LD " << std::hex
						<< Vx << std::dec << " K";
					break;
				}
				case 0x15:
				{
					std::cout << ",LD DT " << std::hex
						<< Vx << std::dec;
					break;
				}
				case 0x18:
				{
					std::cout << ",LD ST " << std::hex
						<< Vx << std::dec;
					break;
				}
				case 0x1E:
				{
					std::cout << ",ADD I " << std::hex
						<< Vx << std::dec;
					break;
				}
				case 0x29:
				{
					std::cout << ",LD F " << std::hex
						<< Vx << std::dec;
					break;
				}
				case 0x33:
				{
					std::cout << ",LD B " << std::hex
						<< Vx << std::dec;
					break;
				}
				case 0x55:
				{
					std::cout << ",LD [I] " << std::hex
						<< Vx << std::dec;
					break;
				}
				case 0x65:
				{
					std::cout << ",LD " << std::hex
						<< Vx << std::dec << " [I]";
					break;
				}
				default:
				{
					std::cout << ",not found";
					break;
				}
			}
			break;
		}
		default:
		{
			std::cout << ",not found";
			break;
		}
	}

	std::cout << std::endl;

	return res;
}

void Chip8::disassembleRom() {
	uint16_t programCounter = START_ADDRESS;

	while (programCounter < 4096) {
		uint16_t opCode = memory_[programCounter] << 8u |
			memory_[programCounter + 1];

		std::cout << std::hex << programCounter << std::dec << ",";
		std::string instruction = parseOpCode(opCode);

		programCounter += 2;
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
	uint8_t byte = opCode_ & 0x00FFu;

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

/* ADD Vx, Vy */
void Chip8::OP_8xy4() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	uint16_t sum = registers_[Vx] + registers_[Vy];

	if (sum > 255U) {
		registers_[0xF] = 1;
	}
	else {
		registers_[0xF] = 0;
	}

	registers_[Vx] = sum & 0xFFu;
}

/* SUB Vx, Vy */
void Chip8::OP_8xy5() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	if (registers_[Vx] > registers_[Vy]) {
		registers_[0xF] = 1;
	}
	else {
		registers_[0xF] = 0;
	}

	registers_[Vx] -= registers_[Vy];
}

/* SHR Vx */
void Chip8::OP_8xy6() {
	uint8_t Vx = (opCode_ & 0x0F00) >> 8u;

	registers_[0xF] = (registers_[Vx] & 0x1u);

	registers_[Vx] >>= 1;
}

/* SUBN Vx, Vy */
void Chip8::OP_8xy7() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	if (registers_[Vy] > registers_[Vx]) {
		registers_[0xF] = 1;
	}
	else {
		registers_[0xF] = 0;
	}

	registers_[Vx] = registers_[Vy] - registers_[Vx];
}

/* SHL Vx */
void Chip8::OP_8xyE() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;

	registers_[0xF] = (registers_[Vx] & 0x80u) >> 7u;

	registers_[Vx] <<= 1;
}

/* SNE Vx, Vy */
void Chip8::OP_9xy0() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;

	if (registers_[Vx] != registers_[Vy]) {
		pc_ += 2;
	}
}

/* LD I, addr */
void Chip8::OP_Annn() {
	uint16_t address = (opCode_ & 0x0FFFu);

	index_ = address;
}

/* JP V0, addr */
void Chip8::OP_Bnnn() {
	uint16_t address = (opCode_ & 0x0FFFu);

	pc_ = registers_[0] + address;
}

/* RND Vx, byte */
void Chip8::OP_Cxkk() {
	uint8_t Vx = (opCode_ & 0x0F00) >> 8u;
	uint8_t byte = (opCode_ & 0x00FF);

	registers_[Vx] = randByte_(randGen_) & byte;
}

/* DRW Vx, Vy, nibble */
void Chip8::OP_Dxyn() {
	uint8_t Vx = (opCode_ & 0x0F00) >> 8u;
	uint8_t Vy = (opCode_ & 0x00F0u) >> 4u;
	uint8_t height = (opCode_ & 0x000F);

	uint8_t xPos = registers_[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers_[Vy] % VIDEO_HEIGHT;

	registers_[0xF] = 0;

	for (unsigned int row = 0; row < height; row++) {
		uint8_t spriteByte = memory_[index_ + row];

		for (unsigned int col = 0; col < 8; col++) {
			uint8_t spritePixel = spriteByte & (0x80u) >> col;
			uint32_t* screenPixel = &screen_[
				((yPos + row) * VIDEO_WIDTH) +
				(xPos + col)];
			if (spritePixel) {
				if (*screenPixel == 0xFFFFFFFF) {
					registers_[0xF] = 1;
				}

				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

/* SKP Vx */
void Chip8::OP_Ex9E() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t key = registers_[Vx];

	if (keypad_[key]) {
		pc_ += 2;
	}
}

/* SKNP Vx */
void Chip8::OP_ExA1() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t key = registers_[Vx];

	if (!keypad_[key]) {
		pc_ += 2;
	}
}

/* LD Vx, DT */
void Chip8::OP_Fx07() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;

	registers_[Vx] = delayTimer_;
}

/* LD Vx, K */
void Chip8::OP_Fx0A() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;

	if (keypad_[0]) {
		registers_[Vx] = 0;
	}
	else if (keypad_[1]) {
		registers_[Vx] = 1;
	}
	else if (keypad_[2]) {
		registers_[Vx] = 2;
	}
	else if (keypad_[3]) {
		registers_[Vx] = 3;
	}
	else if (keypad_[4]) {
		registers_[Vx] = 4;
	}
	else if (keypad_[5]) {
		registers_[Vx] = 4;
	}
	else if (keypad_[6]) {
		registers_[Vx] = 6;
	}
	else if (keypad_[7]) {
		registers_[Vx] = 7;
	}
	else if (keypad_[8]) {
		registers_[Vx] = 8;
	}
	else if (keypad_[9]) {
		registers_[Vx] = 9;
	}
	else if (keypad_[10]) {
		registers_[Vx] = 10;
	}
	else if (keypad_[11]) {
		registers_[Vx] = 11;
	}
	else if (keypad_[12]) {
		registers_[Vx] = 12;
	}
	else if (keypad_[13]) {
		registers_[Vx] = 13;
	}
	else if (keypad_[14]) {
		registers_[Vx] = 14;
	}
	else if (keypad_[15]) {
		registers_[Vx] = 15;
	}
	else {
		pc_ -= 2;
	}
}

/* LD DT, Vx */
void Chip8::OP_Fx15() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	delayTimer_ = registers_[Vx];
}

/* LD ST, Vx */
void Chip8::OP_Fx18() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	soundTimer_ = registers_[Vx];
}

/* ADD I, Vx */
void Chip8::OP_Fx1E() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	index_ += registers_[Vx];
}

/* LD F, Vx */
void Chip8::OP_Fx29() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t digit = registers_[Vx];

	index_ = FONTSET_START_ADDRESS + (5 * digit);
}

/* LD B, Vx */
void Chip8::OP_Fx33() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;
	uint8_t value = registers_[Vx];

	memory_[index_ + 2] = value % 10;
	value /= 10;

	memory_[index_ + 1] = value % 10;
	value /= 10;

	memory_[index_] = value % 10;
}

/* LD [I], Vx */
void Chip8::OP_Fx55() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; i++) {
		memory_[index_ + i] = registers_[i];
	}
}

/* LD Vx, [I] */
void Chip8::OP_Fx65() {
	uint8_t Vx = (opCode_ & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; i++) {
		 registers_[i] = memory_[index_ + i];
	}
}

void Chip8::OP_NULL() {
}

