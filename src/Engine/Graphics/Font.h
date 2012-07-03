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

#ifndef FONT_H
#define FONT_H

#include <string>

#include "SDL.h"
#include "SDL_image.h"

using namespace std;

class Font
{
public:
	Font(string path, int size, Uint8 r, Uint8 g, Uint8 b);
	void draw(int x, int y, SDL_Surface* source, SDL_Surface* screen);
	SDL_Surface* getSurface(string msg);

private:
	string src;
	SDL_Color color;
};

#endif