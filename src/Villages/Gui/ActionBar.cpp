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
#include "Villages/States/SimState.h"

using namespace std;

ActionBar::ActionBar(SimState* state, int x, int y, int width, int height, string backgroundSrc) : UI(x, y, width, height, backgroundSrc)
{
	Logger::debug("ActionBar Constructor");

	ActionBar::state = state;

	buildHouse = new ClickableButton<ActionBar>(x + 10, y + 10, 50, 50, "house-button-normal.png", "house-button-hover.png", "house-button-pressed.png", this, &ActionBar::placeHouse); 

	addComponent("buildHouse", buildHouse);

	buildFarm = new ClickableButton<ActionBar>(x + 74, y + 10, 64, 64, "farm-button-normal.png", "farm-button-hover.png", "farm-button-pressed.png", this, &ActionBar::placeFarm);

	addComponent("buildFarm", buildFarm);

	buildMiningCamp = new ClickableButton<ActionBar>(x + 138, y + 10, 64, 64, "miningcamp-button-normal.png", "miningcamp-button-hover.png", "miningcamp-button-pressed.png", this, &ActionBar::placeMiningCamp);

	addComponent("buildMiningCamp", buildMiningCamp);

	buildMill = new ClickableButton<ActionBar>(x + 202, y + 10, 64, 64, "mill-button-normal.png", "mill-button-hover.png", "mill-button-pressed.png", this, &ActionBar::placeMill);

	addComponent("buildMill", buildMill);

	buildWell = new ClickableButton<ActionBar>(x + 266, y + 10, 64, 64, "well-button-normal.png", "well-button-hover.png", "well-button-pressed.png", this, &ActionBar::placeWell);

	addComponent("buildWell", buildWell);

	buildTavern = new ClickableButton<ActionBar>(x + 330, y + 10, 64, 64, "tavern-button-normal.png", "tavern-button-hover.png", "tavern-button-pressed.png", this, &ActionBar::placeTavern);

	addComponent("buildTavern", buildTavern);

	buildTheatre = new ClickableButton<ActionBar>(x + 394, y + 10, 64, 64, "theatre-button-normal.png", "theatre-button-hover.png", "theatre-button-pressed.png", this, &ActionBar::placeTheatre);

	addComponent("buildTheatre", buildTheatre);

	buildWeaver = new ClickableButton<ActionBar>(x + 458, y + 10, 64, 64, "weaver-button-normal.png", "weaver-button-hover.png", "weaver-button-pressed.png", this, &ActionBar::placeWeaver);

	addComponent("buildWeaver", buildWeaver);

	buildJeweler = new ClickableButton<ActionBar>(x + 522, y + 10, 64, 64, "jeweler-button-normal.png", "jeweler-button-hover.png", "jeweler-button-pressed.png", this, &ActionBar::placeJeweler);

	addComponent("buildJeweler", buildJeweler);

	buildBlacksmith = new ClickableButton<ActionBar>(x + 586, y + 10, 64, 64, "blacksmith-button-normal.png", "blacksmith-button-hover.png", "blacksmith-button-pressed.png", this, &ActionBar::placeBlacksmith);

	addComponent("buildBlacksmith", buildBlacksmith);

	buildBakery = new ClickableButton<ActionBar>(x + 650, y + 10, 64, 64, "bakery-button-normal.png", "bakery-button-hover.png", "bakery-button-pressed.png", this, &ActionBar::placeBakery);

	addComponent("buildBakery", buildBakery);

	buildGuardStation = new ClickableButton<ActionBar>(x + 714, y + 10, 64, 64, "guardstation-button-normal.png", "guardstation-button-hover.png", "guardstation-button-pressed.png", this, &ActionBar::placeGuardStation);

	addComponent("buildGuardStation", buildGuardStation);

	buildMarket = new ClickableButton<ActionBar>(x + 778, y + 10, 64, 64, "market-button-normal.png", "market-button-hover.png", "market-button-pressed.png", this, &ActionBar::placeMarket);

	addComponent("buildMarket", buildMarket);

	buildRoad = new ClickableButton<ActionBar>(x + 842, y + 10, 64, 64, "road-button-normal.png", "road-button-hover.png", "road-button-pressed.png", this, &ActionBar::placeRoad);

	addComponent("buildRoad", buildRoad);

	in = new ClickableButton<ActionBar>(x + 906, y + 10, 32, 32, "zoomin-button-normal.png", "zoomin-button-hover.png", "zoomin-button-pressed.png", this, &ActionBar::zoomIn);

	addComponent("zoomIn", in);

	out = new ClickableButton<ActionBar>(x + 906, y + 42, 32, 32, "zoomout-button-normal.png", "zoomout-button-hover.png", "zoomout-button-pressed.png", this, &ActionBar::zoomOut);

	addComponent("zoomOut", out);
}

ActionBar::~ActionBar()
{
	Logger::debug("ActionBar Destructor");

	delete buildHouse;
	delete buildFarm;
	delete buildMiningCamp;
	delete buildMill;
	delete buildWell;
	delete buildTavern;
	delete buildTheatre;
	delete buildWeaver;
	delete buildJeweler;
	delete buildBlacksmith;
	delete buildBakery;
	delete buildGuardStation;
	delete buildMarket;
	delete buildRoad;
	delete in;
	delete out;
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
	state->placeHouse();
}

void ActionBar::placeFarm()
{
	state->placeFarm();
}

void ActionBar::placeMiningCamp()
{
	state->placeMiningCamp();
}

void ActionBar::placeMill()
{
	state->placeMill();
}

void ActionBar::placeWell()
{
	state->placeWell();
}

void ActionBar::placeTavern()
{
	state->placeTavern();
}

void ActionBar::placeTheatre()
{
	state->placeTheatre();
}

void ActionBar::placeWeaver()
{
	state->placeWeaver();
}

void ActionBar::placeJeweler()
{
	state->placeJeweler();
}

void ActionBar::placeBlacksmith()
{
	state->placeBlacksmith();
}

void ActionBar::placeBakery()
{
	state->placeBakery();
}

void ActionBar::placeGuardStation()
{
	state->placeGuardStation();
}

void ActionBar::placeMarket()
{
	state->placeMarket();
}

void ActionBar::placeRoad()
{
	state->placeRoad();
}

void ActionBar::zoomIn()
{
	state->zoomIn();
}

void ActionBar::zoomOut()
{
	state->zoomOut();
}