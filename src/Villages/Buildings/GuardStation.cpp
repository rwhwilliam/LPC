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

#include "GuardStation.h"

#include <string>

#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/States/SimState.h"

using namespace std;

GuardStation::GuardStation(SimState* state, int xloc, int yloc) : Building(state, "GuardStationImage", xloc, yloc)
{
	Logger::debug("GuardStation Constructor");

	coverageDiameter = 8;
	value = 0;
}

GuardStation::~GuardStation()
{
	Logger::debug("GuardStation Destructor");
}

GuardStation::GuardStation(const GuardStation& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("GuardStation Copy Constructor");
}

GuardStation& GuardStation::operator=(const GuardStation* rhs)
{
	throw VillageException("GuardStation Assignment Operator");
}

void GuardStation::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	int bX = (xloc - coverageDiameter) * state->getTileWidth() - xoffset;
	int bY = (yloc - coverageDiameter) * state->getTileHeight() - yoffset;
	boxRGBA(screen, bX, bY, 
		bX + (coverageDiameter * 2) * state->getTileWidth() + getWidth(),
		bY + (coverageDiameter * 2) * state->getTileHeight() + getHeight(), 
		0, 0, 255, 20);

	img->draw(getMapX() - xoffset, getMapY() - yoffset, screen);
}

int GuardStation::inCoverage(Building* bldg)
{
	int bX = (xloc - coverageDiameter) * state->getTileWidth() - state->getXOffset();
	int bY = (yloc - coverageDiameter) * state->getTileHeight() - state->getYOffset();

	if(bldg->collides(bX, bY, 
		(coverageDiameter * 2) * state->getTileWidth() + getWidth(),
		(coverageDiameter * 2) * state->getTileHeight() + getHeight()))
	{
		return value;
	}

	return 0;
}

void GuardStation::generate()
{
	if(state->getCastle()->getWeapons() > 0)
	{
		state->getCastle()->takeWeapons(1);

		value = 100;
	}
	else
	{
		value -= 5;

		if(value < 50)
			value = 50;
	}
}

bool GuardStation::canPurchase()
{
	return (state->getCastle()->getGold() >= 250);
}

void GuardStation::purchase()
{
	state->getCastle()->takeGold(250);
}