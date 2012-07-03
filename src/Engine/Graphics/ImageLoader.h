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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>
#include <map>

#include "SDL.h"
#include "SDL_image.h"

using namespace std;

class ImageLoader
{
public:
	static bool init();

	static string loadImage(string path);
	static string loadImage(string path, Uint8 r, Uint8 g, Uint8 b);
	static string loadImage(string path, float scale);
	static string loadImage(string path, Uint8 r, Uint8 g, Uint8 b, float scale);

	static string scaleImage(string src, float scale);

	static SDL_Surface* getImage(string id);
	static void cleanup();

private:
	static map<string, SDL_Surface*> images;
	static bool loaded;
};

#endif