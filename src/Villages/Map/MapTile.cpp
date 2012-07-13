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

#include "MapTile.h"

#include <string>

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

using namespace std;

MapTile::MapTile(string src, int xloc, int yloc) : xloc(xloc), yloc(yloc)
{
	Logger::debug("MapTile Constructor");

	img = new Image(src, 255, 0, 255);
}

MapTile::~MapTile()
{
	delete img;

	Logger::debug("MapTile Destructor");
}

MapTile::MapTile(const MapTile& data)
{
	throw VillageException("MapTile Copy Constructor");
}

MapTile& MapTile::operator=(const MapTile* rhs)
{
	throw VillageException("MapTile Assignment Operator");
}