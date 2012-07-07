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

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>

#include "SDL.h"
#include "SDL_image.h"

using namespace std;

class State;

class StateManager
{
public:
	StateManager();
	~StateManager();

	StateManager(const StateManager& data);
	StateManager& operator=(const StateManager* rhs);

	void push(State* state);
	State* pop();

	void draw(SDL_Surface* screen);
	void raiseEvent(SDL_Event* event);
	void update(float time, Uint8* keystates);

private:
	//tho I'm going for the ability of a stack..use a vector for I can 'look' behind
	vector<State*> states;
};

#endif