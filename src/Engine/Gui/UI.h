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

#ifndef UI_H
#define UI_H

#include <map>
#include <string>

#include "SDL.h"

#include "Component.h"

using namespace std;

class Image;

class UI : public Component
{
public:
	UI(int x, int y, int width, int height, string backgroundSrc);
	UI(int x, int y, int width, int height, string backgroundSrc, Uint8 r, Uint8 g, Uint8 b);
	~UI();

	UI(const UI& data);
	UI& operator=(const UI* rhs);

	void raiseEvent(SDL_Event* event);
	void draw(SDL_Surface* screen);

	void addComponent(string src, Component* comp);
	Component* getComponent(string src);
	void removeComponent(string src);

private:
	map<string, Component*> subcomponents;
	Image* background;
};

#endif