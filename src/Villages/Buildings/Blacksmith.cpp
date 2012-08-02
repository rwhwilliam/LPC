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

#include "Blacksmith.h"

#include <string>
#include <math.h>

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

Blacksmith::Blacksmith(SimState* state, int xloc, int yloc) : Building(state, "BlacksmithImage", xloc, yloc)
{
	Logger::debug("Blacksmith Constructor");

	capacity = 4;

	hover = new HoverImage(state, getMapX() + 44, getMapY() - 25, 200, 90, getMapX(), getMapY(), img->getWidth(), img->getHeight());
	hover->setScrolling(true);
	hover->addLine("workers", "Workers:");
	hover->addLine("coverage", "Protection:");
	hover->addLine("road", "Connected");
	hover->addLine("ore", "ore");
}

Blacksmith::~Blacksmith()
{
	Logger::debug("Blacksmith Destructor");
}

Blacksmith::Blacksmith(const Blacksmith& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("Blacksmith Copy Constructor");
}

Blacksmith& Blacksmith::operator=(const Blacksmith* rhs)
{
	throw VillageException("Blacksmith Assignment Operator");
}

void Blacksmith::generate()
{
	int ore = state->getCastle()->getOre();
	ore /= 5;

	ore = min(ore, (int)workers.size() * 2);

	state->getCastle()->takeOre(ore * 5);
	state->getCastle()->addWeapons(ore);

	Logger::debugFormat("Created %i Weapons", ore);
}

bool Blacksmith::canPurchase()
{
	return (state->getCastle()->getGold() >= 750 && state->getCastle()->getWood() >= 250);
}

void Blacksmith::purchase()
{
	state->getCastle()->takeGold(750);
	state->getCastle()->takeWood(250);
}

void Blacksmith::update(float time, Uint8* keystrokes)
{
	hover->editLine("workers", "Workers: " + toString(getWorkerCount()) + "/" + toString(getCapacity()));
	hover->editLine("coverage", "Protection: " + toString(getCoverage()) + "%");

	if(isRoadConnected())
		hover->editLine("road", "Connected to Castle");
	else
		hover->editLine("road", "NOT Connected to Castle");

	if(state->getCastle()->getOre() < getWorkerCount() * 5)
		hover->editLine("ore", "Running LOW on Ore!");
	else
		hover->editLine("ore", "Ore is plentiful");
}