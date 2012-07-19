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

#include <string>
#include <vector>

#include "SDL.h"

#include "Engine/State/State.h"
#include "Engine/Util/Enums.h"
#include "Villages/Util/MouseImage.h"

using namespace std;

enum SimMode { S_NORMAL, S_PLACECASTLE, S_PLACEHOUSE, S_PLACEFARM, S_PLACEMININGCAMP, S_PLACEMILL, S_PLACEWELL, S_PLACETAVERN };

class MouseImage;
class Castle;
class ScrollingMap;
class ActionBar;
class House;
class Farm;
class MiningCamp;
class Mill;
class CaveTile;
class ForestTile;
class Well;
class Tavern;

class SimState : public State
{
public:
	SimState(string path, int width, int height, int xloc, int yloc);
	~SimState();

	SimState(const SimState& data);
	SimState& operator=(const SimState* rhs);

	void update(float time, Uint8* keystrokes);
	void raiseEvent(SDL_Event* event);
	void draw();

	ScrollingMap* getMap();
	int getTileWidth();
	int getTileHeight();

	EngineResult canBuild(int x, int y, int width, int height);

	void placeHouse();
	void placeFarm();
	void placeMiningCamp();
	void placeMill();
	void placeWell();
	void placeTavern();

private:

	int tileWidth, tileHeight;
	
	SimMode mode;
	ScrollingMap* map;
	MouseImage* imageHover;
	Castle* castle;
	ActionBar* actionBar;

	vector<House*> houses;
	vector<Farm*> farms;
	vector<MiningCamp*> camps;
	vector<Mill*> mills;
	vector<Well*> wells;
	vector<Tavern*> taverns;

	vector<CaveTile*> caves;
	vector<ForestTile*> forests;
};

#endif