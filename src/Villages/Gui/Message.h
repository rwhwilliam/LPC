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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "SDL.h"

using namespace std;

class Font;

class Message
{
public:
	Message(Font* font, string msg, int time);
	~Message();

	Message(const Message& data);
	Message& operator=(const Message* rhs);

	string getMessage() { return msg; }
	int getTime() { return time; }
	SDL_Surface* getLine() { return line; }

private:
	string msg;
	int time;
	SDL_Surface* line;
};

#endif