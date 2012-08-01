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

#ifndef CREDITSTATE_H
#define CREDITSTATE_H

#include <string>

#include "Engine/State/State.h"

using namespace std;

class Image;
class StateManager;
class UI;

class CreditState : public State
{
public:
	CreditState(StateManager* manager, int width, int height, int xloc, int yloc);
	~CreditState();

	CreditState(const CreditState& data);
	CreditState* operator=(const CreditState* rhs);

	void update(float time, Uint8* keystrokes);
	void raiseEvent(SDL_Event* event);
	void draw();

private:
	void goToTitle();

	UI* ui;
	//ClickableButton<CreditState>* end;
};

#endif CREDITSTATE_H