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

#ifndef TILEMAP_H
#define TILEMAP_H

#include <map>
#include <vector>

#include "SDL.h"

using namespace std;

class Image;

typedef map<int, vector<int>> layerVector;

class TileMap
{
public:
	TileMap(int width, int height, int tileWidth, int tileHeight, int layerCount);

	TileMap(const TileMap& img);
	TileMap& operator=(const TileMap* rhs);
	~TileMap();

	void addLayer(int id, int dat[]);
	void addTile(int id, Image* img);

	void update(float time, Uint8* keystates);
	void draw(int xoffset, int yoffset, SDL_Surface* screen);

	int getTileWidth() { return tileWidth; };
	int getTileHeight() { return tileHeight; };
private:
	int width, height;
	int tileWidth, tileHeight;
	int layerCount, lastLayerLoaded;
	map<int, Image*> tiles;
	layerVector data;
};

#endif
