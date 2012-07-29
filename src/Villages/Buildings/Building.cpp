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
#include <list>
#include <map>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Map/MapTile.h"
#include "Villages/Objects/Road.h"
#include "Villages/Objects/Villager.h"
#include "Villages/Util/Util.h"

using namespace std;

Building::Building(SimState* state, string src, int xloc, int yloc) : xloc(xloc), yloc(yloc)
{
	Logger::debug("Building Constructor");

	Building::state = state;

	img = new Image(Config::getConfig(src), (Uint8)255, 0, 255, state->getZoomLevel());

	capacity = 0;
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

void Building::resize()
{
	img->setScale(state->getZoomLevel());
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
	return (getMapX() + getWidth() - state->getXOffset() > x && getMapX() - state->getXOffset() < x + width && getMapY() + getHeight() - state->getYOffset() > y && getMapY() - state->getYOffset() < y + height);
}

int Building::getMapX()
{
	return xloc * state->getTileWidth();
}

int Building::getMapY()
{
	return yloc * state->getTileHeight();
}

void Building::addWorker(Villager* person)
{
	if(hasRoom())
	{
		Logger::debug("Worker Added");

		workers.push_back(person);
	}
	else
	{
		Logger::error("Adding worker to full building!");
	}
}

void Building::removeWorker(Villager* person)
{
	Logger::debug("Worker Removed");

	workers.remove(person);
}

void Building::getSurroundingRoads(list<Road*>& temp)
{
	int w = getWidth() / state->getTileWidth();
	int h = getHeight() / state->getTileHeight();

	//top
	for(int i = xloc - 1; i <= xloc + w + 1; ++i)
	{
		map<string, Road*>::iterator rit = state->getRoads()->find(toString(i) + "_" + toString(yloc - 1));
		if(rit != state->getRoads()->end())
			temp.push_back((*state->getRoads())[toString(i) + "_" + toString(yloc - 1)]);
	}

	//bottom
	for(int i = xloc - 1; i <= xloc + w + 1; ++i)
	{
		map<string, Road*>::iterator rit = state->getRoads()->find(toString(i) + "_" + toString(yloc + h));
		if(rit != state->getRoads()->end())
			temp.push_back((*state->getRoads())[toString(i) + "_" + toString(yloc + h)]);
	}

	//left
	for(int i = yloc - 1; i <= yloc + h + 1; ++i)
	{
		map<string, Road*>::iterator rit = state->getRoads()->find(toString(xloc - 1) + "_" + toString(i));
		if(rit != state->getRoads()->end())
			temp.push_back((*state->getRoads())[toString(xloc - 1) + "_" + toString(i)]);
	}

	//right
	for(int i = yloc - 1; i <= yloc + h + 1; ++i)
	{
		map<string, Road*>::iterator rit = state->getRoads()->find(toString(xloc + w) + "_" + toString(i));
		if(rit != state->getRoads()->end())
			temp.push_back((*state->getRoads())[toString(xloc + w) + "_" + toString(i)]);
	}
}

bool Building::inNetwork(list<Road*>& network)
{
	list<Road*> temp;

	getSurroundingRoads(temp);

	list<Road*>::iterator it;
	list<Road*>::iterator it2;
	for(it = temp.begin(); it != temp.end(); ++it)
		for(it2 = network.begin(); it2 != network.end(); ++it2)
			if(find(*it, *it2))
				return true;

	return false;
}

void Building::generate(list<Road*>& network)
{
	if(inNetwork(network))
	{
		generate();
	}
	else
	{
		Logger::debugFormat("Building at (%i, %i) can not produce because it is not attached to the castle via a road!", xloc, yloc);
	}
}