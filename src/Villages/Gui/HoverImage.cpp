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

#include "HoverImage.h"

#include <algorithm>

#include "Engine/Graphics/Font.h"
#include "Engine/Graphics/Image.h"
#include "Engine/Time/Timer.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Gui/Hover.h"
#include "Villages/States/SimState.h"

using namespace std;

HoverImage::HoverImage(SimState* state, int x, int y, int width, int height, int targetX, int targetY, int targetWidth, int targetHeight) : Hover(state, x, y, width, height, targetX, targetY, targetWidth, targetHeight)
{
	Logger::debug("HoverImage Constructor");
}

HoverImage::~HoverImage()
{
	map<pair<int, int>, Image*>::iterator it;
	for(it = images.begin(); it != images.end(); ++it)
	{
		delete it->second;
	}
	images.clear();

	Logger::debug("HoverImage Destructor");
}

HoverImage::HoverImage(const HoverImage& data) : Hover(NULL, 0, 0, 0, 0, 0, 0, 0, 0)
{
	throw VillageException("HoverImage Copy Constructor");
}

HoverImage& HoverImage::operator=(const HoverImage* rhs)
{
	throw VillageException("HoverImage Assignment Operator");
}

void HoverImage::addLine(string line)
{
	Hover::addLine(line);
}

void HoverImage::addLine(string key, string line)
{
	SDL_Surface* temp = font->getSurface(line);
	lines[key] = temp;
	linelist.push_back(temp);
}

void HoverImage::editLine(string key, string line)
{
	if(lines[key] != NULL)
	{
		list<SDL_Surface*>::iterator it = find(linelist.begin(), linelist.end(), lines[key]);

		if(it != linelist.end())
		{
			SDL_Surface* temp = font->getSurface(line);
			SDL_FreeSurface(lines[key]);
			lines[key] = temp;
			*it = temp;
		}
	}
}

void HoverImage::addImage(int x, int y, Image* img)
{
	pair<int, int> xy (x, y);
	images[xy] = img;
}

void HoverImage::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	Hover::draw(xoffset, yoffset, screen);

	if(active && timer->get_ticks() >= 250)
	{
		map<pair<int, int>, Image*>::const_iterator it;
		for(it = images.begin(); it != images.end(); ++it)
		{
			if(scrolling)
			{
				it->second->draw(xloc * state->getZoomLevel() - xoffset + it->first.first, yloc * state->getZoomLevel() - yoffset + it->first.second, screen);
			}
			else
			{
				it->second->draw(xloc + it->first.first, yloc + it->first.second, screen);
			}
		}
	}
}