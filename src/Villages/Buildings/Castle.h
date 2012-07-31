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

#ifndef CASTLE_H
#define CASTLE_H

#include <string>

#include "Building.h"

using namespace std;

class SimState;

class Castle : public Building
{
public:
	Castle(SimState* state, int xloc, int yloc);
	~Castle();

	Castle(const Castle& data);
	Castle& operator=(const Castle* rhs);

	BuildingType getType() { return BT_CASTLE; }
	int getRating() { return 1; }
	void generate() { }

	int getTaxRate() { return tax; }
	int getGold() { return gold; }
	int getFood() { return food; }
	int getWood() { return wood; }
	int getOre() { return ore; }
	int getWeapons() { return weapons; }

	void setTaxRate(int val);

	int takeGold(int val);
	int takeFood(int val);
	int takeWood(int val);
	int takeOre(int val);
	int takeWeapons(int val);

	void addGold(int val) { gold += val; }
	void addFood(int val) { food += val; }
	void addWood(int val) { wood += val; }
	void addOre(int val) { ore += val; }
	void addWeapons(int val) { weapons += val; }

	bool canPurchase() { return false; }
	void purchase() { }

private:
	int gold, food, wood, ore, weapons;
	int tax;
};

#endif