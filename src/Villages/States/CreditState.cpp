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

#include "CreditState.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Gui/ClickableButton.h"
#include "Engine/Gui/Label.h"
#include "Engine/Gui/UI.h"
#include "Engine/State/State.h"
#include "Engine/State/StateManager.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

using namespace std;

CreditState::CreditState(StateManager* manager, int width, int height, int xloc, int yloc) : State(manager, width, height, xloc, yloc)
{
	Logger::debug("CreditState Constructor");

	ui = new UI(xloc, yloc, width, height, "assign-ui.png", 255, 0, 255);

	ui->addComponent("btn", new ClickableButton<CreditState>(450, 550, 64, 32, "end-button-normal.png", "end-button-hover.png", "end-button-pressed.png", this, &CreditState::goToTitle));
	ui->addComponent("header", new Label(350, 210, "lazy.ttf", "Special thanks to:", 42, 0, 0, 0));
	ui->addComponent("1", new Label(200, 270, "lazy.ttf", "- All Liberated Pixel Cup Artists who provided contest art", 22, 0, 0, 0));
	ui->addComponent("2", new Label(200, 300, "lazy.ttf", "- Other OpenGameArt.org Artists for additional images:", 22, 0, 0, 0));
	ui->addComponent("3", new Label(250, 330, "lazy.ttf", "- Rough Small Signs & Symbol Pack by qubodup", 22, 0, 0, 0));
	ui->addComponent("4", new Label(250, 360, "lazy.ttf", "- Beer Bottle (Open/Closed) by Q_x", 22, 0, 0, 0));
	ui->addComponent("5", new Label(250, 390, "lazy.ttf", "- Crystal Dungeon (Title Music) by FoxSynergy", 22, 0, 0, 0));
	ui->addComponent("6", new Label(250, 420, "lazy.ttf", "- RPG status icons by Buch", 22, 0, 0, 0));
	ui->addComponent("7", new Label(250, 450, "lazy.ttf", "- RPG Icons by DitzyDM", 22, 0, 0, 0));
	ui->addComponent("8", new Label(250, 480, "lazy.ttf", "- Tower v2 by yd", 22, 0, 0, 0));
	ui->addComponent("9", new Label(250, 510, "lazy.ttf", "- RPG Path Tile Template by gryc", 22, 0, 0, 0));
}

CreditState::~CreditState()
{
	delete ui;

	Logger::debug("CreditState Destructor");
}

CreditState::CreditState(const CreditState& data) : State(NULL, 0, 0, 0, 0)
{
	throw VillageException("CreditState Copy Constructor");
}

CreditState* CreditState::operator=(const CreditState* rhs)
{
	throw VillageException("CreditState Assignment Operator");
}

void CreditState::goToTitle()
{
	manager->pop();
}

void CreditState::update(float time, Uint8* keystrokes)
{

}

void CreditState::raiseEvent(SDL_Event* event)
{
	ui->raiseEvent(event);
}

void CreditState::draw()
{
	ui->draw(frame);
}