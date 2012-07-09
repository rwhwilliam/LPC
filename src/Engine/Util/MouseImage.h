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

#ifndef MOUSEIMAGE_H
#define MOUSEIMAGE_H

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"

using namespace std;

enum MouseImageMode { MI_NORMAL, MI_BAD };

class MouseImage
{
public:
	MouseImage(string goodSrc, string badSrc, int alpha);
	~MouseImage();

	MouseImage(const MouseImage& data);
	MouseImage& operator=(const MouseImage* rhs);

	void raiseEvent(SDL_Event* event);
	void draw(SDL_Surface* screen);

	void setMode(MouseImageMode mode) { MouseImage::mode = mode; }

	int getX() { return x; }
	int getY() { return y; }
	int getWidth() { return goodImg->getWidth(); }
	int getHeight() { return goodImg->getHeight(); }

private:
	int x, y;
	Image* goodImg;
	Image* badImg;
	MouseImageMode mode;
};

#endif