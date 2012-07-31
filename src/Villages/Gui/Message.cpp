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

#include "Message.h"

#include <string>

#include "SDL.h"

#include "Engine/Graphics/Font.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"

Message::Message(Font* font, string msg, int time) : msg(msg), time(time)
{
	Logger::debug("Message Constructor");

	line = font->getSurface(msg);
}

Message::~Message()
{
	SDL_FreeSurface(line);

	Logger::debug("Message Destructor");
}

Message::Message(const Message& data)
{
	throw VillageException("Message Copy Constructor");
}

Message& Message::operator=(const Message* rhs)
{
	throw VillageException("Message Assignment Operator");
}