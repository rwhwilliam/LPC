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

#include "Wonder.h"

#include <string>

#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Objects/Road.h"
#include "Villages/States/SimState.h"

using namespace std;

Wonder::Wonder(SimState* state, int xloc, int yloc) : Building(state, "WonderImage", xloc, yloc)
{
	Logger::debug("Wonder Constructor");

	capacity = 500;
	workerTurns = 5000;
}

Wonder::~Wonder()
{
	Logger::debug("Wonder Destructor");
}

Wonder::Wonder(const Wonder& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("Wonder Copy Constructor");
}

Wonder& Wonder::operator=(const Wonder* rhs)
{
	throw VillageException("Wonder Assignment Operator");
}

bool Wonder::canPurchase()
{
	return (state->getCastle()->getGold() >= 50000 &&
		state->getCastle()->getWood() >= 10000 &&
		state->getCastle()->getOre() >= 5000);
}

void Wonder::purchase()
{
	state->getCastle()->takeGold(50000);
	state->getCastle()->takeWood(10000);
	state->getCastle()->takeOre(5000);
}

void Wonder::generate(list<Road*>& network)
{
	Building::generate(network);
}

void Wonder::generate()
{
	workerTurns -= workers.size();
}