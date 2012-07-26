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

#include "NumericTextbox.h"

#include <string>

#include "Engine/Graphics/Font.h"
#include "Engine/Gui/Textbox.h"

using namespace std;

void NumericTextbox::raiseEvent(SDL_Event e)
{
	if( e.type == SDL_KEYDOWN )
    {
        //Keep a copy of the current version of the string
        std::string temp = contents;
        
        //If the string less than maximum size
        if( contents.length() <= 16 )
        {
			if( ( e.key.keysym.unicode >= (Uint16)'0' ) && ( e.key.keysym.unicode <= (Uint16)'9' ) )
            {
                //Append the character
                contents += (char)e.key.keysym.unicode;
            }
        }

		//If backspace was pressed and the string isn't blank
        if( ( e.key.keysym.sym == SDLK_BACKSPACE ) && ( contents.length() != 0 ) )
        {
            //Remove a character from the end
            contents.erase( contents.length() - 1 );
        }

		//If the string was changed
        if( contents != temp )
        {
            //Free the old surface
            SDL_FreeSurface( textImage );
        
            //Render a new text surface
			textImage = font->getSurface(contents);
        }
	}
}