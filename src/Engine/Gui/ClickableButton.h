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

#ifndef CLICKABLEBUTTON_H
#define CLICKABLEBUTTON_H

#include "Button.h"

#include "Engine/Util/Logger.h"

template <class T>
class ClickableButton : public Button
{
public:
	ClickableButton(int x, int y, int width, int height, string normalSrc, string hoverSrc, string downSrc, T *who, void (T::*func)(void)) : callee(who), callback(func), Button(x, y, width, height, normalSrc, hoverSrc, downSrc) { }
	ClickableButton(int x, int y, int width, int height, string normalSrc, string hoverSrc, string downSrc, Uint8 r, Uint8 g, Uint8 b, T *who, void (T::*func)(void)) : callee(who), callback(func), Button(x, y, width, height, normalSrc, hoverSrc, downSrc, r, g, b) { }
private:
	void click() { Logger::debug("Button Clicked"); (callee->*callback)(); }

	T * callee;
	void (T::*callback)(void);
};

#endif