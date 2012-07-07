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

#include "SDL.h"

#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

ScrollingMap::ScrollingMap(int width, int height, int tileWidth, int tileHeight, int layerCount) : TileMap(width, height, tileWidth, tileHeight, layerCount)
{
	Logger::debugFormat("ScrollingMap Constructor");

	xoffset = 0;
	yoffset = 0;
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
	if(event->type == SDL_MOUSEMOTION)
	{
		if(event->motion.x <= 200)
		{
			xoffset = (xoffset - 5 < 0) ? 0 : xoffset - 5;
		}
		if(event->motion.y <= 200)
		{
			yoffset = (yoffset - 5 < 0) ? 0 : yoffset - 5;
		}
		if(event->motion.x >= atoi(Config::getConfig("ScreenWidth").c_str()) - 200)
		{
			int temp = width * tileWidth - atoi(Config::getConfig("ScreenWidth").c_str());

			xoffset = (xoffset + 5 > temp) ? temp : xoffset + 5;
		}
		if(event->motion.y >= atoi(Config::getConfig("ScreenHeight").c_str()) - 200)
		{
			int temp = height * tileHeight - atoi(Config::getConfig("ScreenHeight").c_str());

			yoffset = (yoffset + 5 > temp) ? temp : yoffset + 5;
		}
	}
}

void ScrollingMap::draw(SDL_Surface* screen)
{
	TileMap::draw(xoffset, yoffset, screen);
}