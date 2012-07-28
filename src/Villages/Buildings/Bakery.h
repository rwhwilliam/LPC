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

#ifndef BAKERY_H
#define BAKERY_H

#include <string>

#include "Building.h"

using namespace std;

class SimState;

class Bakery : public Building
{
public:
	Bakery(SimState* state, int xloc, int yloc);
	~Bakery();

	Bakery(const Bakery& data);
	Bakery& operator=(const Bakery* rhs);

	BuildingType getType() { return BT_BAKERY; }
	int getRating() { return 1; }
	void generate() { }
};

#endif