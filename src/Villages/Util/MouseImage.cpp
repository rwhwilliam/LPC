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

#include "MouseImage.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Image.h"
#include "Engine/Util/Enums.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/States/SimState.h"

using namespace std;

MouseImage::MouseImage(SimState* state, string goodSrc, string badSrc, int alpha) : x(0), y(0)
{
	Logger::debug("MouseImage Constructor");

	goodImg = new Image(goodSrc, alpha, (Uint8)255, 0, 255, state->getZoomLevel());
	badImg = new Image(badSrc, alpha, (Uint8)255, 0, 255, state->getZoomLevel());

	MouseImage::state = state;

	mode = E_GOOD;

	SDL_GetMouseState(&mapX, &mapY);
}

MouseImage::~MouseImage()
{
	delete goodImg;
	delete badImg;

	state = NULL;

	Logger::debug("MouseImage Destructor");
}

MouseImage::MouseImage(const MouseImage& data)
{
	throw VillageException("MouseImage Copy Constructor");
}

MouseImage& MouseImage::operator=(const MouseImage* rhs)
{
	throw VillageException("MouseImage Assignment Operator");
}

void MouseImage::resize()
{
	goodImg->setScale(state->getZoomLevel());
	badImg->setScale(state->getZoomLevel());
}

void MouseImage::raiseEvent(SDL_Event* event)
{
	if(event->type == SDL_MOUSEMOTION)
	{
		x = (event->motion.x - goodImg->getWidth() / 2) + state->getXOffset();
		y = (event->motion.y - goodImg->getHeight() / 2) + state -> getYOffset();

		x -= x % state->getTileWidth();
		y -= y % state->getTileHeight();

		mapX = x - state->getXOffset();
		mapY = y - state->getYOffset();

		x /= state->getTileWidth();
		y /= state->getTileHeight();

		mode = state->canBuild(mapX, mapY, getWidth(), getHeight());
	}
}

void MouseImage::update(float time, Uint8* keystates)
{
	if(keystates[SDLK_w] || keystates[SDLK_s] || keystates[SDLK_a] || keystates[SDLK_d])
	{
		//the map might have been scrolled
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		int tempX = (mouseX - goodImg->getWidth() / 2) + state->getXOffset();
		int tempY = (mouseY - goodImg->getHeight() / 2) + state->getYOffset();

		tempX -= tempX % state->getTileWidth();
		tempY -= tempY % state->getTileHeight();

		mapX = tempX - state->getXOffset();
		mapY = tempY - state->getYOffset();

		x = tempX / state->getTileWidth();
		y = tempY / state->getTileHeight();

		mode = state->canBuild(mapX, mapY, getWidth(), getHeight());
	}
}

void MouseImage::draw(SDL_Surface* screen)
{
	if(mode == E_GOOD)
		goodImg->draw(mapX, mapY, screen);
	else
		badImg->draw(mapX, mapY, screen);
}