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

#include "Font.h"

#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "FontLoader.h"
#include "Engine/Util/VillageException.h"

using namespace std;

Font::Font(string path, int size, Uint8 r, Uint8 g, Uint8 b)
{
	src = FontLoader::loadFont(path, size);

	color.r = r;
	color.g = g;
	color.b = b;
}

Font::~Font()
{

}

Font::Font(const Font& img)
{
	throw VillageException("Font Copy Constructor");
}

Font& Font::operator=(const Font* rhs)
{
	throw VillageException("Font Assignment Operator");
}

void Font::draw(int x, int y, SDL_Surface* source, SDL_Surface* screen)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, screen, &offset);
}

SDL_Surface* Font::getSurface(string msg)
{
	return TTF_RenderText_Solid(FontLoader::getFont(src), msg.c_str(), color);
}