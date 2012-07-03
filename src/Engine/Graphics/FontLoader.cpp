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

#include "FontLoader.h"

#include <string>
#include <map>

#include "SDL_ttf.h"

#include "Engine/Util/Config.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/Logger.h"

using namespace std;

bool FontLoader::loaded = false;
map<string, TTF_Font*> FontLoader::fonts;

bool FontLoader::init()
{
	if(TTF_Init() == -1)
	{
		loaded = false;
		return false;
	}

	Logger::debug("FontLoader Initialized");

	loaded = true;

	return true;
}

string FontLoader::loadFont(string path, int size)
{
	if(loaded)
	{
		Logger::debugFormat("Loading Font %s at Size %i", path, size);

		string id = Config::getConfig("FontPath") + path + toString(size);

		map<string, TTF_Font*>::iterator it;
		it = fonts.find(id);

		if(it == fonts.end())
		{
			TTF_Font* font = TTF_OpenFont(path.c_str(), size);

			if(font != NULL)
			{
				fonts[id] = font;
			}
			else
			{
				return "";
			}
		}

		return id;
	}

	return "";
}

TTF_Font* FontLoader::getFont(string id)
{
	map<string, TTF_Font*>::iterator it;

	it = fonts.find(id);

	if(it != fonts.end())
	{
		return it->second;
	}

	return NULL;
}

void FontLoader::cleanup()
{
	map<string, TTF_Font*>::iterator it;

	for(it = fonts.begin(); it != fonts.end(); ++it)
		TTF_CloseFont(it->second);

	fonts.clear();

	TTF_Quit();

	Logger::debug("FontLoader Cleanup");

	loaded = false;
}