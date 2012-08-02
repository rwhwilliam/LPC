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

#include "Mill.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Gui/HoverImage.h"
#include "Villages/States/SimState.h"

using namespace std;

Mill::Mill(SimState* state, int xloc, int yloc) : Building(state, "MillImage", xloc, yloc)
{
	Logger::debug("Mill Constructor");

	capacity = 10;

	hover = new HoverImage(state, getMapX() - 8, getMapY() + 125, 200, 70, getMapX(), getMapY(), img->getWidth(), img->getHeight());
	hover->setScrolling(true);
	hover->addLine("workers", "Workers:");
	hover->addLine("coverage", "Protection:");
	hover->addLine("road", "Connected");
}

Mill::~Mill()
{
	Logger::debug("Mill Destructor");
}

Mill::Mill(const Mill& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("Mill Copy Constructor");
}

Mill& Mill::operator=(const Mill* rhs)
{
	throw VillageException("Mill Assignment Operator");
}

void Mill::generate()
{
	int wood = workers.size() * 4;

	state->getCastle()->addWood(wood);

	Logger::debugFormat("Cut %i Wood", wood);
}

bool Mill::canPurchase()
{
	return (state->getCastle()->getGold() >= 150 && state->getCastle()->getWood() >= 25);
}

void Mill::purchase()
{
	state->getCastle()->takeGold(150);
	state->getCastle()->takeWood(25);
}

void Mill::update(float time, Uint8* keystrokes)
{
	hover->editLine("workers", "Workers: " + toString(getWorkerCount()) + "/" + toString(getCapacity()));
	hover->editLine("coverage", "Protection: " + toString(getCoverage()) + "%");

	if(isRoadConnected())
		hover->editLine("road", "Connected to Castle");
	else
		hover->editLine("road", "NOT Connected to Castle");
}