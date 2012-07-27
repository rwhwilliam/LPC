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

#ifndef ASSIGNSTATE_H
#define ASSIGNSTATE_H

#include "Engine/State/State.h"

class SimState;
class UI;

class AssignState : public State
{
public:
	AssignState(StateManager* manager, SimState* simstate, int pop, int width, int height, int xloc, int yloc);
	~AssignState();

	AssignState(const AssignState& data);
	AssignState* operator=(const AssignState* rhs);

	void update(float time, Uint8* keystrokes);
	void raiseEvent(SDL_Event* event);
	void draw();

private:
	void assign();
	int getTotal();

	int pop;
	UI* ui;
	

	SimState* simstate;
};

#endif