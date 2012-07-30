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

#ifndef SIMSTATE_H
#define SIMSTATE_H

#include <map>
#include <string>
#include <vector>
#include <list>

#include "SDL.h"

#include "Engine/Gui/ClickableButton.h"
#include "Engine/State/State.h"
#include "Engine/Util/Enums.h"
#include "Villages/Util/MouseImage.h"

using namespace std;

enum SimMode { S_NORMAL, S_PLACECASTLE, S_PLACEHOUSE, S_PLACEFARM, S_PLACEMININGCAMP, S_PLACEMILL, S_PLACEWELL, S_PLACETAVERN, S_PLACETHEATRE, S_PLACEWEAVER, S_PLACEJEWELER, S_PLACEBLACKSMITH, S_PLACEBAKERY, S_PLACEGUARDSTATION, S_PLACEMARKET, S_PLACEROADSTART, S_PLACEROADEND };

class MouseImage;
class Castle;
class ScrollingMap;
class ActionBar;
class CaveTile;
class ForestTile;
class Building;
class RoadFactory;
class Road;
class StateManager;
class Villager;
class ResourceBar;

class SimState : public State
{
public:
	SimState(StateManager* manager, string path, int width, int height, int xloc, int yloc);
	~SimState();

	SimState(const SimState& data);
	SimState& operator=(const SimState* rhs);

	void update(float time, Uint8* keystrokes);
	void raiseEvent(SDL_Event* event);
	void draw();

	ScrollingMap* getMap();
	int getTileWidth();
	int getTileHeight();
	int getXOffset();
	int getYOffset();
	float getZoomLevel() { return zoomLevel; }
	map<string, Road*>* getRoads() { return &roads; }
	vector<Building*>* getBuildings() { return &buildings; }

	EngineResult canBuild(int x, int y, int width, int height);

	void placeHouse();
	void placeFarm();
	void placeMiningCamp();
	void placeMill();
	void placeWell();
	void placeTavern();
	void placeTheatre();
	void placeWeaver();
	void placeJeweler();
	void placeBlacksmith();
	void placeBakery();
	void placeGuardStation();
	void placeMarket();
	void placeRoad();
	
	void zoomIn();
	void zoomOut();
	void changeZoom();

	void startEndTurn();
	void assignEndTurn(int pop, int farm, int mill, int mine, int blacksmith);
	void finishEndTurn();

	int getPop() { return villagers.size(); }
	int getTurn() { return turn; }
	int getNewPop() { return newPop; }
	int getPopRoom();
	int getWorkRoom();
	int getFarmRoom();
	int getMillRoom();
	int getMineRoom();
	int getBlacksmithRoom();
	int getSpareWater();

	void findHouse(Villager* person);
	void findFarm(Villager* person);
	void findMill(Villager* person);
	void findMine(Villager* person);
	void findBlacksmith(Villager* person);

	int getNewPopCount();
	int letPeopleLeave();

	Castle* getCastle() { return castle; }
	void getRoadNetwork(list<Road*>& network);

private:
	int tileWidth, tileHeight;
	int turn;
	float zoomLevel;
	int newPop;
	
	SimMode mode;
	ScrollingMap* map;
	MouseImage* imageHover;
	Castle* castle;

	vector<Building*> buildings;

	vector<CaveTile*> caves;
	vector<ForestTile*> forests;

	RoadFactory* roadCreator;

	std::map<string, Road*> roads;

	list<Villager*> villagers;

	ActionBar* actionBar;
	ClickableButton<SimState>* endTurnBtn;

	ResourceBar* bar;
};

#endif