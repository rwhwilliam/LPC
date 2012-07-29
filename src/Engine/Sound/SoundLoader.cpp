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

#include "SoundLoader.h"

#include <string>
#include <map>

#include "SDL_mixer.h"

#include "Engine/Util/Config.h"
#include "Engine/Util/Logger.h"

using namespace std;

//setup static variables
bool SoundLoader::loaded = false;
map<string, Mix_Chunk*> SoundLoader::soundeffects;
map<string, Mix_Music*> SoundLoader::music;

bool SoundLoader::init()
{
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		loaded = false;
		return false;
	}

	Logger::debug("SoundLoader Initialized");

	loaded = true;

	return true;
}

string SoundLoader::loadSoundEffect(string path)
{
	if(loaded)
	{
		path = Config::getConfig("SoundEffectPath") + path;

		map<string, Mix_Chunk*>::iterator it;

		it = soundeffects.find(path);

		if(it == soundeffects.end())
		{
			Logger::debugFormat("Loading SoundEffect %s", path.c_str());

			soundeffects[path] = Mix_LoadWAV(path.c_str());

		}

		return path;
	}
	else
	{
		return "";
	}
}

string SoundLoader::loadMusic(string path)
{
	if(loaded)
	{
		path = Config::getConfig("MusicPath") + path;

		map<string, Mix_Music*>::iterator it;

		it = music.find(path);

		if(it == music.end())
		{
			Logger::debugFormat("Loading Music %s", path.c_str());

			music[path] = Mix_LoadMUS(path.c_str());
		}

		return path;
	}
	else
	{
		return "";
	}
}

Mix_Chunk* SoundLoader::getSoundEffect(string id)
{
	map<string, Mix_Chunk*>::iterator it;

	it = soundeffects.find(id);

	if(it != soundeffects.end())
	{
		return it->second;
	}

	return NULL;
}

Mix_Music* SoundLoader::getMusic(string id)
{
	map<string, Mix_Music*>::iterator it;

	it = music.find(id);

	if(it != music.end())
	{
		return it->second;
	}

	return NULL;
}

void SoundLoader::cleanup()
{
	//map<string, Mix_Chunk*>::iterator it;

	//for(it = soundeffects.begin(); it != soundeffects.end(); ++it)
	//	Mix_FreeChunk(it->second);

	//map<string, Mix_Music*>::iterator it2;
	//
	//for(it2 = music.begin(); it2 != music.end(); ++it)
	//	Mix_FreeMusic(it2->second);

	soundeffects.clear();

	music.clear();

	Mix_CloseAudio();

	Logger::debug("SoundLoader Cleanup");

	loaded = false;
}