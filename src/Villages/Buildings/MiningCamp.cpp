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

#include "MiningCamp.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/States/SimState.h"

using namespace std;

MiningCamp::MiningCamp(SimState* state, int xloc, int yloc) : Building(state, "MiningCampImage", xloc, yloc)
{
	Logger::debug("MiningCamp Constructor");

	capacity = 5;
}

MiningCamp::~MiningCamp()
{
	Logger::debug("MiningCamp Destructor");
}

MiningCamp::MiningCamp(const MiningCamp& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("MiningCamp Copy Constructor");
}

MiningCamp& MiningCamp::operator=(const MiningCamp* rhs)
{
	throw VillageException("MiningCamp Assignment Operator");
}

void MiningCamp::generate()
{
	Logger::debugFormat("Mined %i Ore", workers.size() * 2);

	state->getCastle()->addOre(workers.size() * 2);
}

bool MiningCamp::canPurchase()
{
	return (state->getCastle()->getGold() >= 500 && state->getCastle()->getWood() >= 100);
}

void MiningCamp::purchase()
{
	state->getCastle()->takeGold(500);
	state->getCastle()->takeWood(100);
}