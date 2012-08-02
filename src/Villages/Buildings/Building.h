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

#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <list>

#include "SDL.h"

#include "Engine/Graphics/Image.h"

using namespace std;

class MouseImage;
class MapTile;
class SimState;
class Villager;
class Road;
class GuardStation;
class HoverImage;

enum BuildingType { BT_WONDER, BT_BAKERY, BT_BLACKSMITH, BT_CASTLE, BT_FARM, BT_GUARDSTATION, BT_HOUSE, BT_JEWELER, BT_MARKET, BT_MILL, BT_MININGCAMP, BT_TAVERN, BT_THEATRE, BT_WEAVER, BT_WELL };

class Building
{
public:
	Building(SimState* state, string src, int xloc, int yloc);
	~Building();

	Building(const Building& data);
	Building& operator=(const Building* rhs);

	int getX() { return xloc; }
	int getY() { return yloc; }
	int getMapX();
	int getMapY();
	int getWidth() { return img->getWidth(); }
	int getHeight() { return img->getHeight(); }

	bool collides(int x, int y, int width, int height);

	void resize();

	virtual void update(float time, Uint8* keystrokes);
	virtual void raiseEvent(SDL_Event* event);
	virtual void draw(int xoffset, int yoffset, SDL_Surface* screen);
	virtual BuildingType getType() = 0;
	virtual int getRating() = 0;

	virtual bool canPurchase() = 0;
	virtual void purchase() = 0;

	int getCapacity() { return capacity; }
	int getWorkerCount() { return workers.size(); }
	int getRoom() { return capacity - workers.size(); }
	int getCoverage() { return coverage; }

	bool hasRoom() { return (getWorkerCount() < capacity); }
	void addWorker(Villager* person);
	void removeWorker(Villager* person);

	void getSurroundingRoads(list<Road*>& temp);
	bool inNetwork(list<Road*>& network);
	void updateCoverage(list<GuardStation*>& guards);
	void generate(list<Road*>& network);
	bool isGuardCovered() { return (coverage > 0); }
	bool isRoadConnected() { return roadConnected; }

protected:
	virtual void generate() = 0;

	Image* img;
	int xloc, yloc;
	SimState* state;

	int capacity;
	list<Villager*> workers;
	int coverage;
	bool roadConnected;

	HoverImage* hover;
};

#endif