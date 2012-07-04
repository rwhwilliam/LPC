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

#include "Button.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"


using namespace std;

Button::Button(int x, int y, int width, int height, string normalSrc, string hoverSrc, string downSrc) : Component(x, y, width, height)
{
	Logger::debug("Button Constructor");

	normalBackground = new Image(normalSrc);
	hoverBackground = new Image(hoverSrc);
	downBackground = new Image(downSrc);

	state = NORMAL;
}

Button::Button(int x, int y, int width, int height, string normalSrc, string hoverSrc, string downSrc, Uint8 r, Uint8 g, Uint8 b) : Component(x, y, width, height)
{
	Logger::debug("Button Constructor");

	normalBackground = new Image(normalSrc, r, g, b);
	hoverBackground = new Image(hoverSrc, r, g, b);
	downBackground = new Image(downSrc, r, g, b);

	state = NORMAL;
}

Button::~Button()
{
	delete normalBackground;
	delete hoverBackground;
	delete downBackground;

	Logger::debug("Button Destructor");
}

Button::Button(const Button& data)
{
	throw VillageException("Button Copy Constructor");
}

Button& Button::operator=(const Button* rhs)
{
	throw VillageException("Button Assignment Operator");
}

void Button::raiseEvent(SDL_Event* event)
{
	if(active)
	{
		int x = 0, y = 0;

		if(event->type == SDL_MOUSEMOTION)
		{
			x = event->motion.x;
			y = event->motion.y;

			if((x >= Button::x) && (x <= Button::x + Button::width) && (y >= Button::y) && (y <= Button::y + Button::height))
			{
				state = HOVER;
			}
			else
			{
				state = NORMAL;
			}
		}

		if(event->type == SDL_MOUSEBUTTONDOWN)
		{
			if(event->button.button == SDL_BUTTON_LEFT)
			{
				x = event->button.x;
				y = event->button.y;

				if((x >= Button::x) && (x <= Button::x + Button::width) && (y >= Button::y) && (y <= Button::y + Button::height))
				{
					state = DOWN;
				}
			}
		}

		if(event->type == SDL_MOUSEBUTTONUP)
		{
			if(event->button.button == SDL_BUTTON_LEFT)
			{
				x = event->button.x;
				y = event->button.y;

				if((x >= Button::x) && (x <= Button::x + Button::width) && (y >= Button::y) && (y <= Button::y + Button::height))
				{
					if(state == DOWN)
						click();

					state = HOVER;
				}
				else
				{
					state = NORMAL;
				}
			}
		}
	}
}

void Button::draw(SDL_Surface* screen)
{
	if(active)
	{
		switch(state)
		{
		case NORMAL:
			normalBackground->draw(x, y, screen);
			break;
		case HOVER:
			hoverBackground->draw(x, y, screen);
			break;
		case DOWN:
			downBackground->draw(x, y, screen);
			break;
		}
	}
}