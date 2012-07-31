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
#include "Engine/State/State.h"
#include "Engine/State/StateManager.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

using namespace std;

CreditState::CreditState(StateManager* manager, int width, int height, int xloc, int yloc) : State(manager, width, height, xloc, yloc)
{
	Logger::debug("CreditState Constructor");

	bg = new Image("assign-ui.png");

	end = new ClickableButton<CreditState>(400, 400, 64, 32, "end-button-normal.png", "end-button-hover.png", "end-button-pressed.png", this, &CreditState::goToTitle);	
}

CreditState::~CreditState()
{
	delete bg;

	delete end;

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
	end->raiseEvent(event);
}

void CreditState::draw()
{
	bg->draw(0, 0, frame);

	end->draw(frame);
}