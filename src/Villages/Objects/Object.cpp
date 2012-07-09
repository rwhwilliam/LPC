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

#include "Object.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/MouseImage.h"
#include "Engine/Util/VillageException.h"

using namespace std;

Object::Object(string src, int xloc, int yloc, int width, int height) : xloc(xloc), yloc(yloc), width(width), height(height)
{
	Logger::debug("Object Constructor");

	img = new Image(src);
}

Object::~Object()
{
	delete img;

	Logger::debug("Object Destructor");
}

Object::Object(const Object& data)
{
	throw VillageException("Object Copy Constructor");
}

Object& Object::operator=(const Object* rhs)
{
	throw VillageException("Object Assignment Operator");
}

void Object::update(float time, Uint8* keystrokes)
{

}

void Object::raiseEvent(SDL_Event* event)
{

}

void Object::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	img->draw(xloc - xoffset, yloc - yoffset, screen);
}

bool Object::collides(Object* obj)
{
	return (xloc + width >= obj->xloc && xloc <= obj->xloc + obj->width && yloc + height >= obj->yloc && yloc <= obj->yloc + obj->height);
}

bool Object::collides(MouseImage* obj)
{
	return (xloc + width >= obj->getX() && xloc <= obj->getX() + obj->getWidth() && yloc + height >= obj->getY() && yloc <= obj->getY() + obj->getHeight());
}