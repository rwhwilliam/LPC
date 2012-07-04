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

#include "StateManager.h"

#include <vector>

#include "State.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

StateManager::StateManager()
{
	states.clear();
}

StateManager::~StateManager()
{

}

StateManager::StateManager(const StateManager& data)
{
	throw VillageException("Time Copy Constructor");
}

StateManager& StateManager::operator=(const StateManager* rhs)
{
	throw VillageException("Time Assignment Operator");
}

void StateManager::push(State* state)
{
	states.push_back(state);
}

State* StateManager::pop()
{
	State* popped = states.back();

	states.pop_back();

	return popped;
}

void StateManager::update(float time, Uint8* keystates)
{
	vector<State*>::const_iterator ittr = states.end();

	do
	{
		--ittr;

		(*ittr)->update(time, keystates);

		if(!(*ittr)->getExecuteBehind())
			break;
	}
	while(ittr != states.begin());
}

void StateManager::raiseEvent(SDL_Event event)
{
	vector<State*>::const_iterator ittr = states.end();

	do
	{
		--ittr;

		(*ittr)->raiseEvent(event);

		if(!(*ittr)->getRaiseBehind())
			break;
	}
	while(ittr != states.begin());
}

void StateManager::draw(SDL_Surface* screen)
{
	vector<State*>::const_iterator ittr = states.end();

	do
	{
		--ittr;

		(*ittr)->draw();
		(*ittr)->flip(screen);

		if(!(*ittr)->getShowBehind())
			break;
	}
	while(ittr != states.begin());
	
}