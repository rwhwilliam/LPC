/**************************************************************************************************
* Copyright 2012 Ryan Holzum                                                                      *
*                                                                                                 *
* This file is part of Villages.                                                                  *
*                                                                                                 *
* Villages is free software: you can redistribute it and/or modify it under the terms of the      *
* GNU General Public License as published by the Free Software Foundation, either version 3 of    *
* the License, or (at your option) any later version.                                             *
*                                                                                                 *
* Vilages is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without    *
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
* GNU General Public License for more details.                                                    *
*                                                                                                 *
* You should have received a copy of the GNU General Public License along with Vilages.           *
* If not, see http://www.gnu.org/licenses/.                                                       *
**************************************************************************************************/

#include "TitleState.h"

#include <string>

#include "SDL.h"
#include "SDL_mixer.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Gui/ClickableButton.h"
#include "Engine/Sound/SoundLoader.h"
#include "Engine/State/State.h"
#include "Engine/State/StateManager.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/States/SimState.h"
#include "Villages/States/CreditState.h"

using namespace std;

TitleState::TitleState(StateManager* manager, int width, int height, int xloc, int yloc) : State(manager, width, height, xloc, yloc)
{
	Logger::debug("TitleState Constructor");

	bg = new Image("title.png");

	newgame = new ClickableButton<TitleState>(362, 300, 300, 50, "newgame-button-normal.png", "newgame-button-hover.png", "newgame-button-hover.png", this, &TitleState::startNewGame);
	credits = new ClickableButton<TitleState>(362, 380, 300, 50, "credits-button-normal.png", "credits-button-hover.png", "credits-button-hover.png", this, &TitleState::startCredits);

	music = SoundLoader::loadMusic("title.mp3");

	Mix_HaltMusic();

    if(Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( SoundLoader::getMusic(music), -1 );
	}
}

TitleState::~TitleState()
{
	delete bg;

	delete newgame;
	delete credits;

	Mix_HaltMusic();
	Mix_FreeMusic(SoundLoader::getMusic(music));

	Logger::debug("TitleState Destructor");
}

TitleState::TitleState(const TitleState& data) : State(NULL, 0, 0, 0, 0)
{
	throw VillageException("TitleState Copy Constructor");
}

TitleState* TitleState::operator=(const TitleState* rhs)
{
	throw VillageException("TitleState Assignment Operator");
}

void TitleState::startNewGame()
{
	Mix_HaltMusic();

	SimState* s = new SimState(manager, "data/maps/map1.xml", atoi(Config::getConfig("ScreenWidth").c_str()), atoi(Config::getConfig("ScreenHeight").c_str()), 0, 0);

	manager->push(s);
}

void TitleState::startCredits()
{
	CreditState* c = new CreditState(manager, atoi(Config::getConfig("ScreenWidth").c_str()), atoi(Config::getConfig("ScreenHeight").c_str()), 0, 0);

	manager->push(c);
}

void TitleState::update(float time, Uint8* keystrokes)
{

}

void TitleState::raiseEvent(SDL_Event* event)
{
	newgame->raiseEvent(event);
	credits->raiseEvent(event);
}

void TitleState::draw()
{
	bg->draw(0, 0, frame);

	newgame->draw(frame);
	credits->draw(frame);
}