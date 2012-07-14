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

#ifndef ACTIONBAR_H
#define ACTIONBAR_H

#include <string>

#include "SDL.h"

#include "Engine/Gui/ClickableButton.h"
#include "Engine/Gui/UI.h"

using namespace std;

class SimState;

class ActionBar : public UI
{
public:
	ActionBar(SimState* state, int x, int y, int width, int height, string backgroundSrc);
	~ActionBar();

	ActionBar(const ActionBar& data);
	ActionBar& operator=(const ActionBar* rhs);

private:
	void placeHouse();
	void placeFarm();
	void placeMiningCamp();

	SimState* state;
	ClickableButton<ActionBar>* buildHouse;
	ClickableButton<ActionBar>* buildFarm;
	ClickableButton<ActionBar>* buildMiningCamp;
};

#endif