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

#include "Label.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Font.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

using namespace std;

Label::Label(int x, int y, string fontSrc, string content, int size) : Component(x, y, 0, 0)
{
	Logger::debugFormat("Label created with content: %s", content.c_str());

	font = new Font(fontSrc, size, 255, 255, 255);

	textImage = font->getSurface(content);
}

Label::Label(int x, int y, string fontSrc, string content, int size, Uint8 r, Uint8 g, Uint8 b) : Component(x, y, 0, 0)
{
	Logger::debugFormat("Label created with content: %s", content.c_str());

	font = new Font(fontSrc, size, r, g, b);

	textImage = font->getSurface(content);
}

Label::~Label()
{
	delete font;
	
	SDL_FreeSurface(textImage);

	Logger::debug("Label Destructor");
}

Label::Label(const Label& data)
{
	throw VillageException("Label Copy Constructor");
}

Label& Label::operator=(const Label* rhs)
{
	throw VillageException("Label Assignment Operator");
}

void Label::raiseEvent(SDL_Event* event)
{
	//no events
}

void Label::draw(SDL_Surface* screen)
{
	if(active)
	{
		font->draw(x, y, textImage, screen);
	}
}