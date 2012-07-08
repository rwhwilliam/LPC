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

#include "ActionBar.h"

#include <string>

#include "SDL.h"

#include "Engine/Gui/ClickableButton.h"
#include "Engine/Gui/UI.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

using namespace std;

ActionBar::ActionBar(SimState* state, int x, int y, int width, int height, string backgroundSrc) : UI(x, y, width, height, backgroundSrc)
{
	Logger::debug("ActionBar Constructor");

	ActionBar::state = state;

	buildHouse = new ClickableButton<ActionBar>(x + 10, y + 10, 50, 50, "buildhouse-normal.png", "buildhouse-hover.png", "buildhouse-down.png", this, &ActionBar::placeHouse); 

	addComponent("buildHouse", buildHouse);
}

ActionBar::~ActionBar()
{
	Logger::debug("ActionBar Destructor");
}

ActionBar::ActionBar(const ActionBar& data) : UI(0, 0, 0, 0, "")
{
	throw VillageException("ActionBar Copy Constructor");
}

ActionBar& ActionBar::operator=(const ActionBar* rhs)
{
	throw VillageException("ActionBar Assignment Operator");
}

void ActionBar::placeHouse()
{

}