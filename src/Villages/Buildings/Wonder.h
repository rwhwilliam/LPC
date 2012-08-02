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

#ifndef WONDER_H
#define WONDER_H

#include <string>
#include <list>

#include "Building.h"

using namespace std;

class SimState;
class Road;

class Wonder : public Building
{
public:
	Wonder(SimState* state, int xloc, int yloc);
	~Wonder();

	Wonder(const Wonder& data);
	Wonder& operator=(const Wonder* rhs);

	BuildingType getType() { return BT_WONDER; }
	int getRating() { return 1; }
	void generate();
	void generate(list<Road*>& network);

	bool canPurchase();
	void purchase();

	bool hasWon() { return (workerTurns <= 0); }

	void update(float time, Uint8* keystrokes);

private:
	int workerTurns;
};

#endif