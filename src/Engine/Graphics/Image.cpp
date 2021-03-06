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

#include "Image.h"

#include <string>

#include "SDL.h"
#include "SDL_rotozoom.h"

#include "ImageLoader.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/VillageException.h"

using namespace std;

Image::Image(string src) : alpha(255)
{
	Image::src = ImageLoader::loadImage(src);
}

Image::Image(string src, int alpha) : alpha(255)
{
	Image::src = ImageLoader::loadImage(src);
}

Image::Image(string src, Uint8 r, Uint8 g, Uint8 b) : alpha(255)
{
	Image::src = ImageLoader::loadImage(src, r, g, b);
}

Image::Image(string src, int alpha, Uint8 r, Uint8 g, Uint8 b) : alpha(alpha)
{
	Image::src = ImageLoader::loadImage(src, r, g, b);
}

Image::Image(string src, float scale) : alpha(255)
{
	Image::src = ImageLoader::loadImage(src, scale);
}

Image::Image(string src, int alpha, float scale) : alpha(alpha)
{
	Image::src = ImageLoader::loadImage(src, scale);
}

Image::Image(string src, Uint8 r, Uint8 g, Uint8 b, float scale) : alpha(255)
{
	Image::src = ImageLoader::loadImage(src, r, g, b, scale);
}

Image::Image(string src, int alpha, Uint8 r, Uint8 g, Uint8 b, float scale) : alpha(alpha)
{
	Image::src = ImageLoader::loadImage(src, r, g, b, scale);
}

void Image::draw(int x, int y, SDL_Surface* screen)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	//if(alpha % 256 != 255)
		SDL_SetAlpha(ImageLoader::getImage(src), SDL_SRCALPHA, (alpha % 256));

	SDL_BlitSurface(ImageLoader::getImage(src), NULL, screen, &offset);
}

Image::~Image()
{

}

Image::Image(const Image& img)
{
	throw VillageException("Image Copy Constructor");
}

Image& Image::operator=(const Image* rhs)
{
	throw VillageException("Image Assignment Operator");
}

int Image::getWidth()
{
	return ImageLoader::getImage(src)->w;
}

int Image::getHeight()
{
	return ImageLoader::getImage(src)->h;
}

void Image::setAlpha(int alpha)
{
	Image::alpha = alpha;
}

void Image::setScale(float scale)
{
	Image::src = ImageLoader::scaleImage(Image::src, scale);
}