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

#include "Road.h"

#include "SDL.h"

#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"
#include "Villages/States/SimState.h"

Road::Road(SimState* state, int xloc, int yloc) : xloc(xloc), yloc(yloc)
{
	Road::state = state;

	img = new Image("road-vertical.png", (Uint8)255, 0, 255, state->getZoomLevel());

	//calculateMode();
}

Road::~Road()
{
	if(img != NULL)
		delete img;

	Logger::debug("Road Destructor");
}

Road::Road(const Road& data)
{
	throw VillageException("Road Copy Constructor");
}

Road& Road::operator=(const Road* rhs)
{
	throw VillageException("Road Assignment Operator");
}

void Road::resize()
{
	img->setScale(state->getZoomLevel());
}

void Road::calculateMode()
{
	if(img != NULL)
		delete img;

	//default (if n,s,e,w == null)
	img = new Image("road-vertical.png", 255, 0, 255);

	Road* n = getRoad(xloc, yloc - 1);
	Road* s = getRoad(xloc, yloc + 1);
	Road* e = getRoad(xloc + 1, yloc);
	Road* w = getRoad(xloc - 1, yloc);

	if(n != NULL && s == NULL && e == NULL && w == NULL)
		img = new Image("road-end-down.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n == NULL && s != NULL && e == NULL && w == NULL)
		img = new Image("road-end-up.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n == NULL && s == NULL && e != NULL && w == NULL)
		img = new Image("road-end-left.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n == NULL && s == NULL && e == NULL && w != NULL)
		img = new Image("road-end-right.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n != NULL && s != NULL && e == NULL && w == NULL)
		img = new Image("road-vertical.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n == NULL && s == NULL && e != NULL && w != NULL)
		img = new Image("road-horizontal.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n != NULL && s == NULL && e != NULL && w == NULL)
		img = new Image("road-turn-3.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n != NULL && s == NULL && e == NULL && w != NULL)
		img = new Image("road-turn-4.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n == NULL && s != NULL && e != NULL && w == NULL)
		img = new Image("road-turn-2.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n == NULL && s != NULL && e == NULL && w != NULL)
		img = new Image("road-turn-1.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n == NULL && s != NULL && e != NULL && w != NULL)
		img = new Image("road-intersection-down.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n != NULL && s == NULL && e != NULL && w != NULL)
		img = new Image("road-intersection-up.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n != NULL && s != NULL && e == NULL && w != NULL)
		img = new Image("road-intersection-left.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n != NULL && s != NULL && e != NULL && w == NULL)
		img = new Image("road-intersection-right.png", (Uint8)255, 0, 255, state->getZoomLevel());
	if(n != NULL && s != NULL && e != NULL && w != NULL)
		img = new Image("road-intersection.png", (Uint8)255, 0, 255, state->getZoomLevel());

	/*if(mode == R_VERTICAL)
	{
		img = new Image("road-vertical.png", 255, 0, 255);
	}
	else if(mode == R_HORIZONTAL)
	{
		img = new Image("road-horizontal.png", 255, 0, 255);
	}*/
}

Road* Road::getRoad(int x, int y)
{
	map<string, Road*>::const_iterator it = (*state->getRoads()).find(toString(x) + "_" + toString(y));
	
	if(it != (*state->getRoads()).end())
		return it->second;

	return NULL;
}

void Road::getSurroundingRoads(list<Road*>& temp)
{
	Road* n = getRoad(xloc, yloc - 1);
	Road* s = getRoad(xloc, yloc + 1);
	Road* e = getRoad(xloc + 1, yloc);
	Road* w = getRoad(xloc - 1, yloc);

	if(n != NULL)
		temp.push_back(n);

	if(s != NULL)
		temp.push_back(s);

	if(e != NULL)
		temp.push_back(e);

	if(w != NULL)
		temp.push_back(w);
}

void Road::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	img->draw(getMapX() - xoffset, getMapY() - yoffset, screen);
}

bool Road::collides(int x, int y, int width, int height)
{
	return (getMapX() + getWidth() - state->getXOffset() > x && getMapX() - state->getXOffset() < x + width && getMapY() + getHeight() - state->getYOffset() > y && getMapY() - state->getYOffset() < y + height);
}

int Road::getMapX()
{
	return xloc * state->getTileWidth();
}

int Road::getMapY()
{
	return yloc * state->getTileHeight();
}