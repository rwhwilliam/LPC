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

#ifndef LABEL_H
#define LABEL_H

#include <string>

#include "SDL.h"

#include "Component.h"

using namespace std;

class Font;

class Label : public Component
{
public:
	Label(int x, int y, string fontSrc, string content, int size);
	Label(int x, int y, string fontSrc, string content, int size, Uint8 r, Uint8 g, Uint8 b);
	~Label();

	Label(const Label& data);
	Label& operator=(const Label* rhs);

	void setText(string txt);

	void raiseEvent(SDL_Event* event);
	void draw(SDL_Surface* screen);

private:
	Font* font;
	SDL_Surface* textImage;
	string content;
};

#endif