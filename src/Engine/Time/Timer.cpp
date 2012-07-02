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

#include "Timer.h"

#include <exception>

#include "SDL.h"

#include "Engine/Util/Logger.h"

using namespace std;

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

Timer::~Timer()
{

}

Timer::Timer(const Timer& ship)
{
	throw exception("Timer Copy Constructor");
}

Timer& Timer::operator=(const Timer* rhs)
{
	throw exception("Timer Assignment Operator");
}

void Timer::start()
{
	started = true;

	paused = false;

	startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	started = false;

	paused = false;
}

int Timer::get_ticks() const
{
	if(started == true)
	{
		if(paused == true)
		{
			return pausedTicks;
		}
		else
		{
			return SDL_GetTicks() - startTicks;
		}
	}

	return 0;
}

void Timer::pause()
{
	if((started == true) && (paused == false))
	{
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()
{
	if(paused == true)
	{
		paused = false;

		startTicks = SDL_GetTicks() - pausedTicks;

		pausedTicks = 0;
	}
}

bool Timer::is_started() const
{
	return started;
}

bool Timer::is_paused() const
{
	return paused;
}
