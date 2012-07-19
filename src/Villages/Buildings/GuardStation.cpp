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

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Building.h"

using namespace std;

GuardStation::GuardStation(int xloc, int yloc) : Building("GuardStationImage", xloc, yloc)
{
	Logger::debug("GuardStation Constructor");
}

GuardStation::~GuardStation()
{
	Logger::debug("GuardStation Destructor");
}

GuardStation::GuardStation(const GuardStation& data) : Building("", 0, 0)
{
	throw VillageException("GuardStation Copy Constructor");
}

GuardStation& GuardStation::operator=(const GuardStation* rhs)
{
	throw VillageException("GuardStation Assignment Operator");
}