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

#ifndef SCROLLINGMAP_H
#define SCROLLINGMAP_H

#include "SDL.h"

#include "Engine/Graphics/TileMap.h"

class SimState;

class ScrollingMap : public TileMap
{
public:
	ScrollingMap(SimState* state, int width, int height, int tileWidth, int tileHeight, int layerCount);

	ScrollingMap(const ScrollingMap& img);
	ScrollingMap& operator=(const ScrollingMap* rhs);
	~ScrollingMap();

	void raiseEvent(SDL_Event* event);
	void draw(SDL_Surface* screen);
	void update(float time, Uint8* keystates);

	int getXOffset() { return xoffset; }
	int getYOffset() { return yoffset; }

	void resize();

protected:
	int xoffset, yoffset;
	SimState* state;
};

#endif