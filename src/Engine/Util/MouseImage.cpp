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

#include "MouseImage.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

using namespace std;

MouseImage::MouseImage(string goodSrc, string badSrc, int alpha) : x(0), y(0)
{
	Logger::debug("MouseImage Constructor");

	goodImg = new Image(goodSrc, alpha);
	badImg = new Image(badSrc, alpha);

	mode = MI_NORMAL;
}

MouseImage::~MouseImage()
{
	delete goodImg;
	delete badImg;

	Logger::debug("MouseImage Destructor");
}

MouseImage::MouseImage(const MouseImage& data)
{
	throw VillageException("MouseImage Copy Constructor");
}

MouseImage& MouseImage::operator=(const MouseImage* rhs)
{
	throw VillageException("MouseImage Assignment Operator");
}

void MouseImage::raiseEvent(SDL_Event* event)
{
	if(event->type == SDL_MOUSEMOTION)
	{
		x = event->motion.x - goodImg->getWidth() / 2;
		y = event->motion.y - goodImg->getHeight() / 2;
	}
}

void MouseImage::draw(SDL_Surface* screen)
{
	if(mode == MI_NORMAL)
		goodImg->draw(x, y, screen);
	else
		badImg->draw(x, y, screen);
}