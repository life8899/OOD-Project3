/////////////////////////////////////////////////////////////////////////////////
// FileManager.h - FileManager class definition                                //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "TestDriver.h"
#include <string>

#ifdef TESTINGHARNESS_EXPORTS // inside the dll
#define TESTING_HARNESS_API    __declspec(dllexport)
#else // outside dll
#define TESTING_HARNESS_API    __declspec(dllimport)
#endif

class TESTING_HARNESS_API FileManager
{
public:
	static Tests readTestFile(const std::string& filename);
};

#endif // !FILE_MANAGER_H
