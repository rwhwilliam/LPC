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

#include "ImageLoader.h"

#include <string>
#include <map>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_rotozoom.h"

#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/Util.h"
#include "Engine/Util/Tokenizer.h"

using namespace std;

//setup initial values
bool ImageLoader::loaded = false;
map<string, SDL_Surface*> ImageLoader::images;

bool ImageLoader::init()
{
	loaded = true;

	Logger::debug("ImageLoader Initialized");

	return true;
}

string ImageLoader::loadImage(string path)
{
	if(loaded)
	{
		return ImageLoader::loadImage(path, 1);
	}
}

string ImageLoader::loadImage(string path, Uint8 r, Uint8 g, Uint8 b)
{
	if(loaded)
	{
		return ImageLoader::loadImage(path, r, g, b, 1);
	}

	return "";
}

string ImageLoader::loadImage(string path, float scale)
{
	if(loaded)
	{
		path = Config::getConfig("ImagePath") + path + "_" + toString(scale);

		map<string, SDL_Surface*>::iterator it;

		it = images.find(path);

		if(it == images.end())
		{
			Logger::debugFormat("Loading Image %s", path.c_str());

			SDL_Surface* loadedImage = NULL;
			SDL_Surface* optimizedImage = NULL;

			loadedImage = IMG_Load(path.substr(0, path.find_last_of("_")).c_str());

			if(loadedImage != NULL)
			{
				optimizedImage = SDL_DisplayFormat(loadedImage);

				SDL_FreeSurface(loadedImage);

				if(scale != 1)
				{
					SDL_Surface* scaledImage = zoomSurface (optimizedImage, scale, scale, 1);

					SDL_FreeSurface(optimizedImage);

					if(scaledImage != NULL)
					{
						images[path] = scaledImage;
					}
					else
					{
						return "";
					}
				}
				else
				{
					images[path] = optimizedImage;
				}
			}
			else
			{
				return "";
			}
		}

		return path;
	}

	return "";
}

string ImageLoader::loadImage(string path, Uint8 r, Uint8 g, Uint8 b, float scale)
{
	if(loaded)
	{
		string stub = loadImage(path, scale);

		//since we called the other loadImage, there now has to be an entry
		//in the map
		SDL_Surface* image = images[stub];

		Uint32 colorkey = SDL_MapRGB(image->format, r, g, b);
		SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey);

		return stub;
	}

	return "";
}

string ImageLoader::scaleImage(string src, float scale)
{
	string base = src.substr(0, src.find_last_of("_"));

	Logger::debugFormat("Scaling image %s", base.c_str());

	if(ImageLoader::getImage(base + "_" + toString(scale)) != NULL)
		return base + "_" + toString(scale);

	if(ImageLoader::getImage(base + "_" + toString(1)) != NULL)
		images[base + "_" + toString(scale)] = zoomSurface(ImageLoader::getImage(base + "_" + toString(1)), scale, scale, 1);
	else
		return ImageLoader::loadImage(base, scale);

	return base + "_" + toString(scale);
}

SDL_Surface* ImageLoader::getImage(string id)
{
	map<string, SDL_Surface*>::iterator it;

	it = images.find(id);

	if(it != images.end())
	{
		return it->second;
	}

	return NULL;
}

void ImageLoader::cleanup()
{
	//cleanup all the images
	map<string, SDL_Surface*>::iterator it;

	for(it = images.begin(); it != images.end(); ++it)
		SDL_FreeSurface(it->second);

	images.clear();

	Logger::debug("ImageLoader Cleanup");

	loaded = false;
}