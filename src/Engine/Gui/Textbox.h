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

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>

#include "SDL.h"

#include "Component.h"

using namespace std;

class Image;
class Font;

class Textbox : Component
{
public:
	Textbox(int x, int y, int width, int height, string backgroundSrc, string fontSrc, int fontSize);
	~Textbox();

	Textbox(const Textbox& data);
	Textbox& operator=(const Textbox* rhs);

	void draw(SDL_Surface* screen);
	virtual void raiseEvent(SDL_Event* event);

	void setContent(string c);
	
protected:
	Image* background;
	Font* font;
	Uint8* prevState;
	string contents;
	SDL_Surface* textImage;
};

#endif