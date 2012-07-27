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

#include "AssignState.h"

#include "Engine/Gui/Label.h"
#include "Engine/Gui/UI.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Gui/IncrementBox.h"
#include "Villages/States/SimState.h"

AssignState::AssignState(StateManager* manager, SimState* simstate, int pop, int width, int height, int xloc, int yloc) : State(manager, width, height, xloc, yloc), pop(pop)
{
	Logger::debug("AssignState Constructor");

	AssignState::simstate = simstate;

	ui = new UI(xloc, yloc, width, height, "assign-ui.png", 255, 0, 255);

	ui->addComponent("headerLbl", new Label(240, 220, "lazy.ttf", "People Moved to your Village!", 32, 0, 0, 0));

	ui->addComponent("lblFarm", new Label(50, 285, "lazy.ttf", "Farmers (" + toString(simstate->getFoodRoom()) + ")", 22, 0, 0, 0));
	ui->addComponent("txtFarm", new IncrementBox(180, 280, 128, 32, 0, 5));

	AssignState::showBehind = true;
}

AssignState::~AssignState()
{
	delete ui;

	Logger::debug("AssignState Destructor");
}

AssignState::AssignState(const AssignState& data) : State(NULL, 0, 0, 0, 0)
{
	throw VillageException("AssignState Copy Constructor");
}

AssignState* AssignState::operator=(const AssignState* rhs)
{
	throw VillageException("AssignState Assignment Operator");
}

void AssignState::update(float time, Uint8* keystrokes)
{
	
}

void AssignState::raiseEvent(SDL_Event* event)
{
	ui->raiseEvent(event);
}

void AssignState::draw()
{
	ui->draw(frame);
}