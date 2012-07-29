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

#include "House.h"

#include <string>
#include <vector>
#include <math.h>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Objects/Villager.h"
#include "Villages/States/SimState.h"

using namespace std;

House::House(SimState* state, int xloc, int yloc) : Building(state, "HouseImage", xloc, yloc)
{
	Logger::debug("House Constructor");

	capacity = 10; 
}

House::~House()
{
	Logger::debug("House Destructor");
}

House::House(const House& data) : Building(NULL, "", 0, 0)
{
	throw VillageException("House Copy Constructor");
}

House& House::operator=(const House* rhs)
{
	throw VillageException("House Assignment Operator");
}

int House::getRating()
{
	int rating = 0;

	int minAvg = 0, count = 0;
	vector<Building*>::const_iterator it;
	for(it = state->getBuildings()->begin(); it != state->getBuildings()->end(); ++it)
	{
		if((*it)->getType() == BT_MININGCAMP)
		{
			minAvg += (int)sqrt(pow(xloc - (*it)->getX(), 2.0) + pow(yloc - (*it)->getY(), 2.0));
			count++;
		}
	}

	rating += minAvg / count;

	rating += 150 - (int)sqrt(pow(xloc - state->getCastle()->getX(), 2.0) + pow(yloc - state->getCastle()->getY(), 2.0));

	rating += coverage / 2;

	Logger::debugFormat("House rating of %i", rating);

	if(roadConnected)
		return rating;
	else
		return 1;
}