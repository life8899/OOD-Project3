// TestedCode2.cpp : Defines the exported functions for the DLL application.
/////////////////////////////////////////////////////////////////////////////////
// TestedCode2.cpp - TestedCode2 DLL application                               //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "TestedCode2.h"


extern "C"
{
	/*
	* User is responsible for deleting ITest object
	*/
	__declspec(dllexport) void _cdecl GET_TESTED_CD_2_FN_NM(ITestedCode2** itest)
	{
		*itest = new TestedCode2();
	}

}
