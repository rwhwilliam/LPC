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
#include <map>
#include <stdlib.h>

#include "tinyxml2.h"
#include "SDL.h"
#include "SDL_mixer.h"


#include "Engine/Graphics/Image.h"
#include "Engine/Sound/SoundLoader.h"
#include "Engine/State/StateManager.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Enums.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Tokenizer.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Buildings/Bakery.h"
#include "Villages/Buildings/Blacksmith.h"
#include "Villages/Buildings/Building.h"
#include "Villages/Buildings/Castle.h"
#include "Villages/Buildings/GuardStation.h"
#include "Villages/Buildings/House.h"
#include "Villages/Buildings/Farm.h"
#include "Villages/Buildings/Jeweler.h"
#include "Villages/Buildings/Market.h"
#include "Villages/Buildings/MiningCamp.h"
#include "Villages/Buildings/Mill.h"
#include "Villages/Buildings/Tavern.h"
#include "Villages/Buildings/Theatre.h"
#include "Villages/Buildings/Wonder.h"
#include "Villages/Buildings/Weaver.h"
#include "Villages/Buildings/Well.h"
#include "Villages/Gui/ActionBar.h"
#include "Villages/Gui/MessageBox.h"
#include "Villages/Gui/ResourceBar.h"
#include "Villages/Map/CaveTile.h"
#include "Villages/Map/ForestTile.h"
#include "Villages/Objects/Road.h"
#include "Villages/Objects/RoadFactory.h"
#include "Villages/Objects/Villager.h"
#include "Villages/States/AssignState.h"
#include "Villages/Util/MouseImage.h"
#include "Villages/Util/ScrollingMap.h"
#include "Villages/Util/Util.h"

using namespace std;
using namespace tinyxml2;

SimState::SimState(StateManager* manager, string path, int width, int height, int xloc, int yloc) : State(manager, width, height, xloc, yloc)
{
	zoomLevel = 1;
	turn = 1;
	newPop = 0;

	music = SoundLoader::loadMusic("Calmtown.ogg");

	if(Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(SoundLoader::getMusic(music), -1);
	}

	mode = S_PLACECASTLE;
	imageHover = new MouseImage(this, "castle.png", "castle-bad.png", 128);

	castle = NULL;
	wonder = NULL;

	actionBar = new ActionBar(this, 176, 698, 400, 100, "");
	endTurnBtn = new ClickableButton<SimState>(825, 55, 128, 64, "end-button-normal.png", "end-button-hover.png", "end-button-pressed.png", this, &SimState::startEndTurn);
	msgBox = new MessageBox(0, 50, 210, 250);

	roadCreator = NULL;

	bar = new ResourceBar(this);

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
	map = new ScrollingMap(this, _width, _height, _tilewidth, _tileheight, _layers);

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
	Mix_HaltMusic();
	Mix_FreeMusic(SoundLoader::getMusic(music));

	delete map;

	if(imageHover != NULL)
		delete imageHover;

	if(castle != NULL)
		delete castle;

	if(wonder != NULL)
		delete wonder;
	
	if(actionBar != NULL)
		delete actionBar;

	if(endTurnBtn != NULL)
		delete endTurnBtn;

	if(roadCreator != NULL)
		delete roadCreator;

	delete bar;
	delete msgBox;

	vector<Building*>::iterator bit;
	for(bit = buildings.begin(); bit != buildings.end(); ++bit)
	{
		delete (*bit);

		//buildings.erase(bit);
	}

	vector<CaveTile*>::iterator cit;
	for(cit = caves.begin(); cit != caves.end(); ++cit)
	{
		delete (*cit);

		//caves.erase(cit);
	}

	caves.clear();

	vector<ForestTile*>::iterator foit;
	for(foit = forests.begin(); foit != forests.end(); ++foit)
	{
		delete (*foit);

		//forests.erase(foit);
	}

	forests.clear();

	std::map<string, Road*>::iterator rit;
	for(rit = roads.begin(); rit != roads.end(); ++rit)
	{
		delete rit->second;

		//roads.erase(rit);
	}

	roads.clear();

	list<Villager*>::iterator vit;
	for(vit = villagers.begin(); vit != villagers.end(); ++vit)
	{
		delete (*vit);

		//villagers.erase(vit);
	}

	villagers.clear();
}

SimState::SimState(const SimState& data) : State(NULL, 0, 0, 0, 0)
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

int SimState::getXOffset()
{
	return map->getXOffset();
}

int SimState::getYOffset()
{
	return map->getYOffset();
}

void SimState::update(float time, Uint8* keystrokes)
{
	map->update(time, keystrokes);

	bar->update(time, keystrokes);
	msgBox->update(time, keystrokes);

	if(imageHover != NULL)
		imageHover->update(time, keystrokes);

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
		
		if(roadCreator != NULL)
		{
			delete roadCreator;
			roadCreator = NULL;

			mode = S_NORMAL;
		}

		if(mode = S_DELETE)
			mode = S_NORMAL;
	}
}

void SimState::raiseEvent(SDL_Event* event)
{
	map->raiseEvent(event);
	bar->raiseEvent(event);

	if(actionBar != NULL && mode == S_NORMAL)
		actionBar->raiseEvent(event);

	if(endTurnBtn != NULL && mode == S_NORMAL)
		endTurnBtn->raiseEvent(event);

	if(imageHover != NULL)
		imageHover->raiseEvent(event);

	if(roadCreator != NULL && mode == S_PLACEROADEND)
		roadCreator->raiseEvent(event);

	if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
	{
		switch(mode)
		{
		case S_PLACECASTLE:
		{
			if(castle == NULL)
			{
				if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
				{
					castle = new Castle(this, imageHover->getX(), imageHover->getY());

					Logger::debugFormat("Castle placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;
				
					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Cannot build there!");
				}

			}
			else
			{
				Logger::error("Tried to place a second castle");
			}
			break;
		}

		case S_PLACEWONDER:
		{
			if(wonder == NULL)
			{
				if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
				{
					wonder = new Wonder(this, imageHover->getX(), imageHover->getY());

					Logger::debugFormat("Wonder placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;
				
					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Cannot build there!");
				}

			}
			else
			{
				Logger::error("Tried to place a second wonder");
			}
			break;
		}

		case S_PLACEHOUSE:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				House* house = new House(this, imageHover->getX(), imageHover->getY());

				if(house->canPurchase())
				{
					house->purchase();
					buildings.push_back(house);

					Logger::debugFormat("House placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;

					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Not enough resources!");
				}
			}
			else
			{
				msgBox->addMessage("Cannot build there!");
			}

			break;
		}

		case S_PLACEFARM:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Farm* farm = new Farm(this, imageHover->getX(), imageHover->getY());

				if(farm->canPurchase())
				{
					farm->purchase();
					buildings.push_back(farm);

					Logger::debugFormat("Farm placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;

					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Not enough resources!");
				}
			}
			else
			{
				msgBox->addMessage("Cannot build there!");
			}

			break;
		}

		case S_PLACEMININGCAMP:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				MiningCamp* camp = new MiningCamp(this, imageHover->getX(), imageHover->getY());

				if(camp->canPurchase())
				{
					camp->purchase();
					buildings.push_back(camp);

					Logger::debugFormat("Mining Camp placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;

					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Not enough resources!");
				}
			}
			else
			{
				msgBox->addMessage("Cannot build there!");
			}

			break;
		}

		case S_PLACEMILL:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Mill* mill = new Mill(this, imageHover->getX(), imageHover->getY());

				if(mill->canPurchase())
				{
					mill->purchase();
					buildings.push_back(mill);

					Logger::debugFormat("Mill placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;

					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Not enough resources!");
				}
			}
			else
			{
				msgBox->addMessage("Cannot build there!");
			}

			break;
		}

		case S_PLACEWELL:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Well* well = new Well(this, imageHover->getX(), imageHover->getY());

				if(well->canPurchase())
				{
					well->purchase();
					buildings.push_back(well);

					Logger::debugFormat("Well placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;

					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Not enough resources!");
				}
			}
			else
			{
				msgBox->addMessage("Cannot build there!");
			}

			break;
		}

		case S_PLACETAVERN:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Tavern* tavern = new Tavern(this, imageHover->getX(), imageHover->getY());
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
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Theatre* theatre = new Theatre(this, imageHover->getX(), imageHover->getY());
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
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Weaver* weaver = new Weaver(this, imageHover->getX(), imageHover->getY());
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
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Jeweler* jeweler = new Jeweler(this, imageHover->getX(), imageHover->getY());
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
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Blacksmith* blacksmith = new Blacksmith(this, imageHover->getX(), imageHover->getY());

				if(blacksmith->canPurchase())
				{
					blacksmith->purchase();
					buildings.push_back(blacksmith);

					Logger::debugFormat("Blacksmith placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;

					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Not enough resources!");
				}
			}
			else
			{
				msgBox->addMessage("Cannot build there!");
			}

			break;
		}

		case S_PLACEBAKERY:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Bakery* bakery = new Bakery(this, imageHover->getX(), imageHover->getY());
				buildings.push_back(bakery);

				Logger::debugFormat("Bakery placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEGUARDSTATION:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				GuardStation* guardStation = new GuardStation(this, imageHover->getX(), imageHover->getY());

				if(guardStation->canPurchase())
				{
					guardStation->purchase();
					buildings.push_back(guardStation);

					Logger::debugFormat("GuardStation placed at (%i, %i)", imageHover->getX(), imageHover->getY());

					mode = S_NORMAL;

					if(imageHover != NULL)
						delete imageHover;

					imageHover = NULL;
				}
				else
				{
					msgBox->addMessage("Not enough resources!");
				}
			}
			else
			{
				msgBox->addMessage("Cannot build here!");
			}

			break;
		}

		case S_PLACEMARKET:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) == E_GOOD)
			{
				Market* market = new Market(this, imageHover->getX(), imageHover->getY());
				buildings.push_back(market);

				Logger::debugFormat("Market placed at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_NORMAL;

				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}

			break;
		}

		case S_PLACEROADSTART:
		{
			if(canBuild(imageHover->getMapX(), imageHover->getMapY(), imageHover->getWidth(), imageHover->getHeight()) != E_BAD)
			{
				Logger::debugFormat("Placed Road Start at (%i, %i)", imageHover->getX(), imageHover->getY());

				mode = S_PLACEROADEND;

				if(roadCreator != NULL)
					delete roadCreator;

				roadCreator = new RoadFactory(this, imageHover->getX(), imageHover->getY());
			
				if(imageHover != NULL)
					delete imageHover;

				imageHover = NULL;
			}
			else
			{
				msgBox->addMessage("Cannot build here!");
			}

			break;
		}

		case S_PLACEROADEND:
		{
			if(roadCreator->canBuild())
			{
				Logger::debugFormat("Placed Road End at (%i, %i)", roadCreator->getFinishX(), roadCreator->getFinishY());

				mode = S_NORMAL;

				roadCreator->createRoad();

				std::map<string, Road*>::iterator ittr;
					for(ittr = roads.begin(); ittr != roads.end(); ++ittr)
						ittr->second->calculateMode();

				delete roadCreator;
				roadCreator = NULL;
			}
			else
			{
				msgBox->addMessage("Cannot build here!");
			}

			break;
		}

		case S_DELETE:
		{
			vector<Building*>::const_iterator it;
			for(it = buildings.begin(); it != buildings.end(); ++it)
			{
				if((*it)->collides(event->motion.x, event->motion.y, 1, 1))
				{
					list<Villager*>::iterator ittr;
					for(ittr = villagers.begin(); ittr != villagers.end(); ++ittr)
					{
						if((*ittr)->getHouse() == *it || (*ittr)->getJob() == *it)
						{
							Logger::debug("Citizen Leaving");

							villagers.remove(*ittr);
						}
					}

					Logger::debug("Demolishing Building");
					msgBox->addMessage("Building Destroyed");

					buildings.erase(it);
					break;
				}
			}

			
			std::map<string, Road*>::iterator itt;
			for(itt = roads.begin(); itt != roads.end(); ++itt)
			{
				if(itt->second->collides(event->motion.x, event->motion.y, 1, 1))
				{
					roads.erase(itt);

					Logger::debug("Demolishing Road");
					msgBox->addMessage("Road Destroyed");

					std::map<string, Road*>::iterator ittr;
					for(ittr = roads.begin(); ittr != roads.end(); ++ittr)
						ittr->second->calculateMode();

					break;
				}
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

	if(wonder != NULL)
		wonder->draw(xoffset, yoffset, frame);

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

	std::map<string, Road*>::const_iterator rit;
	for(rit = roads.begin(); rit != roads.end(); ++rit)
	{
		rit->second->draw(xoffset, yoffset, frame);
	}

	if(roadCreator != NULL && mode == S_PLACEROADEND)
		roadCreator->draw(xoffset, yoffset, frame);

	if(actionBar != NULL && mode == S_NORMAL)
		actionBar->draw(frame);

	bar->draw(frame);
	msgBox->draw(frame);

	if(endTurnBtn != NULL && mode == S_NORMAL)
		endTurnBtn->draw(frame);
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

void SimState::placeGuardStation()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEGUARDSTATION;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "guardstation.png", "guardstation-bad.png", 128);
	}
}

void SimState::placeMarket()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEMARKET;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "market.png", "market-bad.png", 128);
	}
}

void SimState::placeRoad()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEROADSTART;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "road-horizontal.png", "road-bad.png", 128);
	}
}

void SimState::placeWonder()
{
	if(mode == S_NORMAL)
	{
		mode = S_PLACEWONDER;

		if(imageHover != NULL)
			delete imageHover;

		imageHover = new MouseImage(this, "wonder.png", "wonder-bad.png", 128);
	}
}

void SimState::zoomIn()
{
	zoomLevel = (zoomLevel * 2.0 < 1) ? zoomLevel * 2.0 : 1;

	changeZoom();

	Logger::debugFormat("Zooming In to %f", zoomLevel);
}

void SimState::zoomOut()
{
	zoomLevel = (zoomLevel / 2.0 > .25) ? zoomLevel / 2.0 : .25;

	changeZoom();

	Logger::debugFormat("Zooming In to %f", zoomLevel);
}

void SimState::deleteStuff()
{
	if(mode == S_NORMAL)
	{
		mode = S_DELETE;

		if(imageHover != NULL)
			delete imageHover;

	}
}

void SimState::changeZoom()
{
	tileWidth = static_cast<int>(atoi(Config::getConfig("TileWidth").c_str()) * zoomLevel);
	tileHeight = static_cast<int>(atoi(Config::getConfig("TileHeight").c_str()) * zoomLevel);

	map->setTileWidth(tileWidth);
	map->setTileHeight(tileHeight);

	castle->resize();

	vector<Building*>::iterator bit;
	for(bit = buildings.begin(); bit != buildings.end(); ++bit)
	{
		(*bit)->resize();
	}

	map->resize();

	vector<CaveTile*>::iterator cit;
	for(cit = caves.begin(); cit != caves.end(); ++cit)
	{
		(*cit)->resize();
	}

	vector<ForestTile*>::iterator fit;
	for(fit = forests.begin(); fit != forests.end(); ++fit)
	{
		(*fit)->resize();
	}

	std::map<string, Road*>::iterator rit;
	for(rit = roads.begin(); rit != roads.end(); ++rit)
	{
		rit->second->resize();
	}

	if(imageHover != NULL)
		imageHover->resize();

	if(roadCreator != NULL)
		roadCreator->resize();
}

EngineResult SimState::canBuild(int x, int y, int width, int height)
{
	if(castle != NULL)
		if(castle->collides(x, y, width, height))
			return E_BAD;

	if(wonder != NULL)
		if(wonder->collides(x, y, width, height))
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

	std::map<string, Road*>::const_iterator rit;
	for(rit = roads.begin(); rit != roads.end(); ++rit)
		if(rit->second->collides(x, y, width, height))
			return E_BADROAD;

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
			if((abs(((*cit)->getMapX() + (*cit)->getWidth() / 2 - map->getXOffset()) - (x + width / 2)) <= 256) && (abs(((*cit)->getMapY() + (*cit)->getHeight() / 2 - map->getYOffset()) - (y + height / 2)) <= 256))
				return E_GOOD;

		return E_BAD;

		break;
	}

	case S_PLACEMILL:
	{
		vector<ForestTile*>::const_iterator fit;
		for(fit = forests.begin(); fit != forests.end(); ++fit)
			if((abs(((*fit)->getMapX() + (*fit)->getWidth() / 2 - map->getXOffset()) - (x + width / 2)) <= 256) && (abs(((*fit)->getMapY() + (*fit)->getHeight() / 2 - map->getYOffset()) - (y + height / 2)) <= 256))
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

int SimState::getPopRoom()
{
	int ret = 0;

	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() == BT_HOUSE)
			ret += dynamic_cast<House*>(*it)->getRoom();
		
	return ret;
}

int SimState::getWorkRoom()
{
	return getFarmRoom() + getMillRoom() + getMineRoom() + getBlacksmithRoom();
}

int SimState::getFarmRoom()
{
	int ret = 0;

	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() == BT_FARM)
			ret += (*it)->getRoom();
			//ret += dynamic_cast<House*>(*it)->getRoom();
		
	return ret;
}

int SimState::getMillRoom()
{
	int ret = 0;

	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() == BT_MILL)
			ret += (*it)->getRoom();
		
	return ret;
}

int SimState::getMineRoom()
{
	int ret = 0;

	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() == BT_MININGCAMP)
			ret += (*it)->getRoom();
		
	return ret;
}

int SimState::getBlacksmithRoom()
{
	int ret = 0;

	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() == BT_BLACKSMITH)
			ret += (*it)->getRoom();
		
	return ret;
}

int SimState::getSpareWater()
{
	int ret = 0;

	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() == BT_WELL)
			ret++;
		
	return ret * 15 - villagers.size();
}

int SimState::getWonderRoom()
{
	if(wonder != NULL)
		return wonder->getRoom();

	return 0;
}

void SimState::findHouse(Villager* person)
{
	House* temp = NULL;
	int val = - 1000;
	
	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
	{
		if((*it)->getType() == BT_HOUSE)
		{
			if((*it)->getRating() > val)
			{
				val = (*it)->getRating();
				temp = dynamic_cast<House*>(*it);
			}
		}
	}

	person->setResidence(temp);
}

void SimState::findFarm(Villager* person)
{
	Farm* temp = NULL;
	int val = -1000;
	
	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
	{
		if((*it)->getType() == BT_FARM)
		{
			if((*it)->getRating() > val)
			{
				val = (*it)->getRating();
				temp = dynamic_cast<Farm*>(*it);
			}
		}
	}

	person->setJob(temp);
}

void SimState::findMill(Villager* person)
{
	Mill* temp = NULL;
	int val = -1000;
	
	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
	{
		if((*it)->getType() == BT_MILL)
		{
			if((*it)->getRating() > val)
			{
				val = (*it)->getRating();
				temp = dynamic_cast<Mill*>(*it);
			}
		}
	}

	person->setJob(temp);
}

void SimState::findMine(Villager* person)
{
	MiningCamp* temp = NULL;
	int val = -1000;
	
	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
	{
		if((*it)->getType() == BT_MININGCAMP)
		{
			if((*it)->getRating() > val)
			{
				val = (*it)->getRating();
				temp = dynamic_cast<MiningCamp*>(*it);
			}
		}
	}

	person->setJob(temp);
}

void SimState::findBlacksmith(Villager* person)
{
	Blacksmith* temp = NULL;
	int val = -1000;
	
	vector<Building*>::const_iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
	{
		if((*it)->getType() == BT_BLACKSMITH)
		{
			if((*it)->getRating() > val)
			{
				val = (*it)->getRating();
				temp = dynamic_cast<Blacksmith*>(*it);
			}
		}
	}

	person->setJob(temp);
}

int SimState::getNewPopCount()
{
	int newPop = rand() % 3;

	if(getPopRoom() <= 0)
		return 0;

	if(getWorkRoom() <= 0)
		return 0;

	if(castle->getFood() == 0 && getSpareWater() <= 0)
		return 0;

	if(castle->getFood() == 0 || getSpareWater() <= 0)
		newPop -= 2;

	if(castle->getTaxRate() >= 25)
		return 0;

	if(castle->getTaxRate() >= 15 && castle->getTaxRate() < 25)
		newPop -= 1;

	if(castle->getTaxRate() == 0)
		newPop += 2;

	if(castle->getTaxRate() > 0 && castle->getTaxRate() <= 5)
		newPop += 1;

	return (newPop >= 0) ? newPop : 0;
}

void SimState::getRoadNetwork(list<Road*>& network)
{
	list<Road*> newRoads;

	//get roads surrounding the castle
	castle->getSurroundingRoads(newRoads);

	list<Road*>::iterator it;
	for(it = newRoads.begin(); it != newRoads.end(); ++it)
	{
		network.push_back(*it);
	}


	//generate the network
	//ie, get all roads connected to these roads till we run out of new connected roads :P
	while(true)
	{
		if(newRoads.empty())
			break;

		list<Road*> temp;

		list<Road*>::iterator it;
		for(it = newRoads.begin(); it != newRoads.end(); ++it)
			(*it)->getSurroundingRoads(temp);

		newRoads.clear();

		for(it = temp.begin(); it != temp.end(); ++it)
		{
			bool found = false;
			list<Road*>::iterator it2;
			for(it2 = network.begin(); it2 != network.end(); ++it2)
			{
				if(find(*it, *it2))
				{
					found = true;
					break;
				}
			}

			if(!found)
			{
				network.push_back(*it);
				newRoads.push_back(*it);
			}
		}
	}
}

int SimState::letPeopleLeave()
{
	list<Villager*> leaving;

	list<Villager*>::const_iterator it;
	for(it = villagers.begin(); it != villagers.end(); ++it)
		if((*it)->wantsToLeave())
			leaving.push_back(*it);

	for(it = leaving.begin(); it != leaving.end(); ++it)
	{
		villagers.remove(*it);
		delete (*it);
	}

	return leaving.size();
}

void SimState::startEndTurn()
{
	Logger::debugFormat("Ending turn %i.", turn);

	list<Road*> network;
	getRoadNetwork(network);

	Logger::debug("Generating Resources");

	vector<Building*>::iterator it;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		(*it)->generate(network);

	if(wonder != NULL)
		wonder->generate(network);

	Logger::debug("Updating Guard Station Coverage");

	list<GuardStation*> guards;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() == BT_GUARDSTATION)
			guards.push_back(dynamic_cast<GuardStation*>(*it));

	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() != BT_GUARDSTATION)
			(*it)->updateCoverage(guards);

	Logger::debug("Collecting Taxes");

	int unCovered = 0;
	for(it = buildings.begin(); it != buildings.end(); ++it)
		if((*it)->getType() != BT_GUARDSTATION)
			if(!(*it)->isGuardCovered())
				unCovered++;

	Logger::debugFormat("%i Buildings Uncovered", unCovered);

	int taxes = 50 + (int)(villagers.size() * 100 * castle->getTaxRate() / 100.0) - 5 * unCovered;

	Logger::debugFormat("%i Taxes Collected", taxes);

	castle->addGold(taxes);
	castle->takeFood(villagers.size());

	int left = letPeopleLeave();

	Logger::debugFormat("%i People Left", left);

	newPop = getNewPopCount();

	Logger::debugFormat("%i People Joined", newPop);

	if(newPop == 0)
	{
		Logger::debugFormat("%i Villages Joined", newPop);

		manager->push(new AssignState(manager, this, newPop, 1024, 768, 0, 0));
	}
	else
	{
		//skip the assigning of people
		finishEndTurn();
	}

	newPop -= left;
}

void SimState::assignEndTurn(int pop, int farm, int mill, int mine, int blacksmith, int wonder)
{
	if(pop >= farm + mill + mine + blacksmith + wonder)
	{
		if(farm <= getFarmRoom())
		{
			for(int i = 0; i < farm; ++i)
			{
				Villager* v = new Villager(this, NULL, NULL);
				findHouse(v);
				findFarm(v);
				villagers.push_back(v);
			}
		}

		if(mill <= getMillRoom())
		{
			for(int i = 0; i < mill; ++i)
			{
				Villager* v = new Villager(this, NULL, NULL);
				findHouse(v);
				findMill(v);
				villagers.push_back(v);
			}
		}

		if(mine <= getMineRoom())
		{
			for(int i = 0; i < mine; ++i)
			{
				Villager* v = new Villager(this, NULL, NULL);
				findHouse(v);
				findMine(v);
				villagers.push_back(v);
			}
		}

		if(blacksmith <= getBlacksmithRoom())
		{
			for(int i = 0; i < blacksmith; ++i)
			{
				Villager* v = new Villager(this, NULL, NULL);
				findHouse(v);
				findBlacksmith(v);
				villagers.push_back(v);
			}
		}

		if(getWonder() != NULL)
		{
			if(wonder <= getWonderRoom())
			{
				for(int i = 0; i < wonder; ++i)
				{
					Villager* v = new Villager(this, NULL, NULL);
					findHouse(v);
					v->setJob(getWonder());
					villagers.push_back(v);
				}
			}
		}
	}

	finishEndTurn();
}

void SimState::finishEndTurn()
{
	turn++;
}