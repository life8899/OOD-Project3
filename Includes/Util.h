/////////////////////////////////////////////////////////////////////////////////
// Util.h - Util class definition                                              //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>

class UTIL {
public:

	static std::string msgEncode(const std::string& inStr) {
		// replace all instances of character '\n' with string "\\n"

		std::stringstream s;

		// loop over all the characters of the original string
		for (char ch : inStr) {

			// if char equals '\n' the write ... to buffer
			if (ch == '\n')
			{
				s << "\\n";
			}
			// else write ... to buffer
			else
			{
				s << ch;
			}

		}

		// return string representation of string stream
		return s.str();
	}

	static std::string msgDecode(const std::string& inStr) {

		std::string outStr(inStr);

		// replace all instances of string "\\n" with character '\n'
		size_t index = 0;

		while (true) {
			// find the first index where my substring occurs
			index = outStr.find("\\n", index);

			// if not found break out of while loop
			if (index == std::string::npos) break;

			// make the replacement
			outStr.replace(index, 3, "\n");
		}

		return outStr;
	}
};

#endif // UTIL_H