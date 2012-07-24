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

#ifndef ROAD_H
#define ROAD_H

#include "Engine/Graphics/Image.h"

class SimState;

class Road
{
public:
	Road(SimState* state, int x, int y);
	~Road();

	Road(const Road& data);
	Road& operator=(const Road* rhs);

	int getX() { return xloc; }
	int getY() { return yloc; }
	int getMapX();
	int getMapY();
	int getWidth() { return img->getWidth(); }
	int getHeight() { return img->getHeight(); }
	Road* getRoad(int x, int y);

	void calculateMode();
	bool collides(int x, int y, int width, int height);

	void resize();

	void draw(int xoffset, int yoffset, SDL_Surface* screen);

private:
	int xloc, yloc;
	Image* img;
	SimState* state;
};

#endif