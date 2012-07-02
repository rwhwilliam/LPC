#include <iostream>

#include "SDL.h"

#include "Engine/Util/Logger.h"
#include "Engine/Util/Config.h"

using namespace std;

SDL_Event event;

int main(int argc, char* args[])
{
	try
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
			return 0;

		if(SDL_EnableUNICODE( SDL_ENABLE ) == -1)
			return 0;

		SDL_Surface* screen = SDL_SetVideoMode(atoi(Config::getConfig("ScreenWidth").c_str()), atoi(Config::getConfig("ScreenHeight").c_str()), 32, SDL_SWSURFACE);

		bool quit = false;

		Logger::debugFormat("logging test");

		while(!quit)
		{
			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			Uint8 *keystates = SDL_GetKeyState(NULL);

			SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

			SDL_Flip(screen);
		}

		SDL_EnableUNICODE(SDL_DISABLE);

		SDL_Quit();
	}
	catch(exception& e)
	{
		Logger::errorFormat("Error Message: {0}", e.what());
	}

	return 0;
}