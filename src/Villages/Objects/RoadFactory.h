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

#ifndef ROADFACTORY_H
#define ROADFACTORY_H

#include <vector>

#include "SDL.h"

using namespace std;

class SimState;
class Road;
class Image;

class RoadFactory
{
public:
	RoadFactory(SimState* state, int startX, int startY);
	~RoadFactory();

	RoadFactory(const RoadFactory& data);
	RoadFactory& operator=(const RoadFactory* rhs);

	int getFinishX() { return finishX; }
	int getFinishY() { return finishY; }

	bool canBuild();
	void createRoad();

	void resize();

	void raiseEvent(SDL_Event* event);
	void draw(int xoffset, int yoffset, SDL_Surface* screen);

private:
	SimState* state;
	int startX, startY;
	int finishX, finishY;
	vector<pair<int, int> > coords;
	Image* preImg;
};

#endif