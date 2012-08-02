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

#ifndef BLACKSMITH_H
#define BLACKSMITH_H

#include <string>

#include "Building.h"

using namespace std;

class SimState;

class Blacksmith : public Building
{
public:
	Blacksmith(SimState* state, int xloc, int yloc);
	~Blacksmith();

	Blacksmith(const Blacksmith& data);
	Blacksmith& operator=(const Blacksmith* rhs);

	BuildingType getType() { return BT_BLACKSMITH; }
	int getRating() { return coverage; }
	void generate();

	bool canPurchase();
	void purchase();

	void update(float time, Uint8* keystrokes);
};

#endif