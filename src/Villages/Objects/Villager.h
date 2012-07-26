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

#ifndef VILLAGER_H
#define VILLAGER_H

enum Job { IDLE, FARM };

class House;
class Building;

class Villager
{
public:
	Villager(Building* job, House* residence);
	~Villager();

	Villager(const Villager& data);
	Villager& operator=(const Villager* rhs);

	void setJob(Building* job) { Villager::job = job; }
	void setResidence(House* house) { Villager::residence = house; }
	Building* getJob() { return job; }
	House* getHouse() { return residence; }

private:
	Building* job;
	House* residence;
};

#endif