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

#include <math.h>

#include "Engine/Gui/Label.h"
#include "Engine/Gui/UI.h"
#include "Engine/State/StateManager.h"
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
	ui->addComponent("infoLbl", new Label(280, 265, "lazy.ttf", "Please Assign all " + toString(pop) + " of them to a Job", 22, 0, 0, 0));

	ui->addComponent("lblFarm", new Label(40, 315, "lazy.ttf", "Farm (" + toString(simstate->getFarmRoom()) + " Spots)", 22, 0, 0, 0));
	ui->addComponent("txtFarm", new IncrementBox(280, 310, 128, 32, 0, min(pop, simstate->getFarmRoom())));

	ui->addComponent("lblMill", new Label(40, 380, "lazy.ttf", "Mill (" + toString(simstate->getMillRoom()) + " Spots)", 22, 0, 0, 0));
	ui->addComponent("txtMill", new IncrementBox(280, 375, 128, 32, 0, min(pop, simstate->getMillRoom())));

	ui->addComponent("lblMine", new Label(500, 315, "lazy.ttf", "Mine (" + toString(simstate->getMineRoom()) + " Spots)", 22, 0, 0, 0));
	ui->addComponent("txtMine", new IncrementBox(740, 310, 128, 32, 0, min(pop, simstate->getMineRoom())));

	ui->addComponent("lblBlacksmith", new Label(500, 380, "lazy.ttf", "Blacksmith (" + toString(simstate->getBlacksmithRoom()) + " Spots)", 22, 0, 0, 0));
	ui->addComponent("txtBlacksmith", new IncrementBox(740, 375, 128, 32, 0, min(pop, simstate->getBlacksmithRoom())));
	
	ui->addComponent("btnAssign", new ClickableButton<AssignState>(400, 450, 64, 32, "assign-normal.png", "assign-hover.png", "assign-pressed.png", this, &AssignState::assign));
	
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

int AssignState::getTotal()
{
	return dynamic_cast<IncrementBox*>(ui->getComponent("txtFarm"))->getValue() + 
		dynamic_cast<IncrementBox*>(ui->getComponent("txtMill"))->getValue() +
		dynamic_cast<IncrementBox*>(ui->getComponent("txtMine"))->getValue() +
		dynamic_cast<IncrementBox*>(ui->getComponent("txtBlacksmith"))->getValue();
}

void AssignState::assign()
{
	if(getTotal() == pop)
	{
		simstate->assignEndTurn(pop, dynamic_cast<IncrementBox*>(ui->getComponent("txtFarm"))->getValue(), 
			dynamic_cast<IncrementBox*>(ui->getComponent("txtMill"))->getValue(), 
			dynamic_cast<IncrementBox*>(ui->getComponent("txtMine"))->getValue(), 
			dynamic_cast<IncrementBox*>(ui->getComponent("txtBlacksmith"))->getValue());

		manager->pop();
	}
}