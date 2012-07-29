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

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/State/State.h"
#include "Engine/State/StateManager.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

TitleState::TitleState(StateManager* manager, int width, int height, int xloc, int yloc) : State(manager, width, height, xloc, yloc)
{
	Logger::debug("TitleState Constructor");

	bg = new Image("title.png");
}

TitleState::~TitleState()
{
	delete bg;

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

void TitleState::update(float time, Uint8* keystrokes)
{

}

void TitleState::raiseEvent(SDL_Event* event)
{
	
}

void TitleState::draw()
{
	bg->draw(0, 0, frame);
}