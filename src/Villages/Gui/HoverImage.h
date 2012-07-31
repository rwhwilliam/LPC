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

#ifndef HOVERIMAGE_H
#define HOVERIMAGE_H

#include "Hover.h"

#include "SDL.h"

class Image;

class HoverImage : public Hover
{
public:
	HoverImage(SimState* state, int x, int y, int width, int height, int targetX, int targetY, int targetWidth, int targetHeight);
	~HoverImage();

	HoverImage(const HoverImage& data);
	HoverImage& operator=(const HoverImage* rhs);

	void addLine(string line);
	void addLine(string key, string line);
	void editLine(string key, string line);
	void addImage(int x, int y, Image* img);

	void draw(int xoffset, int yoffset, SDL_Surface* screen);

private:
	map<pair<int, int>, Image*> images;
};

#endif