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

#include <cmath>
#include <string>
#include <vector>

#include "tinyxml2.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Enums.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Tokenizer.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Gui/ActionBar.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Buildings/House.h"
#include "Villages/Buildings/Farm.h"
#include "Villages/Buildings/MiningCamp.h"
#include "Villages/Map/CaveTile.h"
#include "Villages/Util/MouseImage.h"
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
	int _tilewidth = atoi(Config::getConfig("TileWidth").c_str());
	int _tileheight = atoi(Config::getConfig("TileHeight").c_str());
	int _layers = atoi(doc.FirstChildElement("Map")->FirstChildElement("Layers")->GetText());
	map = new ScrollingMap(_width, _height, _tilewidth, _tileheight, _layers);

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

	for(const XMLNode* node=doc.FirstChildElement("Map")->FirstChildElement("Caves")->FirstChildElement("Cave");
		node; node=node->NextSibling())
	{
		int _x = atoi(node->FirstChildElement("X")->GetText());
		int _y = atoi(node->FirstChildElement("Y")->GetText());
		CaveTile* cave = new CaveTile(this, _x, _y);

		caves.push_back(cave);
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

	vector<CaveTile*>::iterator cit;
	for(cit = caves.begin(); cit != caves.end(); ++cit)
	{
		delete (*cit);

		caves.erase(cit);
	}

	caves.clear();

	vector<MiningCamp*>::iterator mit;
	for(mit = camps.begin(); mit != camps.end(); ++mit)
	{
		delete (*mit);

		camps.erase(mit);
	}

	camps.clear();
}

SimState::SimState(const SimState& data) : State(0, 0, 0, 0)
{
	throw VillageException("SimState Copy Constructor");
}

SimState& SimState::operator=(const SimState* rhs)
{
	throw VillageException("SimState Assignment Operator");
}

int SimState::getTileWidth()
{
	return map->getTileWidth();
}

int SimState::getTileHeight()
{
	return map->getTileHeight();
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

	vector<MiningCamp*>::const_iterator mit;
	for(mit = camps.begin(); mit != camps.end(); ++mit)
	{
		(*mit)->update(time, keystrokes);
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
				if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
				{
					castle = new Castle(imageHover->getX(), imageHover->getY());

					Logger::debugFormat("Castle placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;
				
					imageHover = NULL;
				}

			}
			else
			{
				Logger::error("Tried to place a second castle");
			}
			break;
		}

		case S_PLACEHOUSE:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				House* house = new House(imageHover->getX(), imageHover->getY());
				houses.push_back(house);

				Logger::debugFormat("House placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEFARM:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Farm* farm = new Farm(imageHover->getX(), imageHover->getY());
				farms.push_back(farm);

				Logger::debugFormat("Farm placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEMININGCAMP:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				MiningCamp* camp = new MiningCamp(imageHover->getX(), imageHover->getY());
				camps.push_back(camp);

				Logger::debugFormat("Mining Camp placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

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

	vector<CaveTile*>::const_iterator cit;
	for(cit = caves.begin(); cit != caves.end(); ++cit)
	{
		(*cit)->draw(xoffset, yoffset, frame);
	}

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

	vector<MiningCamp*>::const_iterator mit;
	for(mit = camps.begin(); mit != camps.end(); ++mit)
	{
		(*mit)->draw(xoffset, yoffset, frame);
	}

	if(actionBar != NULL && mode == S_NORMAL)
		actionBar->draw(frame);
}

void SimState::placeHouse()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEHOUSE;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "house.png", "house-bad.png", 128);
	}
}

void SimState::placeFarm()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEFARM;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "farm.png", "farm-bad.png", 128);
	}
}

void SimState::placeMiningCamp()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEMININGCAMP;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "miningcamp.png", "miningcamp-bad.png", 128);
	}
}

EngineResult SimState::canBuild(int x, int y, int width, int height)
{
	if(castle != NULL)
		if(castle->collides(x, y, width, height))
			return E_BAD;

	vector<House*>::const_iterator hit;
	for(hit = houses.begin(); hit != houses.end(); ++hit)
		if((*hit)->collides(x, y, width, height))
			return E_BAD;

	vector<Farm*>::const_iterator fit;
	for(fit = farms.begin(); fit != farms.end(); ++fit)
		if((*fit)->collides(x, y, width, height))
			return E_BAD;

	vector<MiningCamp*>::const_iterator mit;
	for(mit = camps.begin(); mit != camps.end(); ++mit)
		if((*mit)->collides(x, y, width, height))
			return E_BAD;

	vector<CaveTile*>::const_iterator cit;
	for(cit = caves.begin(); cit != caves.end(); ++cit)
		if((*cit)->collides(x, y, width, height))
			return E_BAD;

	switch(mode)
	{
	case S_PLACEHOUSE:

		break;

	case S_PLACEFARM:

		break;

	case S_PLACEMININGCAMP:
	{
		vector<CaveTile*>::const_iterator cit;
		for(cit = caves.begin(); cit != caves.end(); ++cit)
			if((abs(((*cit)->getMapX() + (*cit)->getWidth() / 2) - (x + width / 2)) <= 200) && (abs(((*cit)->getMapY() + (*cit)->getHeight() / 2) - (y + height / 2)) <= 200))
				return E_GOOD;

		return E_BAD;

		break;
	}
	default:
		//nothing to do here
		break;
	}

	return E_GOOD;
}