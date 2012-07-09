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

#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include "SDL.h"

using namespace std;

class Image;
class MouseImage;

class Object
{
public:
	Object(string src, int xloc, int yloc, int width, int height);
	~Object();

	Object(const Object& data);
	Object& operator=(const Object* rhs);

	bool collides(Object* obj);
	bool collides(MouseImage* obj);

	virtual void update(float time, Uint8* keystrokes);
	virtual void raiseEvent(SDL_Event* event);
	virtual void draw(int xoffset, int yoffset, SDL_Surface* screen);

private:
	Image* img;
	int xloc, yloc, width, height;
};

#endif