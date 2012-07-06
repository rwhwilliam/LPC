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

#include "Logger.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <ctime>
#include <cstdarg>

#include "Engine/Util/Config.h"

using namespace std;

//initalize
LogLevel Logger::logwriting = DEBUGGING;

void Logger::record(LogLevel level, string message)
{
	if(level >= logwriting)
	{
		static ofstream fout(Config::getConfig("LogFile").c_str(), fstream::app);

		fout << enumToString(level) << "\t" << getDate() << "\t" << message << endl;
	}
}

//the beauty of va_list has no bounds :)
void Logger::recordFormat(LogLevel level, string message, ...)
{
	const char* cstr = message.c_str();

	va_list args;
	va_start(args, cstr);

	record(level, format(cstr, args));

	va_end(args);
}

void Logger::trace(string message)
{
	record(TRACE, message);
}

void Logger::traceFormat(string message, ...)
{
	const char* cstr = message.c_str();

	va_list args;
	va_start(args, cstr);

	record(TRACE, format(cstr, args));

	va_end(args);
}

void Logger::debug(string message)
{
	record(DEBUGGING, message);
}

void Logger::debugFormat(string message, ...)
{
	const char* cstr = message.c_str();

	va_list args;
	va_start(args, cstr);

	record(DEBUGGING, format(cstr, args));

	va_end(args);
}

void Logger::info(string message)
{
	record(INFO, message);
}

void Logger::infoFormat(string message, ...)
{
	const char* cstr = message.c_str();

	va_list args;
	va_start(args, cstr);

	record(INFO, format(cstr, args));

	va_end(args);
}

void Logger::warn(string message)
{
	record(WARN, message);
}

void Logger::warnFormat(string message, ...)
{
	const char* cstr = message.c_str();

	va_list args;
	va_start(args, cstr);

	record(WARN, format(cstr, args));

	va_end(args);
}

void Logger::error(string message)
{
	record(ERROR, message);
}

void Logger::errorFormat(string message, ...)
{
	const char* cstr = message.c_str();

	va_list args;
	va_start(args, cstr);

	record(ERROR, format(cstr, args));

	va_end(args);
}

string Logger::format(const char *str, va_list args)
{
	ostringstream output;

	for(int i = 0; i < strlen(str); ++i)
	{
		if(str[i] == '%' && str[i+1] != '%' && i+1 < strlen(str))
		{
			switch(str[i+1])
			{
			case 's':
			{
				char *temp = va_arg(args, char*);
				output << temp;
				break;
			}
			case 'i':
			{
				int temp = va_arg(args, int);
				output << temp;
				break;
			}
			case 'd':
			{
				double temp = va_arg(args, double);
				output << temp;
				break;
			}
			case 'f':
			{
				float temp = va_arg(args, float);
				output << temp;
				break;
			}
			default:
				output << str[i];
			}

			i++;
		}
		else
		{
			output << str[i];
		}
	}

	return output.str();
}

string Logger::enumToString(LogLevel level)
{
	switch(level)
	{
	case TRACE:
		return "TRACE";
	case DEBUGGING:
		return "DEBUG";
	case INFO:
		return "INFO";
	case WARN:
		return "WARNING";
	case ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

string Logger::getDate()
{
	time_t t = time(0);
	struct tm * now = localtime(&t);

	ostringstream o;

	o << now->tm_mon << "/" << now->tm_mday << "/" << (now->tm_year + 1900) << " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;
	return o.str();
}
