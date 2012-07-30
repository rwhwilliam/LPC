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

#include "IncrementBox.h"

#include <string>

#include "Engine/Gui/ClickableButton.h"
#include "Engine/Gui/Component.h"
#include "Engine/Gui/Textbox.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"

using namespace std;

IncrementBox::IncrementBox(int x, int y, int width, int height, int min, int max) : Component(x, y, width, height), min(min), max(max)
{
	box = new Textbox(x, y, 100, 32, "textbox-small.png", "lazy.ttf", 22);
	up = new ClickableButton<IncrementBox>(x + 96, y, 32, 16, "arrow-up.png", "arrow-up.png", "arrow-up.png", this, &IncrementBox::increment);
	down = new ClickableButton<IncrementBox>(x + 96, y + 16, 32, 16, "arrow-down.png", "arrow-down.png", "arrow-down.png", this, &IncrementBox::decrement);

	current = min;

	box->setContent(toString(current));
}

IncrementBox::IncrementBox(int x, int y, int width, int height, int min, int max, int val) : Component(x, y, width, height), min(min), max(max)
{
	box = new Textbox(x, y, 100, 32, "textbox-small.png", "lazy.ttf", 22);
	up = new ClickableButton<IncrementBox>(x + 96, y, 32, 16, "arrow-up.png", "arrow-up.png", "arrow-up.png", this, &IncrementBox::increment);
	down = new ClickableButton<IncrementBox>(x + 96, y + 16, 32, 16, "arrow-down.png", "arrow-down.png", "arrow-down.png", this, &IncrementBox::decrement);

	current = val;

	box->setContent(toString(current));
}

IncrementBox::~IncrementBox()
{
	delete up;
	delete down;
	delete box;
}

IncrementBox::IncrementBox(const IncrementBox& data) : Component(0, 0, 0, 0)
{
	throw VillageException("IncrementBox Copy Constructor");
}

IncrementBox& IncrementBox::operator=(const IncrementBox* rhs)
{
	throw VillageException("IncrementBox Assignment Operator");
}

void IncrementBox::increment()
{
	current = (current >= max) ? max : current + 1;

	box->setContent(toString(current));
}

void IncrementBox::decrement()
{
	current = (current <= min) ? min : current - 1;

	box->setContent(toString(current));
}

void IncrementBox::raiseEvent(SDL_Event* event)
{
	box->raiseEvent(event);
	up->raiseEvent(event);
	down->raiseEvent(event);
}

void IncrementBox::draw(SDL_Surface* screen)
{
	box->draw(screen);
	up->draw(screen);
	down->draw(screen);
}