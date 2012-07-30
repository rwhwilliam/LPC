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

#include "ResourceBar.h"

#include <vector>

#include "SDL.h"

#include "Engine/Gui/Label.h"
#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Gui/IncrementBox.h"
#include "Villages/States/SimState.h"
#include "Villages/Buildings/Castle.h"

using namespace std;

ResourceBar::ResourceBar(SimState* state)
{
	Logger::debug("ResourceBar Constructor");

	ResourceBar::state = state;

	bg = new Image("resource-ui.png", 255, 0, 255);
	bg2 = new Image("resource-ui2.png", 255, 0, 255);

	goldImg = new Image("icon-gold.png", 255, 0, 255);
	foodImg = new Image("icon-food.png", 255, 0, 255);
	woodImg = new Image("icon-wood.png", 255, 0, 255);
	oreImg = new Image("icon-ore.png", 255, 0, 255);
	weaponsImg = new Image("icon-weapons.png", 255, 0, 255);
	wellImg = new Image("icon-water.png", 255, 0, 255);

	goldLbl = new Label(130, 12, "lazy.ttf", "", 30);
	foodLbl = new Label(280, 12, "lazy.ttf", "", 30);
	woodLbl = new Label(430, 12, "lazy.ttf", "", 30);
	oreLbl = new Label(580, 12, "lazy.ttf", "", 30);
	weaponsLbl = new Label(730, 12, "lazy.ttf", "", 30);
	wellLbl = new Label(880, 12, "lazy.ttf", "", 30);

	taxLbl = new Label(600, 62, "lazy.ttf", "Tax %:", 22);

	box = new IncrementBox(670, 55, 120, 32, 0, 100, 5);

	popLbl = new Label(230, 62, "lazy.ttf", "Pop:", 22);
	pop = new Label(280, 62, "lazy.ttf", "", 22);

	newPop = new Label(380, 62, "lazy.ttf", "+0", 22);

	turnLbl = new Label(500, 62, "lazy.ttf", "Turn:", 22);
	turn = new Label(550, 62, "lazy.ttf", "", 22);
}

ResourceBar::~ResourceBar()
{
	delete bg;
	delete bg2;

	delete goldImg;
	delete foodImg;
	delete woodImg;
	delete oreImg;
	delete weaponsImg;
	delete wellImg;

	delete goldLbl;
	delete foodLbl;
	delete woodLbl;
	delete oreLbl;
	delete weaponsLbl;
	delete wellLbl;

	delete taxLbl;

	delete box;

	delete popLbl;
	delete pop;

	delete newPop;
	
	delete turn;

	Logger::debug("ResourceBar Destructor");
}

ResourceBar::ResourceBar(const ResourceBar& data)
{
	throw VillageException("ResourceBar Copy Constructor");
}

ResourceBar& ResourceBar::operator=(const ResourceBar* rhs)
{
	throw VillageException("ResourceBar Assignment Operator");
}

void ResourceBar::update(float time, Uint8* keystrokes)
{
	if(state->getCastle() != NULL)
	{
		goldLbl->setText(toString(state->getCastle()->getGold()));
		foodLbl->setText(toString(state->getCastle()->getFood()));
		woodLbl->setText(toString(state->getCastle()->getWood()));
		oreLbl->setText(toString(state->getCastle()->getOre()));
		weaponsLbl->setText(toString(state->getCastle()->getWeapons()));
		wellLbl->setText(toString(state->getSpareWater()));

		state->getCastle()->setTaxRate(box->getValue());
		pop->setText(toString(state->getPop()));

		if(state->getNewPop() >= 0)
			newPop->setText("+" + toString(state->getNewPop()));
		else
			newPop->setText("-" + toString(abs(state->getNewPop())));

		turn->setText(toString(state->getTurn()));
	}
}

void ResourceBar::raiseEvent(SDL_Event* event)
{
	box->raiseEvent(event);
}

void ResourceBar::draw(SDL_Surface* screen)
{
	bg->draw(62, 0, screen);
	bg2->draw(212, 50, screen);

	goldImg->draw(80, 5, screen);
	goldLbl->draw(screen);

	foodImg->draw(230, 5, screen);
	foodLbl->draw(screen);

	woodImg->draw(380, 5, screen);
	woodLbl->draw(screen);

	oreImg->draw(530, 5, screen);
	oreLbl->draw(screen);

	weaponsImg->draw(680, 5, screen);
	weaponsLbl->draw(screen);

	wellImg->draw(830, 5, screen);
	wellLbl->draw(screen);

	taxLbl->draw(screen);
	box->draw(screen);

	popLbl->draw(screen);
	pop->draw(screen);

	newPop->draw(screen);

	turnLbl->draw(screen);
	turn->draw(screen);
}