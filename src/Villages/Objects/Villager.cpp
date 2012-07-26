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

#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"

Villager::Villager(Building* job, House* residence)
{
	Villager::job = job;

	Villager::residence = residence;
}

Villager::~Villager()
{
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