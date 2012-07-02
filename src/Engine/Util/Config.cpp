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

#include "Config.h"

#include <string>
#include <map>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

bool Config::loaded = false;
map<string, string> Config::config;

string Config::getConfig(string key)
{
	if(!loaded)
		Config::load();

	map<string, string>::iterator it;
	it = config.find(key);

	if(it != config.end())
	{
		return it->second;
	}

	return "";
}

void Config::load()
{
	config.clear();

	XMLDocument doc;
	if(!doc.LoadFile("data/config/config.xml"))
	{
		printf("Config File Load Failed");
	}

	for(const XMLNode* node = doc.FirstChildElement("Configuration")->FirstChildElement(); node; node=node->NextSibling())
	{
		string name = node->FirstChildElement("Name")->GetText();
		string value = node->FirstChildElement("Value")->GetText();

		map<string, string>::iterator it;
		it = config.find(name);

		//don't let you over-ride a value already in the file...this could be a bad idea
		//might fix if I have time, rather have a polished game experience than a super secure
		//game :)
		if(it == config.end())
		{
			config[name] = value;
		}
	}

	loaded = true;
}