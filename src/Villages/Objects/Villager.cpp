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

#include "Villager.h"

#include <math.h>
#include <stdlib.h>

#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Buildings/House.h"
#include "Villages/States/SimState.h"

using namespace std;

Villager::Villager(SimState* simstate, Building* job, House* residence)
{
	Villager::job = job;

	Villager::residence = residence;

	Villager::simstate = simstate;
}

Villager::~Villager()
{
	job->removeWorker(this);
	residence->removeWorker(this);

	Villager::job = NULL;
	Villager::residence = NULL;
}

Villager::Villager(const Villager& data)
{
	throw VillageException("Villager Copy Constructor");
}

Villager& Villager::operator=(const Villager* rhs)
{
	throw VillageException("Villager Assignment Operator");
}

void Villager::setJob(Building* job)
{
	job->addWorker(this);
	Villager::job = job;
}

void Villager::setResidence(House* residence)
{
	residence->addWorker(this);
	Villager::residence = residence;
}

bool Villager::wantsToLeave()
{
	int random = rand() % 5000 + 1;

	if(random <= pow(simstate->getCastle()->getTaxRate(), 2.0))
		return true;

	if(simstate->getCastle()->getFood() <= 0)
	{
		random = rand() % 100 + 1;
		
		if(random <= 1)
			return true;
	}

	if(simstate->getSpareWater() <= 0)
	{
		random = rand() % 100 + 1;
		
		if(random <= 1)
			return true;
	}

	int coverage = residence->getCoverate();
	if(coverage < 75)
	{
		random = rand() % 75 + 1;

		if(random > coverage)
			return true;
	}

	return false;
}