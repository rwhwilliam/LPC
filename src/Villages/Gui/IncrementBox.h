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

#ifndef INCREMENTBOX_H
#define INCREMENTBOX_H

#include "Engine/Gui/ClickableButton.h"

#include "SDL.h"

#include "Engine/Gui/Component.h"

class Textbox;

class IncrementBox : public Component
{
public:
	IncrementBox(int x, int y, int width, int height, int min, int max);
	IncrementBox(int x, int y, int width, int height, int min, int max, int val);
	~IncrementBox();

	IncrementBox(const IncrementBox& data);
	IncrementBox& operator=(const IncrementBox* rhs);

	void increment();
	void decrement();

	int getValue() { return current; }
	void setValue(int val) { current = val; }

	void raiseEvent(SDL_Event* event);
	void draw(SDL_Surface* screen);

private:
	ClickableButton<IncrementBox>* up;
	ClickableButton<IncrementBox>* down;
	Textbox* box;

	int min, max;
	int current;
};

#endif