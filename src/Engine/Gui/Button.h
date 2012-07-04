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

#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "SDL.h"

#include "Component.h"

using namespace std;

enum ButtonState {NORMAL, HOVER, DOWN};

class UIEvent;
class Image;

class Button : public Component
{
public:
	Button(int x, int y, int width, int height, string normalSrc, string hoverSrc, string downSrc);
	Button(int x, int y, int width, int height, string normalSrc, string hoverSrc, string downSrc, Uint8 r, Uint8 g, Uint8 b);
	~Button();

	Button(const Button& data);
	Button& operator=(const Button* rhs);

	virtual void click() = 0;
	void raiseEvent(SDL_Event* event);
	void draw(SDL_Surface* screen);
private:
	Image* normalBackground;
	Image* hoverBackground;
	Image* downBackground;
	ButtonState state;
};

#endif