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

#include "State.h"

#include "SDL.h"
#include "SDL_image.h"

#include "Engine/State/StateManager.h"
#include "Engine/Util/VillageException.h"

State::State(StateManager* manager, int width, int height, int xloc, int yloc) : width(width), height(height), xloc(xloc), yloc(yloc)
{ 
	showBehind = false;
	executeBehind = false;

	frame = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
	State::manager = manager;
}

State::~State()
{
	SDL_FreeSurface(frame);
}

State::State(const State& data)
{
	throw VillageException("Time Copy Constructor");
}

State& State::operator=(const State* rhs)
{
	throw VillageException("Time Assignment Operator");
}

void State::flip(SDL_Surface* screen)
{
	SDL_Rect offset;

	offset.x = xloc;
	offset.y = yloc;

	SDL_BlitSurface(frame, NULL, screen, &offset);
}