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

#ifndef SOUNDLOADER_H
#define SOUNDLOADER_H

#include <string>
#include <map>

#include "SDL_mixer.h"

using namespace std;

class SoundLoader
{
public:
	static bool init();
	static string loadSoundEffect(string path);
	static string loadMusic(string path);
	static Mix_Chunk* getSoundEffect(string id);
	static Mix_Music* getMusic(string id);
	static void cleanup();

private:
	static map<string, Mix_Chunk*> soundeffects;
	static map<string, Mix_Music*> music;
	static bool loaded;
};

#endif