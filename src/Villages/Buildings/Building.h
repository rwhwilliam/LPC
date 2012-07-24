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

#ifndef BUILDING_H
#define BUILDING_H

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"

using namespace std;

class MouseImage;
class MapTile;
class SimState;

class Building
{
public:
	Building(SimState* state, string src, int xloc, int yloc);
	~Building();

	Building(const Building& data);
	Building& operator=(const Building* rhs);

	int getMapX();
	int getMapY();
	int getWidth() { return img->getWidth(); }
	int getHeight() { return img->getHeight(); }

	bool collides(int x, int y, int width, int height);

	void resize();

	virtual void update(float time, Uint8* keystrokes);
	virtual void raiseEvent(SDL_Event* event);
	virtual void draw(int xoffset, int yoffset, SDL_Surface* screen);

private:
	Image* img;
	int xloc, yloc;
	SimState* state;
};

#endif