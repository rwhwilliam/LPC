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

#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "SDL.h"

using namespace std;

class Image
{
public:
	Image(string src);
	Image(string src, int alpha);
	Image(string src, Uint8 r, Uint8 g, Uint8 b);
	Image(string src, int alpha, Uint8 r, Uint8 g, Uint8 b);
	Image(string src, float scale);
	Image(string src, int alpha, float scale);
	Image(string src, Uint8 r, Uint8 g, Uint8 b, float scale);
	Image(string src, int alpha, Uint8 r, Uint8 g, Uint8 b, float scale);

	Image(const Image& img);
	Image& operator=(const Image* rhs);
	~Image();

	void draw(int x, int y, SDL_Surface* screen);

	int getWidth();
	int getHeight();

	void setAlpha(int alpha);
	void setScale(float scale);
private:
	string src;
	int apha;
};

#endif