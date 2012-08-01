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

#include "MessageBox.h"

#include <string>

#include "Engine/Graphics/Font.h"
#include "Engine/Graphics/Image.h"
#include "Engine/Time/Timer.h"
#include "Engine/Util/Logger.h"
#include "Engine/Util/VillageException.h"
#include "Villages/Gui/Message.h"

MessageBox::MessageBox(int x, int y, int width, int height) : xloc(x), yloc(y), width(width), height(height)
{
	Logger::debug("MessageBox Constructor");

	timer = new Timer();
	timer->start();
	font = new Font("lazy.ttf", 15, 255, 255, 255);
	bg = new Image("messagebox-ui.png", 255, 0, 255);
}

MessageBox::~MessageBox()
{
	delete timer;
	delete font;
	delete bg;

	list<Message*>::iterator it;
	for(it = msgs.begin(); it != msgs.end(); ++it)
		delete *it;

	msgs.clear();

	Logger::debug("MessageBox Destructor");
}

MessageBox::MessageBox(const MessageBox& data)
{
	throw VillageException("MessageBox Copy Constructor");
}

MessageBox& MessageBox::operator=(const MessageBox* rhs)
{
	throw VillageException("MessageBox Assignment Operator");
}

void MessageBox::addMessage(string msg)
{
	Message* m = new Message(font, msg, timer->get_ticks());
	msgs.push_back(m);
}

void MessageBox::update(float time, Uint8* keystrokes)
{
	if(msgs.size() > 0)
	{
		Message* front = msgs.front();
		if(front->getTime() <= timer->get_ticks() - 20000)
		{
			delete front;
			msgs.pop_front();
		}

		if(msgs.size() > 9)
		{
			delete msgs.front();
			msgs.pop_front();
		}
	}
}

void MessageBox::draw(SDL_Surface* screen)
{
	if(msgs.size() > 0)
	{
		bg->draw(xloc, yloc, screen);

		int i = 0;
		list<Message*>::const_iterator it;
		for(it = msgs.begin(); it != msgs.end(); ++it)
			font->draw(xloc + 10, yloc + 25 + 20 * i++, (*it)->getLine(), screen);
	}
}