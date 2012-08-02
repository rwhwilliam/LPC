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

#include "Farm.h"

#include <string>
#include <vector>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Gui/HoverImage.h"
#include "Villages/Objects/Villager.h"
#include "Villages/States/SimState.h"

using namespace std;

Farm::Farm(SimState* state, int xloc, int yloc) : Building(state, "FarmImage", xloc, yloc)
{
	Logger::debug("Farm Constructor");

	capacity = 20;

	hover = new HoverImage(state, getMapX() + 60, getMapY() + 25, 200, 70, getMapX(), getMapY(), img->getWidth(), img->getHeight());
	hover->setScrolling(true);
	hover->addLine("workers", "Workers:");
	hover->addLine("coverage", "Protection:");
	hover->addLine("road", "Connected");
}

Farm::~Farm()
{
	Logger::debug("Farm Destructor");
}

Farm::Farm(const Farm& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("Farm Copy Constructor");
}

Farm& Farm::operator=(const Farm* rhs)
{
	throw VillageException("Farm Assignment Operator");
}

void Farm::generate()
{
	int food = workers.size() * 5;

	state->getCastle()->addFood(food);

	Logger::debugFormat("Farmed %i Food", food);
}

bool Farm::canPurchase()
{
	return (state->getCastle()->getGold() >= 100);
}

void Farm::purchase()
{
	state->getCastle()->takeGold(100);
}

void Farm::update(float time, Uint8* keystrokes)
{
	hover->editLine("workers", "Workers: " + toString(getWorkerCount()) + "/" + toString(getCapacity()));
	hover->editLine("coverage", "Protection: " + toString(getCoverage()) + "%");

	if(isRoadConnected())
		hover->editLine("road", "Connected to Castle");
	else
		hover->editLine("road", "NOT Connected to Castle");
}