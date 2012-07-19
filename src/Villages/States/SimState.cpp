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
#include "Villages/Buildings/Bakery.h"
#include "Villages/Buildings/Blacksmith.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Buildings/House.h"
#include "Villages/Buildings/Farm.h"
#include "Villages/Buildings/Jeweler.h"
#include "Villages/Buildings/MiningCamp.h"
#include "Villages/Buildings/Mill.h"
#include "Villages/Buildings/Tavern.h"
#include "Villages/Buildings/Theatre.h"
#include "Villages/Buildings/Weaver.h"
#include "Villages/Buildings/Well.h"
#include "Villages/Map/CaveTile.h"
#include "Villages/Map/ForestTile.h"
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

	for(const XMLNode* node=doc.FirstChildElement("Map")->FirstChildElement("Forests")->FirstChildElement("Forest");
		node; node=node->NextSibling())
	{
		int _x = atoi(node->FirstChildElement("X")->GetText());
		int _y = atoi(node->FirstChildElement("Y")->GetText());
		ForestTile* forest = new ForestTile(this, _x, _y);

		forests.push_back(forest);
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

	vector<Building*>::iterator bit;
	for(bit = buildings.begin(); bit != buildings.end(); ++bit)
	{
		delete (*bit);

		buildings.erase(bit);
	}

	vector<CaveTile*>::iterator cit;
	for(cit = caves.begin(); cit != caves.end(); ++cit)
	{
		delete (*cit);

		caves.erase(cit);
	}

	caves.clear();

	vector<ForestTile*>::iterator foit;
	for(foit = forests.begin(); foit != forests.end(); ++foit)
	{
		delete (*foit);

		forests.erase(foit);
	}

	forests.clear();
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

	vector<Building*>::const_iterator bit;
	for(bit = buildings.begin(); bit != buildings.end(); ++bit)
	{
		(*bit)->update(time, keystrokes);
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
				buildings.push_back(house);

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
				buildings.push_back(farm);

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
				buildings.push_back(camp);

				Logger::debugFormat("Mining Camp placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEMILL:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Mill* mill = new Mill(imageHover->getX(), imageHover->getY());
				buildings.push_back(mill);

				Logger::debugFormat("Mill placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEWELL:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Well* well = new Well(imageHover->getX(), imageHover->getY());
				buildings.push_back(well);

				Logger::debugFormat("Well placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACETAVERN:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Tavern* tavern = new Tavern(imageHover->getX(), imageHover->getY());
				buildings.push_back(tavern);

				Logger::debugFormat("Tavern placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACETHEATRE:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Theatre* theatre = new Theatre(imageHover->getX(), imageHover->getY());
				buildings.push_back(theatre);

				Logger::debugFormat("Theatre placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEWEAVER:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Weaver* weaver = new Weaver(imageHover->getX(), imageHover->getY());
				buildings.push_back(weaver);

				Logger::debugFormat("Weaver placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEJEWELER:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Jeweler* jeweler = new Jeweler(imageHover->getX(), imageHover->getY());
				buildings.push_back(jeweler);

				Logger::debugFormat("Jeweler placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEBLACKSMITH:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Blacksmith* blacksmith = new Blacksmith(imageHover->getX(), imageHover->getY());
				buildings.push_back(blacksmith);

				Logger::debugFormat("Blacksmith placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEBAKERY:
		{
			if(canBuild(imageHover->getX(), imageHover->getY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Bakery* bakery = new Bakery(imageHover->getX(), imageHover->getY());
				buildings.push_back(bakery);

				Logger::debugFormat("Bakery placed at (%i, %i)", imageHover->getX(), imageHover->getY());

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

	vector<ForestTile*>::const_iterator foit;
	for(foit = forests.begin(); foit != forests.end(); ++foit)
	{
		(*foit)->draw(xoffset, yoffset, frame);
	}

	vector<Building*>::const_iterator bit;
	for(bit = buildings.begin(); bit != buildings.end(); ++bit)
	{
		(*bit)->draw(xoffset, yoffset, frame);
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

void SimState::placeMill()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEMILL;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "mill.png", "mill-bad.png", 128);
	}
}

void SimState::placeWell()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEWELL;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "well.png", "well-bad.png", 128);
	}
}

void SimState::placeTavern()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACETAVERN;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "tavern.png", "tavern-bad.png", 128);
	}
}

void SimState::placeTheatre()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACETHEATRE;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "theatre.png", "theatre-bad.png", 128);
	}
}

void SimState::placeWeaver()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEWEAVER;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "weaver.png", "weaver-bad.png", 128);
	}
}

void SimState::placeJeweler()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEJEWELER;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "jeweler.png", "jeweler-bad.png", 128);
	}
}

void SimState::placeBlacksmith()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEBLACKSMITH;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "blacksmith.png", "blacksmith-bad.png", 128);
	}
}

void SimState::placeBakery()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEBAKERY;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "bakery.png", "bakery-bad.png", 128);
	}
}

EngineResult SimState::canBuild(int x, int y, int width, int height)
{
	if(castle != NULL)
		if(castle->collides(x, y, width, height))
			return E_BAD;

	vector<Building*>::const_iterator bit;
	for(bit = buildings.begin(); bit != buildings.end(); ++bit)
		if((*bit)->collides(x, y, width, height))
			return E_BAD;

	vector<CaveTile*>::const_iterator cit;
	for(cit = caves.begin(); cit != caves.end(); ++cit)
		if((*cit)->collides(x, y, width, height))
			return E_BAD;

	vector<ForestTile*>::const_iterator foit;
	for(foit = forests.begin(); foit != forests.end(); ++foit)
		if((*foit)->collides(x, y, width, height))
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

	case S_PLACEMILL:
	{
		vector<ForestTile*>::const_iterator fit;
		for(fit = forests.begin(); fit != forests.end(); ++fit)
			if((abs(((*fit)->getMapX() + (*fit)->getWidth() / 2) - (x + width / 2)) <= 200) && (abs(((*fit)->getMapY() + (*fit)->getHeight() / 2) - (y + height / 2)) <= 200))
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