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

#ifndef SIMSTATE_H
#define SIMSTATE_H

#include <string>

#include "SDL.h"

#include "Engine/State/State.h"
#include "Engine/Util/MouseImage.h"
#include "Villages/Objects/Castle.h"
#include "Villages/Util/ScrollingMap.h"

using namespace std;

enum SimMode { S_NORMAL, S_PLACECASTLE };

class SimState : public State
{
public:
	SimState(string path, int width, int height, int xloc, int yloc);
	~SimState();

	SimState(const SimState& data);
	SimState& operator=(const SimState* rhs);

	void update(float time, Uint8* keystrokes);
	void raiseEvent(SDL_Event* event);
	void draw();

	TileMap* getMap();

private:
	SimMode mode;
	ScrollingMap* map;
	MouseImage* imageHover;
	Castle* castle;
};

#endif