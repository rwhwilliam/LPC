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

#include "Building.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Map/MapTile.h"

using namespace std;

Building::Building(SimState* state, string src, int xloc, int yloc) : xloc(xloc), yloc(yloc)
{
	Logger::debug("Building Constructor");

	Building::state = state;

	img = new Image(Config::getConfig(src), 255, 0, 255);

	width = img->getWidth();
	height = img->getHeight();
}

Building::~Building()
{
	delete img;

	Logger::debug("Building Destructor");
}

Building::Building(const Building& data)
{
	throw VillageException("Building Copy Constructor");
}

Building& Building::operator=(const Building* rhs)
{
	throw VillageException("Building Assignment Operator");
}

void Building::update(float time, Uint8* keystrokes)
{

}

void Building::raiseEvent(SDL_Event* event)
{

}

void Building::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	img->draw(getMapX() - xoffset, getMapY() - yoffset, screen);
}

bool Building::collides(int x, int y, int width, int height)
{
	return (getMapX() + Building::width - state->getXOffset() > x && getMapX() - state->getXOffset() < x + width && getMapY() + Building::height - state->getYOffset() > y && getMapY() - state->getYOffset() < y + height);
}

int Building::getMapX()
{
	return xloc * state->getTileWidth();
}

int Building::getMapY()
{
	return yloc * state->getTileHeight();
}