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

#include "Hover.h"

#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#include "Engine/Graphics/Font.h"
#include "Engine/Time/Timer.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/States/SimState.h"

Hover::Hover(SimState* state, int x, int y, int width, int height, int targetX, int targetY, int targetWidth, int targetHeight) : xloc(x), yloc(y), width(width), height(height)
{
	Logger::debug("Hover Constructor");

	Hover::state = state;

	target.x = targetX;
	target.y = targetY;
	target.w = targetWidth;
	target.h = targetHeight;

	active = false;

	timer = new Timer();

	font = new Font("lazy.ttf", 15, 255, 255, 255);
}

Hover::~Hover()
{
	delete timer;
	delete font;

	vector<SDL_Surface*>::iterator it;
	for(it = lines.begin(); it != lines.end(); ++it)
		SDL_FreeSurface(*it);

	Logger::debug("Hover Destructor");
}

Hover::Hover(const Hover& data)
{
	throw VillageException("Hover Copy Constructor");
}

Hover& Hover::operator=(const Hover* rhs)
{
	throw VillageException("Hover Assignment Operator");
}

void Hover::addLine(string line)
{
	lines.push_back(font->getSurface(line));
}

void Hover::raiseEvent(SDL_Event* event)
{
	if(event->type == SDL_MOUSEMOTION)
	{
		int x = 0, y = 0;

		if(scrolling)
		{
			x += state->getXOffset();
			y += state->getYOffset();
		}

		x += event->motion.x;
		y += event->motion.y;

		if((x >= target.x * state->getZoomLevel() - state->getXOffset()) && (x <= (target.x + target.w) * state->getZoomLevel() - state->getXOffset()) && (y >= target.y * state->getZoomLevel() - state->getYOffset()) && (y <= (target.y + target.h) * state->getZoomLevel() - state->getYOffset()))
		{
			active = true;
			timer->start();
		}
		else
		{
			active = false;
			timer->stop();
		}
	}
}

void Hover::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	if(active && timer->get_ticks() >= 500)
	{
		boxRGBA(screen, xloc * state->getZoomLevel() - xoffset, yloc * state->getZoomLevel() - yoffset, xloc * state->getZoomLevel() + width - xoffset, yloc * state->getZoomLevel() + height - yoffset, 225, 206, 46, 25);

		vector<SDL_Surface*>::const_iterator it;
		for(it != lines.begin(); it != lines.end(); ++it)
			font->draw(10 + xloc * state->getZoomLevel() - xoffset, 10 + yloc * state->getZoomLevel() - yoffset, *it, screen);
	}
}