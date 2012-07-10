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

#include "SimState.h"

#include <string>
#include <vector>

#include "tinyxml2.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/MouseImage.h"
#include "Engine/Util/Tokenizer.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Gui/ActionBar.h"
#include "Villages/Objects/Object.h"
#include "Villages/Objects/Castle.h"
#include "Villages/Objects/House.h"
#include "Villages/Objects/Farm.h"
#include "Villages/Util/ScrollingMap.h"

using namespace std;
using namespace tinyxml2;

SimState::SimState(string path, int width, int height, int xloc, int yloc) : State(width, height, xloc, yloc)
{
	mode = S_PLACECASTLE;
	imageHover = new MouseImage(this, "castle.png", "castle.png", 128);

	castle = NULL;

	actionBar = new ActionBar(this, 40, 0, 400, 100, "");


	XMLDocument doc;
	if (!doc.LoadFile(path.c_str()))
	{
		Logger::errorFormat("Error loading Map File %s", path.c_str());
	}

	int _width = atoi(doc.FirstChildElement("Map")->FirstChildElement("Width")->GetText());
	int _height = atoi(doc.FirstChildElement("Map")->FirstChildElement("Height")->GetText());
	int _layers = atoi(doc.FirstChildElement("Map")->FirstChildElement("Layers")->GetText());
	map = new ScrollingMap(_width, _height, 32, 32, _layers);

	for(const XMLNode* node=doc.FirstChildElement("Map")->FirstChildElement("Tiles")->FirstChildElement("Tile");
		node; node=node->NextSibling())
	{
		int id = atoi(node->FirstChildElement("Id")->GetText());
		Uint8 r = 0, g = 0, b = 0;

		//I'll bet this is a 'bad' way to convert strings to Uint8's...but its the only
		//way I can figure out
		if(node->FirstChildElement("RMask") != NULL)
			r = atoi(node->FirstChildElement("RMask")->GetText()) % sizeof(r);

		if(node->FirstChildElement("GMask") != NULL)
			g = atoi(node->FirstChildElement("GMask")->GetText()) % sizeof(g);

		if(node->FirstChildElement("BMask") != NULL)
			b = atoi(node->FirstChildElement("BMask")->GetText()) % sizeof(b);

		map->addTile(id, new Image(node->FirstChildElement("Path")->GetText(), r, g, b));
	}

	int layer = 0;
	for(const XMLNode* node = doc.FirstChildElement("Map")->FirstChildElement("Data")->FirstChildElement("DataLayer");
		node; node=node->NextSibling())
	{
		Tokenizer t(node->ToElement()->GetText());

		int i = 0;
		int* data = new int[_width * _height];
		
		while (t.NextToken())
		{
			data[i++] = atoi(t.GetToken().c_str());
		}

		map->addLayer(layer++, data);

		delete[] data;
	}
}

SimState::~SimState()
{
	delete map;

	if(imageHover != NULL)
		delete imageHover;

	if(castle != NULL)
		delete castle;
	
	if(actionBar != NULL)
		delete actionBar;

	vector<House*>::iterator hit;
	for(hit = houses.begin(); hit != houses.end(); ++hit)
	{
		delete (*hit);

		houses.erase(hit);
	}

	houses.clear();

	vector<Farm*>::iterator fit;
	for(fit = farms.begin(); fit != farms.end(); ++fit)
	{
		delete (*fit);

		farms.erase(fit);
	}

	farms.clear();
}

SimState::SimState(const SimState& data) : State(0, 0, 0, 0)
{
	throw VillageException("SimState Copy Constructor");
}

SimState& SimState::operator=(const SimState* rhs)
{
	throw VillageException("SimState Assignment Operator");
}

void SimState::update(float time, Uint8* keystrokes)
{
	map->update(time, keystrokes);

	vector<House*>::const_iterator hit;
	for(hit = houses.begin(); hit != houses.end(); ++hit)
	{
		(*hit)->update(time, keystrokes);
	}

	vector<Farm*>::const_iterator fit;
	for(fit = farms.begin(); fit != farms.end(); ++fit)
	{
		(*fit)->update(time, keystrokes);
	}

	if(keystrokes[SDLK_ESCAPE] && mode != S_PLACECASTLE)
	{
		if(imageHover != NULL)
		{
			delete imageHover;
			imageHover = NULL;

			mode = S_NORMAL;
		}
	}
}

void SimState::raiseEvent(SDL_Event* event)
{
	map->raiseEvent(event);

	if(actionBar != NULL && mode == S_NORMAL)
		actionBar->raiseEvent(event);

	if(imageHover != NULL)
		imageHover->raiseEvent(event);

	if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
	{
		switch(mode)
		{
		case S_PLACECASTLE:
		{
			if(castle == NULL)
			{
				castle = new Castle("castle.png", imageHover->getX(), imageHover->getY());

				Logger::debugFormat("Castle placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;
				
				imageHover = NULL;

			}
			else
			{
				Logger::error("Tried to place a second castle");
			}
			break;
		}

		case S_PLACEHOUSE:
		{
			House* house = new House("house.png", imageHover->getX(), imageHover->getY());
			houses.push_back(house);

			Logger::debugFormat("House placed at (%i, %i)", imageHover->getX(), imageHover->getY());

			mode = S_NORMAL;

			if(imageHover != NULL)
				delete imageHover;

			imageHover = NULL;

			break;
		}

		case S_PLACEFARM:
		{
			Farm* farm = new Farm("farm.png", imageHover->getX(), imageHover->getY());
			farms.push_back(farm);

			Logger::debugFormat("Farm placed at (%i, %i)", imageHover->getX(), imageHover->getY());

			mode = S_NORMAL;

			if(imageHover != NULL)
				delete imageHover;

			imageHover = NULL;

			break;
		}
		}
	}
}


void SimState::draw()
{
	SDL_FillRect(frame, &frame->clip_rect, SDL_MapRGB(frame->format, 0x00, 0x00, 0x00));

	map->draw(frame);

	int xoffset = map->getXOffset();
	int yoffset = map->getYOffset();

	if(castle != NULL)
		castle->draw(xoffset, yoffset, frame);

	if(imageHover != NULL)
		imageHover->draw(frame);

	if(actionBar != NULL && mode == S_NORMAL)
		actionBar->draw(frame);

	vector<House*>::const_iterator hit;
	for(hit = houses.begin(); hit != houses.end(); ++hit)
	{
		(*hit)->draw(xoffset, yoffset, frame);
	}

	vector<Farm*>::const_iterator fit;
	for(fit = farms.begin(); fit != farms.end(); ++fit)
	{
		(*fit)->draw(xoffset, yoffset, frame);
	}
}

MouseImageMode SimState::checkCollision(MouseImage* img)
{
	if(castle != NULL)
		if(castle->collides(img))
			return MI_BAD;

	vector<House*>::const_iterator hit;
	for(hit = houses.begin(); hit != houses.end(); ++hit)
		if((*hit)->collides(img))
			return MI_BAD;

	vector<Farm*>::const_iterator fit;
	for(fit = farms.begin(); fit != farms.end(); ++fit)
		if((*fit)->collides(img))
			return MI_BAD;

	return MI_NORMAL;
}

void SimState::placeHouse()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEHOUSE;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "house.png", "house.png", 128);
	}
}

void SimState::placeFarm()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEFARM;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "farm.png", "farm.png", 128);
	}
}