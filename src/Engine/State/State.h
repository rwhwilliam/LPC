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

#ifndef STATE_H
#define STATE_H

#include "SDL.h"
#include "SDL_image.h"

class StateManager;

class State
{
public:
	virtual void update(float time, Uint8* keystates) = 0;
	virtual void raiseEvent(SDL_Event* event) = 0;
	virtual void draw() = 0;
	
	//unconventional...but this is what draws "frame" onto the screen
	void flip(SDL_Surface* screen);

	bool getShowBehind() { return showBehind; }
	bool getRaiseBehind() { return raiseBehind; }
	bool getExecuteBehind() { return executeBehind; }

protected:
	State(StateManager* manager, int width, int height, int xloc, int yloc);
	~State();

	State(const State& data);
	State& operator=(const State* rhs);

	//if true, drawing will go another layer down
	//using this you could draw a menu only half the screen over a game and still see it
	bool showBehind;

	//if true, updates, but -not- usercontrol/input will go another layer down
	//using this you can draw a menu and the game can still be running/animating beneath it
	bool executeBehind;

	//if true, raises SDL events 'below' this state to others
	bool raiseBehind;

	//the position and width/height of this 'frame'
	int width, height;
	int xloc, yloc;

	//actual surface that is drawn
	SDL_Surface* frame;

	//state manager
	StateManager* manager;
};

#endif