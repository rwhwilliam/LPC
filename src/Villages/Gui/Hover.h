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

#ifndef HOVER_H
#define HOVER_H

#include <string>
#include <list>

#include "SDL.h"

using namespace std;

class Timer;
class SimState;
class Font;

class Hover
{
public:
	Hover(SimState* state, int x, int y, int width, int height, int targetX, int targetY, int targetWidth, int targetHeight);
	~Hover();

	Hover(const Hover& data);
	Hover& operator=(const Hover* rhs);

	void addLine(string line);
	void setScrolling(bool scrolling) { Hover::scrolling = scrolling; }

	void raiseEvent(SDL_Event* event);
	void draw(int xoffset, int yoffset, SDL_Surface* screen);

private:
	list<SDL_Surface*> lines;
	int xloc, yloc;
	int width, height;
	SDL_Rect target;
	Timer* timer;
	bool active;
	bool scrolling;
	SimState* state;
	Font* font;
};

#endif