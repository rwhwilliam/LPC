#include <iostream>

#include "SDL.h"

#include "Engine/Graphics/FontLoader.h"
#include "Engine/Graphics/ImageLoader.h"
#include "Engine/Sound/SoundLoader.h"
#include "Engine/State/State.h"
#include "Engine/State/StateManager.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Config.h"

#include "Engine/Time/Timer.h"

#include "Villages/States/SimState.h"

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

		if(!ImageLoader::init())
		return 0;

		if(!FontLoader::init())
			return 0;

		if(!SoundLoader::init())
			return 0;

		SDL_WM_SetCaption( "Villages", NULL );

		StateManager stateManager;

		SimState* s = new SimState("data/maps/map1.xml", atoi(Config::getConfig("ScreenWidth").c_str()), atoi(Config::getConfig("ScreenHeight").c_str()), 0, 0);

		stateManager.push(s);

		bool quit = false;

		Logger::debugFormat("logging test");

		Timer t;
		

		while(!quit)
		{
			t.start();
			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_QUIT)
				{
					quit = true;
				}

				stateManager.raiseEvent(&event);
			}
			

			Uint8 *keystates = SDL_GetKeyState(NULL);

			SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

			stateManager.update(0, keystates);

			stateManager.draw(screen);

			SDL_Flip(screen);

			
			Logger::debugFormat("ms %i", t.get_ticks());
			t.stop();
		}

		SoundLoader::cleanup();
		FontLoader::cleanup();
		ImageLoader::cleanup();

		SDL_EnableUNICODE(SDL_DISABLE);

		SDL_Quit();
	}
	catch(exception& e)
	{
		Logger::errorFormat("Error Message: {0}", e.what());
	}

	return 0;
}