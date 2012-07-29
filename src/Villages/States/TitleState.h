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

#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "Engine/State/State.h"

class Image;

class TitleState : public State
{
public:
	TitleState(StateManager* manager, int width, int height, int xloc, int yloc);
	~TitleState();

	TitleState(const TitleState& data);
	TitleState* operator=(const TitleState* rhs);

	void update(float time, Uint8* keystrokes);
	void raiseEvent(SDL_Event* event);
	void draw();

private:
	Image* bg;
};

#endif