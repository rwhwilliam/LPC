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

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdarg>
#include <string>

using namespace std;

enum LogLevel {TRACE, DEBUGGING, INFO, WARN, ERROR};

class Logger
{
public:
	static void setLogLevel(LogLevel level) { logwriting = level; }

	static void record(LogLevel level, string message);
	static void recordFormat(LogLevel level, string message, ...);

	static void traceFormat(string message, ...);
	static void trace(string message);

	static void debugFormat(string message, ...);
	static void debug(string message);

	static void infoFormat(string message, ...);
	static void info(string message);

	static void warnFormat(string message, ...);
	static void warn(string message);

	static void errorFormat(string message, ...);
	static void error(string message);
	
private:
	static string getDate();
	static string enumToString(LogLevel level);
	static string format(const char *str, va_list args);

	static LogLevel logwriting;//the level at which this and above we write to file
};

#endif