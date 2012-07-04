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

#ifndef COMPONENT_H
#define COMPONENT_H

#include "SDL.h"

class Image;

class Component
{
public:
	virtual void raiseEvent(SDL_Event* event) = 0;
	virtual void draw(SDL_Surface* screen) = 0;

	bool getActive() { return active; }
	void setActive(bool active) { Component::active = active; }

protected:
	Component();
	Component(int x, int y, int width, int height);
	
	int x, y;
	int width, height;
	bool active;
};

#endif