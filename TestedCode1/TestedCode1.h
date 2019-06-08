/////////////////////////////////////////////////////////////////////////////////
// TestedCode1.h - TestedCode1 class definition                                //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#ifndef TESTED_CODE1_H
#define TESTED_CODE1_H

#include <exception>

#ifdef TESTEDCODE1_EXPORTS // inside the dll
#define TESTED_CODE1_API    __declspec(dllexport)
#else // outside dll
#define TESTED_CODE1_API    __declspec(dllimport)
#endif


class TESTED_CODE1_API TestedCode1
{
public:
	static bool returnsFalse() 
	{ 
		return false; 
	}

	static bool returnsTrue() 
	{ 
		return true;
	}

	static int returnsOne()
	{
		return 1; 
	}

	static int returnsTwo() 
	{ return 2; 
	}

	static int throwsException() 
	{ 
		throw std::exception("Throws Exception");
	}
};
#endif //TESTED_CODE1_H