/////////////////////////////////////////////////////////////////////////////////
// TestedCode2.h - TestedCode class definition                                 //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#ifndef TESTED_CODE2_H
#define TESTED_CODE2_H

#include <exception>

#ifdef TESTEDCODE2_EXPORTS // inside the dll
#define TESTED_CODE2_API    __declspec(dllexport)
#else // outside dll
#define TESTED_CODE2_API
#endif

#define GET_TESTED_CD_2_FN_NM      getTestedCode2
#define GET_TESTED_CD_2_FN_NM_STR "getTestedCode2"

typedef struct TESTED_CODE2_API _itested_code_2
{
public:
	virtual bool returnsFalse() = 0;

	virtual bool returnsTrue() = 0;

	virtual int returnsOne() = 0;

	virtual int returnsTwo() = 0;

	virtual int throwsException() = 0;
} ITestedCode2;


class TESTED_CODE2_API TestedCode2: public ITestedCode2
{
public:
	bool returnsFalse()
	{
		return false;
	}

	bool returnsTrue()
	{
		return true;
	}

	int returnsOne()
	{
		return 1;
	}

	int returnsTwo()
	{
		return 2;
	}

	int throwsException()
	{
		throw std::exception("Throws Exception");
	}
};

#endif //TESTED_CODE2_H