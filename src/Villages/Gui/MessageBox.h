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

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <string>
#include <list>

#include "SDL.h"

using namespace std;

class Timer;
class Font;
class Message;
class Image;

class MessageBox
{
public:
	MessageBox(int x, int y, int width, int height);
	~MessageBox();

	MessageBox(const MessageBox& data);
	MessageBox& operator=(const MessageBox* rhs);

	void addMessage(string msg);

	void update(float time, Uint8* keystrokes);
	void draw(SDL_Surface* screen);

private:
	int xloc, yloc;
	int width, height;
	Timer* timer;
	Font* font;
	list<Message*> msgs;
	Image* bg;
};

#endif