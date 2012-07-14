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

#ifndef MAPTILE_H
#define MAPTILE_H

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Villages/States/SimState.h"

using namespace std;

class MouseImage;

class MapTile
{
public:
	MapTile(SimState* state, string src, int xloc, int yloc);
	~MapTile();

	MapTile(const MapTile& data);
	MapTile& operator=(const MapTile* rhs);

	bool collides(int x, int y, int width, int height);

	int getX() { return xloc; }
	int getY() { return yloc; }
	int getWidth() { return img->getWidth(); }
	int getHeight() { return img->getHeight(); }
	int getMapX();
	int getMapY();

	virtual void draw(int xoffset, int yoffset, SDL_Surface* screen);

private:
	SimState* state;
	Image* img;
	int xloc, yloc;
};

#endif