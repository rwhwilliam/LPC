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

#ifndef RESOURCEBAR_H
#define RESOURCEBAR_H

#include "SDL.h"

class SimState;
class Label;
class Image;
class IncrementBox;

class ResourceBar
{
public:
	ResourceBar(SimState* state);
	~ResourceBar();

	ResourceBar(const ResourceBar& data);
	ResourceBar& operator=(const ResourceBar* rhs);


	void update(float time, Uint8* keystrokes);
	void raiseEvent(SDL_Event* event);
	void draw(SDL_Surface* screen);

private:
	SimState* state;

	Image* bg;
	Image* bg2;

	Image* goldImg;
	Image* foodImg;
	Image* woodImg; 
	Image* oreImg;
	Image* weaponsImg;
	Image* wellImg;

	Label* goldLbl;
	Label* foodLbl;
	Label* woodLbl;
	Label* oreLbl;
	Label* weaponsLbl;
	Label* wellLbl;
	
	Label* taxLbl;
	IncrementBox* box;

	Label* popLbl;
	Label* pop;

	Label* newPop;

	Label* turnLbl;
	Label* turn;
};

#endif