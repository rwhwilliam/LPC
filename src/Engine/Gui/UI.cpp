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

#include "UI.h"

#include <map>
#include <string>

#include "SDL.h"

#include "Component.h"
#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

using namespace std;

UI::UI(int x, int y, int width, int height, string backgroundSrc) : Component(x, y, width, height)
{
	Logger::debug("UI Constructor");

	if(backgroundSrc != "")
		background = new Image(backgroundSrc);
	else
		background = NULL;
}

UI::UI(int x, int y, int width, int height, string backgroundSrc, Uint8 r, Uint8 g, Uint8 b) : Component(x, y, width, height)
{
	Logger::debug("UI Constructor");

	background = new Image(backgroundSrc, r, g, b);
}

UI::~UI()
{
	delete background;

	map<string, Component*>::iterator it;

	for(it = subcomponents.begin(); it != subcomponents.end(); ++it)
		delete it->second;

	subcomponents.clear();

	Logger::debug("UI Destructor");
}

UI::UI(const UI& data)
{
	throw VillageException("UI Copy Constructor");
}

UI& UI::operator=(const UI* rhs)
{
	throw VillageException("UI Assignment Operator");
}

void UI::raiseEvent(SDL_Event* event)
{
	if(active)
	{
		map<string, Component*>::iterator it;

		for(it = subcomponents.begin(); it != subcomponents.end(); ++it)
			it->second->raiseEvent(event);
	}
}

void UI::draw(SDL_Surface* screen)
{
	if(active)
	{
		map<string, Component*>::iterator it;

		for(it = subcomponents.begin(); it != subcomponents.end(); ++it)
			it->second->draw(screen);
	}
}

void UI::addComponent(string src, Component* comp)
{
	map<string, Component*>::iterator it;

	it = subcomponents.find(src);

	if(it == subcomponents.end())
	{
		subcomponents[src] = comp;
	}
}

Component* UI::getComponent(string src)
{
	map<string, Component*>::iterator it;

	it = subcomponents.find(src);

	if(it != subcomponents.end())
	{
		return subcomponents[src];
	}

	return NULL;
}

void UI::removeComponent(string src)
{
	map<string, Component*>::iterator it;

	it = subcomponents.find(src);

	if(it != subcomponents.end())
	{
		delete it->second;
		subcomponents.erase(it);
	}
}