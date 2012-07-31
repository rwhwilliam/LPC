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
	string s = "Please Assign all " + toString(pop) + " of them to a Job";
	ui->addComponent("infoLbl", new Label(280, 265, "lazy.ttf", s.c_str(), 22, 0, 0, 0));

	s = "Farm (" + toString(simstate->getFarmRoom()) + " Spots)";
	ui->addComponent("lblFarm", new Label(40, 315, "lazy.ttf", s.c_str(), 22, 0, 0, 0));
	ui->addComponent("txtFarm", new IncrementBox(280, 310, 128, 32, 0, min(pop, simstate->getFarmRoom())));

	s = "Mill (" + toString(simstate->getMillRoom()) + " Spots)";
	ui->addComponent("lblMill", new Label(40, 380, "lazy.ttf", s.c_str(), 22, 0, 0, 0));
	ui->addComponent("txtMill", new IncrementBox(280, 375, 128, 32, 0, min(pop, simstate->getMillRoom())));

	s = "Mine (" + toString(simstate->getMineRoom()) + " Spots)";
	ui->addComponent("lblMine", new Label(500, 315, "lazy.ttf", s.c_str(), 22, 0, 0, 0));
	ui->addComponent("txtMine", new IncrementBox(740, 310, 128, 32, 0, min(pop, simstate->getMineRoom())));

	s = "Blacksmith (" + toString(simstate->getBlacksmithRoom()) + " Spots)";
	ui->addComponent("lblBlacksmith", new Label(500, 380, "lazy.ttf", s.c_str(), 22, 0, 0, 0));
	ui->addComponent("txtBlacksmith", new IncrementBox(740, 375, 128, 32, 0, min(pop, simstate->getBlacksmithRoom())));
	
	if(simstate->getWonder() != NULL)
	{
		s = "Wonder (" + toString(simstate->getWonderRoom()) + " Spots)";
		ui->addComponent("lblWonder", new Label(400, 450, "lazy.ttf", s.c_str(), 22, 0, 0, 0));
		ui->addComponent("txtWonder", new IncrementBox(640, 445, 128, 32, 0, min(pop, simstate->getWonderRoom())));
	}

	ui->addComponent("btnAssign", new ClickableButton<AssignState>(450, 500, 64, 32, "assign-normal.png", "assign-hover.png", "assign-pressed.png", this, &AssignState::assign));
	
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
		dynamic_cast<IncrementBox*>(ui->getComponent("txtBlacksmith"))->getValue() + 
		(simstate->getWonder() != NULL) ? dynamic_cast<IncrementBox*>(ui->getComponent("txtWonder"))->getValue() : 0;
}

void AssignState::assign()
{
	if(getTotal() == pop)
	{
		simstate->assignEndTurn(pop, dynamic_cast<IncrementBox*>(ui->getComponent("txtFarm"))->getValue(), 
			dynamic_cast<IncrementBox*>(ui->getComponent("txtMill"))->getValue(), 
			dynamic_cast<IncrementBox*>(ui->getComponent("txtMine"))->getValue(), 
			dynamic_cast<IncrementBox*>(ui->getComponent("txtBlacksmith"))->getValue(),
			(simstate->getWonder() != NULL) ? dynamic_cast<IncrementBox*>(ui->getComponent("txtWonder"))->getValue() : 0);

		manager->pop();
	}
}