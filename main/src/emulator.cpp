#include <iostream>
#include <chrono>

#include "chip8.h"
#include "display.h"

int main(int argc, char** argv) {
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>"
			<< std::endl;
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int delayDiff = std::stoi(argv[2]);
	char const* romFile = argv[3];

	Display display("emu", VIDEO_WIDTH * videoScale,
			VIDEO_HEIGHT * videoScale,
			VIDEO_WIDTH,
			VIDEO_HEIGHT);

	Chip8 chip8;
	chip8.loadRom(romFile);

	int videoPitch = sizeof(chip8.screen_[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();

	bool quit = false;

//	chip8.disassembleRom();

	while(!quit) {
		quit = display.processInput(chip8.keypad_);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float,
		      std::chrono::milliseconds::period>
			      (currentTime - lastCycleTime).count();
		if (dt > delayDiff) {
			lastCycleTime = currentTime;
			chip8.cycle();
			display.update(chip8.screen_, videoPitch);
		}
	}

	return 0;

}


