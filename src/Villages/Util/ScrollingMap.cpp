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

#include "ScrollingMap.h"

#include <iostream>
#include <stdlib.h>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/States/SimState.h"

using namespace std;

ScrollingMap::ScrollingMap(SimState* state, int width, int height, int tileWidth, int tileHeight, int layerCount) : TileMap(width, height, tileWidth, tileHeight, layerCount)
{
	Logger::debugFormat("ScrollingMap Constructor");

	xoffset = tileWidth * 100;
	yoffset = tileHeight * 50;

	ScrollingMap::state = state;
}

ScrollingMap::~ScrollingMap()
{
	Logger::debugFormat("ScrollingMap Destructor");
}

ScrollingMap::ScrollingMap(const ScrollingMap& data) : TileMap(0, 0, 0, 0, 0)
{
	throw VillageException("ScrollingMap Copy Constructor");
}

ScrollingMap& ScrollingMap::operator=(const ScrollingMap* rhs)
{
	throw VillageException("ScrollingMap Assignment Operator");
}

void ScrollingMap::raiseEvent(SDL_Event* event)
{
	/*if(event->type == SDL_MOUSEMOTION)
	{
		int speed = 10;

		if(event->motion.x <= 150)
		{
			xoffset = (xoffset - speed < 0) ? 0 : xoffset - speed;
		}
		if(event->motion.y <= 150)
		{
			yoffset = (yoffset - speed < 0) ? 0 : yoffset - speed;
		}
		if(event->motion.x >= atoi(Config::getConfig("ScreenWidth").c_str()) - 150)
		{
			int temp = width * tileWidth - atoi(Config::getConfig("ScreenWidth").c_str());

			xoffset = (xoffset + speed > temp) ? temp : xoffset + speed;
		}
		if(event->motion.y >= atoi(Config::getConfig("ScreenHeight").c_str()) - 150)
		{
			int temp = height * tileHeight - atoi(Config::getConfig("ScreenHeight").c_str());

			yoffset = (yoffset + speed > temp) ? temp : yoffset + speed;
		}
	}*/
}

void ScrollingMap::update(float time, Uint8* keystates)
{
	int speed = 20;

	if(keystates[SDLK_w])
	{
		yoffset = (yoffset - speed < 0) ? 0 : yoffset - speed;
	}
	if(keystates[SDLK_s])
	{
		int temp = height * (tileHeight * 2) - atoi(Config::getConfig("ScreenHeight").c_str());

		yoffset = (yoffset + speed > temp) ? temp : yoffset + speed;
	}
	if(keystates[SDLK_a])
	{
		xoffset = (xoffset - speed < 0) ? 0 : xoffset - speed;
	}
	if(keystates[SDLK_d])
	{
		int temp = width * (tileWidth * 2) - atoi(Config::getConfig("ScreenWidth").c_str());

		xoffset = (xoffset + speed > temp) ? temp : xoffset + speed;
	}
}

void ScrollingMap::setXOffset(int val)
{
	int temp = width * (tileWidth * 2) - atoi(Config::getConfig("ScreenWidth").c_str());

	xoffset = (val > temp) ? temp : val;
	xoffset = (xoffset < 0) ? 0 : xoffset;
}

void ScrollingMap::setYOffset(int val)
{
	int temp = height * (tileHeight * 2) - atoi(Config::getConfig("ScreenHeight").c_str());

	yoffset = (val > temp) ? temp : val;
	yoffset = (yoffset < 0) ? 0 : yoffset;
}

void ScrollingMap::draw(SDL_Surface* screen)
{
	TileMap::draw(xoffset, yoffset, screen);
}

void ScrollingMap::resize()
{
	map<int, Image*>::iterator it;
	for(it = tiles.begin(); it != tiles.end(); ++it)
	{
		it->second->setScale(state->getZoomLevel());
	}
}