/////////////////////////////////////////////////////////////////////////////////
// TestDriver.h - TestDriver class definition                                  //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#ifndef TEST_DRIVER_H
#define TEST_DRIVER_H

#include "TestHarness.h"

#include <string>
#include <vector>

/*
*  The class to read in the TestDriver Elemts into
*/
typedef struct _dll_ref 
{

	std::string _dll_nm;

	_dll_ref(const std::string& dll_nm)
		: _dll_nm(dll_nm) {}
} DLLRef;

/*
*  The class to read in the TestDriver Elemts into
*/
typedef struct _tested_code : public DLLRef 
{

	_tested_code(const std::string& dll_nm)
		: _dll_ref(dll_nm) {}
} TestedCode;

/*
*  The class to read in the TestDriver Elements into
*/
typedef struct _test_driver : public DLLRef 
{
	std::string _test_driver_name;
	std::string _factory_method_nm;
	std::vector<TestedCode> _tested_codes;

	_test_driver(const std::string& name, const std::string& dll_name, const std::string& dll_factory_method)
		: _dll_ref(dll_name), _test_driver_name(name), _factory_method_nm(dll_factory_method) {}
} TestDriver;

typedef struct _test 
{
	std::string _name;
	std::vector<TestDriver> _testdrivers;

	_test(const std::string& name) : _name(name) {}
} Test;

typedef std::vector<Test> Tests;

/*
* The ITest interface that must be implmented for every tested code runner.  The
*   Executor of the tests will first call setup(...), followed by runTests() follwed by teardown()
*/
typedef struct _itest_t 
{
	virtual void setup(const TestingContext* const, const std::vector<TestedCode>&) = 0;

	virtual bool runTests() = 0;

	virtual void teardown() = 0;
} ITest;

typedef void (*TestFactory) (ITest**);

#endif //TEST_DRIVER_H