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

#include "RoadFactory.h"

#include <vector>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Objects/Road.h"
#include "Villages/States/SimState.h"

using namespace std;

RoadFactory::RoadFactory(SimState* state, int startX, int startY) : startX(startX), startY(startY)
{
	Logger::debug("RoadFactory Constructor");

	RoadFactory::state = state;

	preImg = new Image("road-horizontal.png", 128, (Uint8)255, 0, 255, state->getZoomLevel());

	coords.clear();
	coords.push_back(pair<int, int>(startX, startY));
}

RoadFactory::~RoadFactory()
{
	Logger::debug("RoadFactory Destructor");
}

RoadFactory::RoadFactory(const RoadFactory& data)
{
	throw VillageException("RoadFactory Copy Constructor");
}

RoadFactory& RoadFactory::operator=(const RoadFactory* rhs)
{
	throw VillageException("RoadFactory Assignment Operator");
}

void RoadFactory::resize()
{
	preImg->setScale(state->getZoomLevel());
}

void RoadFactory::raiseEvent(SDL_Event* event)
{
	if(event->type == SDL_MOUSEMOTION)
	{
		int tempX = finishX;
		int tempY = finishY;

		finishX = (event->motion.x /*- preImg->getWidth() / 2*/) + state->getXOffset();
		finishY = (event->motion.y /*- preImg->getHeight() / 2*/) + state -> getYOffset();

		finishX -= finishX % state->getTileWidth();
		finishY -= finishY % state->getTileHeight();

		finishX /= state->getTileWidth();
		finishY /= state->getTileHeight();

		if(finishX != tempX || finishY != tempY)
		{
			coords.clear();

			if(abs(finishX - startX) >= abs(finishY - startY))
			{
				finishY = startY;
			}
			else
			{
				finishX = startX;
			}

			if(startX == finishX)
			{
				preImg = new Image("road-vertical.png", 128, 255, 0, 255);

				if(startY < finishY)
					for(int i = startY; i <= finishY; i++)
						coords.push_back(pair<int, int>(startX, i));
				else
					for(int i = startY; i >= finishY; i--)
						coords.push_back(pair<int, int>(startX, i));
			}
			else
			{
				preImg = new Image("road-horizontal.png", 128, 255, 0, 255);

				if(startX < finishX)
					for(int i = startX; i <= finishX; i++)
						coords.push_back(pair<int, int>(i, startY));
				else
					for(int i = startX; i >= finishX; i--)
						coords.push_back(pair<int, int>(i, startY));
			}
		}
	}
}

void RoadFactory::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	vector<pair<int, int> >::iterator it;
	for(it = coords.begin(); it < coords.end(); ++it)
	{
		preImg->draw(it->first * state->getTileWidth() - xoffset, it->second * state->getTileHeight() - yoffset, screen);
	}
}

bool RoadFactory::canBuild()
{
	vector<pair<int, int> >::iterator it;
	for(it = coords.begin(); it < coords.end(); ++it)
		if(state->canBuild(it->first * state->getTileWidth() - state->getXOffset(), it->second * state->getTileHeight() - state->getYOffset(), preImg->getWidth(), preImg->getHeight()) == E_BAD)
			return false;

	return true;
}

void RoadFactory::createRoad()
{
	vector<pair<int, int> >::iterator it;
	for(it = coords.begin(); it < coords.end(); ++it)
	{
		map<string, Road*>::iterator rit = state->getRoads()->find(toString(it->first) + "_" + toString(it->second));

		if(rit == state->getRoads()->end())
		{
			//add a road segment here
			(*state->getRoads())[toString(it->first) + "_" + toString(it->second)] = new Road(state, it->first, it->second);
		}
	}

	for(it = coords.begin(); it < coords.end(); ++it)
	{
		(*state->getRoads())[toString(it->first) + "_" + toString(it->second)]->calculateMode();
	}
}