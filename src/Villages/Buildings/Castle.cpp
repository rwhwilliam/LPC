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

#include "Castle.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/States/SimState.h"

using namespace std;

Castle::Castle(SimState* state, int xloc, int yloc) : Building(state, "CastleImage", xloc, yloc)
{
	Logger::debug("Castle Constructor");

	tax = 5;
}

Castle::~Castle()
{
	Logger::debug("Castle Destructor");
}

Castle::Castle(const Castle& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("Castle Copy Constructor");
}

Castle& Castle::operator=(const Castle* rhs)
{
	throw VillageException("Castle Assignment Operator");
}

void Castle::setTaxRate(int val)
{
	if(val > 100)
		val = 100;
	if(val < 0)
		val = 0;

	tax = val;

	Logger::debugFormat("Setting tax rate to %i percent", tax);
}

int Castle::takeGold(int val)
{
	Logger::debugFormat("Attempting to take %i gold", val);

	if(val > gold)
	{
		Logger::debugFormat("Could only take %i gold", gold);

		val = gold;
		gold = 0;
		return val;
	}

	gold -= val;
	return val;
}

int Castle::takeFood(int val)
{
	Logger::debugFormat("Attempting to take %i food", val);

	if(val > food)
	{
		Logger::debugFormat("Could only take %i food", food);

		val = food;
		food = 0;
		return val;
	}

	food -= val;
	return val;
}

int Castle::takeWood(int val)
{
	Logger::debugFormat("Attempting to take %i wood", val);

	if(val > wood)
	{
		Logger::debugFormat("Could only take %i wood", wood);

		val = wood;
		wood = 0;
		return val;
	}

	wood -= val;
	return val;
}

int Castle::takeOre(int val)
{
	Logger::debugFormat("Attempting to take %i ore", val);

	if(val > ore)
	{
		Logger::debugFormat("Could only take %i ore", ore);

		val = ore;
		ore = 0;
		return val;
	}

	ore -= val;
	return val;
}

int Castle::takeWeapons(int val)
{
	Logger::debugFormat("Attempting to take %i weapons", val);

	if(val > weapons)
	{
		Logger::debugFormat("Could only take %i weapons", weapons);

		val = weapons;
		weapons = 0;
		return val;
	}

	weapons -= val;
	return val;
}