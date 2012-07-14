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

#include "ForestTile.h"

#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/States/SimState.h"


ForestTile::ForestTile(SimState* state, int xloc, int yloc) : MapTile(state, Config::getConfig("ForestImage"), xloc, yloc)
{
	Logger::debug("ForestTile Constructor");
}

ForestTile::~ForestTile()
{
	Logger::debug("ForestTile Destructor");
}

ForestTile::ForestTile(const ForestTile& data) : MapTile(NULL, "", 0, 0)
{
	throw VillageException("ForestTile Copy Constructor");
}

ForestTile& ForestTile::operator=(const ForestTile* rhs)
{
	throw VillageException("ForestTile Assignment Operator");
}