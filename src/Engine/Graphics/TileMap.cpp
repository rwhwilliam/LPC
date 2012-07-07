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

#include "TileMap.h"

#include <map>
#include <vector>
#include <exception>

#include "SDL.h"

#include "Image.h"
#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

TileMap::TileMap(int width, int height, int tileWidth, int tileHeight, int layerCount) : width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight), layerCount(layerCount)
{
	Logger::debugFormat("TileMap Constructed.  Width: %i, Height: %i, TileWidth: %i, TileHeight: %i, Layers: %i", width, height, tileWidth, tileHeight, layerCount);

	lastLayerLoaded = -1;
}

TileMap::~TileMap()
{
	data.clear();

	map<int, Image*>::iterator it;

	for(it = tiles.begin(); it != tiles.end(); ++it)
		delete it->second;

	tiles.clear();

	Logger::debug("TileMap Deleted");
}

TileMap::TileMap(const TileMap& img)
{
	throw VillageException("TileMap Copy Constructor");
}

TileMap& TileMap::operator=(const TileMap* rhs)
{
	throw VillageException("TileMap Assignment Operator");
}

void TileMap::addLayer(int id, int dat[])
{
	if(id < layerCount)
	{
		//requires you to load the base layer and on up, not out of order
		if(id == lastLayerLoaded + 1)
		{
			for(int i = 0; i < width * height; i++)
			{
				data[id].push_back(dat[i]);
			}

			lastLayerLoaded = id;

			Logger::debugFormat("TileMap: Layer %i Loaded", id);
		}
		else
		{
			Logger::debugFormat("TileMap: Tried to load previous layer %i, Last Loaded Layer: %i", id, lastLayerLoaded);
		}
	}
	else
	{
		Logger::debug("TileMap: Tried to load a layer higher than there is capacity for");
	}
}

void TileMap::addTile(int id, Image* img)
{
	map<int, Image*>::iterator it;

	it = tiles.find(id);

	if(it != tiles.end())
	{
		//log here that we just possibly changed this tile
		Logger::warnFormat("TileMap: Overwritten tile %i", id);
	}

	Logger::debugFormat("TileMap: Tile id %i set", id);

	tiles[id] = img;
}

void TileMap::update(float time, Uint8* keystates)
{
	//nothing yet
}

void TileMap::draw(int xoffset, int yoffset, SDL_Surface* screen)
{
	for(int k = 0; k <= lastLayerLoaded; k++)
	{
		for(int j = yoffset / tileHeight; j <= (yoffset + atoi(Config::getConfig("ScreenHeight").c_str())) / tileHeight; j++)
		{
			for(int i = xoffset / tileWidth; i <= (xoffset + atoi(Config::getConfig("ScreenWidth").c_str())) / tileWidth; i++)
			{
				if(i >= width || j >= height)
					continue;

				//is this a valid index?
				if(data[k].size() > i + j * height)
				{
					//is it greater than 0?  0 = no tile (for layers above base)
					//we just roll with it for the base layer as well...
					if(data[k][i + j * height] > 0)
					{
						//to speed up things, I'm going to assume the tiles needed are there
						tiles[data[k][i + j * height]]->draw(i * tileWidth - xoffset, j * tileHeight - yoffset, screen);
					}
				}
			}
		}
	}
}