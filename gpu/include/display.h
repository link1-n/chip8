#ifndef DISPLAY_HEADER
#define DISPLAY_HEADER

#include "SDL.h"

class Display {
	private:
		SDL_Window* window_{};
		SDL_Renderer* renderer_{};
		SDL_Texture* texture_{};
	public:
		Display(char const* title,
				int windowWidth,
				int windowHeight,
				int textureWidth,
				int textureHeight
				); //Constructor
		~Display(); //Destructor

		void update(void const* buffer, int pitch);
		bool processInput(uint8_t* keys);
};

#endif
