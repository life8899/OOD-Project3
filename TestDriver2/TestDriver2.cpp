/////////////////////////////////////////////////////////////////////////////////
// TestDriver2.cpp - TestDriver2 class definition                              //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

// TestDriver1.cpp : Defines a number of tests for the TestedCode1.dll, 
//   Note this testing driver is implicitly linked to the TestedCode.dll.
//
// DLLTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "../TestedCode2/TestedCode2.h"
#include "../Includes/TestHarness.h"
#include "../TestedCode1/TestedCode1.h"
#include "../Includes/TestDriver.h"

#include<iostream>
#include<stdexcept>
#include <Windows.h>

using std::cerr;
//using std::cout;
using std::endl;
using std::logic_error;
using std::string;

typedef void (*TestedCode2Factory) (ITestedCode2**);

void* loadTestedCode(const char* libraryName, const string factoryFnName);

class testTestClass_returnsFalse
{
public:

	testTestClass_returnsFalse(ITestedCode2& testedCode2): _testedCode2(&testedCode2) {}

	bool operator() ()
	{
		// the class method under test
		bool result = (*_testedCode2).returnsFalse();

		// now use the assertion library to test that the method returns false
		assertFalse("testClass.returnsFalse", result);

		return !result;
	}

private:
	ITestedCode2* _testedCode2;
};

class testTestClass_returnsTrue
{
public:

	testTestClass_returnsTrue(ITestedCode2& testedCode2) : _testedCode2(&testedCode2) {}

	bool operator() ()
	{
		// the class method under test
		bool result = (*_testedCode2).returnsTrue();

		// now use the assertion library to test that the method returns true
		assertTrue("testClass.returnsTrue", result);

		return result;
	}

private:
	ITestedCode2 * _testedCode2;
};

class testTestClass_returnsOne
{
public:

	testTestClass_returnsOne(ITestedCode2& testedCode2) : _testedCode2(&testedCode2) {}

	bool operator() ()
	{
		// the class method under test
		int result = (*_testedCode2).returnsOne();

		// now use the assertion library to test that the method returns one
		assertEquals("testClass.returnsEquals", result, 1);

		return 1== result;
	}

private:
	ITestedCode2 * _testedCode2;
};

class testTestClass_returnsTwo
{
public:

	testTestClass_returnsTwo(ITestedCode2& testedCode2) : _testedCode2(&testedCode2) {}

	bool operator() ()
	{
		// the class method under test
		int result = (*_testedCode2).returnsTwo();

		// now use the assertion library to test that the method returns two
		assertEquals("testClass.returnsEquals", result, 2);

		return 2 == result;
	}

private:
	ITestedCode2 * _testedCode2;
};

class testTestClass_throwsException
{
public:

	testTestClass_throwsException(ITestedCode2& testedCode2) : _testedCode2(&testedCode2) {}

	bool operator() ()
	{
		try {
			// the class method under test
			int result = (*_testedCode2).throwsException();
			assertFail("Expecting exception");
			return false;
		}
		catch (std::exception e)
		{
			assertSuccess("Exception Thrown");
			return true;
		}
	}
private:
	ITestedCode2 * _testedCode2;
};

/*
* This class run a sequence of tests that test the proper execution of TestClass2,
*  in a snigle testing suite.
*/
class TestClass1TestingClass : public ITest
{
public:

	void setup(const TestingContext* const ctx, const std::vector<TestedCode>& testedCodes)
	{
		//cout << "setup(const TestingContext&): called" << endl;

		// load the tested code element for testing
		if (testedCodes.size() != 1) {
			// some strange error
			cerr << "TestDriver2 requires a tested code entry" << endl;
			throw logic_error("TestDriver2 requires a tested code entry");
		}

		const TestedCode& testedCode2 = testedCodes[0];

		TestedCode2Factory testedCdFcty = (TestedCode2Factory)loadTestedCode(testedCode2._dll_nm.c_str(), GET_TESTED_CD_2_FN_NM_STR);

		testedCdFcty(&_i_tested_cd_2);

		if(_i_tested_cd_2==nullptr) {
			// some strange error
			cerr << "TestedCode2Factory did not return a tested code 2" << endl;
			throw logic_error("TestedCode2Factory did not return a tested code 2");
		}

		if (ctx != nullptr) tharness.setTestingContext(*ctx);

		int testSuiteId = tharness.createTestSuiteRunner(0, "TestClass testing suite");

		// Setup the test cases 
		tharness.createTestRunner(testSuiteId, "test TestClass.returnsFalse()", testTestClass_returnsFalse(*_i_tested_cd_2));

		tharness.createTestRunner(testSuiteId, "test TestClass.returnsTrue()", testTestClass_returnsTrue(*_i_tested_cd_2));

		tharness.createTestRunner(testSuiteId, "test TestClass.returnsOne()", testTestClass_returnsOne(*_i_tested_cd_2));

		tharness.createTestRunner(testSuiteId, "test TestClass.returnsTwo()", testTestClass_returnsTwo(*_i_tested_cd_2));

		tharness.createTestRunner(testSuiteId, "test TestClass.throwsException()", testTestClass_throwsException(*_i_tested_cd_2));

		// set the reporting level for my tests
		tharness.setShowTimestamp(true);

		tharness.setTestReportingLevel(TestHarness::SHOW_ALL_MESSAGES);
	}

	bool runTests()
	{
		//cout << "runTests(): called" << endl;

		// run the tests
		std::unique_ptr<TestLogger> testLogger = tharness.runTests();

		return testLogger->getStatus();
	}

	void teardown()
	{
		//cout << "teardown(): called" << endl;
	}

private:
	TestHarness tharness;
	ITestedCode2* _i_tested_cd_2;
};

extern "C"
{
	/*
	* User is responsible for deleting ITest object
	*/
	__declspec(dllexport) void _cdecl getTests(ITest** itest)
	{
		*itest = new TestClass1TestingClass();
	}
}

/*
*  Loads the Itest using "Run-Time Dynamic Linking," by loading a dll via LoadLibrary and
*    resolving functions using GetProcAddress.
*
*   Note: The Itest instance must be freed using delete.
*/
void* loadTestedCode(const char* libraryName, const string factoryFnName)
{

	size_t len = strlen(libraryName) + 1;

	// to call win32 load library need a w_char-string (2-byte wide char)
	LPWSTR libName = new wchar_t[len];

	size_t outLen;

	// std library function to do convertion
	mbstowcs_s(&outLen, libName, len, libraryName, strlen(libraryName) + 1);

	// here we actually load the dll we are interested in
	HINSTANCE hDLL = LoadLibrary(libName);

	// remember to free the dynamic string
	delete libName;

	// see if the libary was loaded
	if (NULL != hDLL)
	{

		// Load the factory method from out library
		void* factory = GetProcAddress(hDLL, factoryFnName.c_str());

		// see if we successfully loaded our library
		if (factory != nullptr)
		{
			// success print happy message
			//cout << "Loaded factory method " << factoryFnName << "() from library: " << libraryName << endl;

			// now return the factory method
			return factory;
		}
		else
		{
			// fail print unhappy message
			cerr << "could not load factory method " << factoryFnName << "() from library: " << libraryName << endl;
		}

		// unload library
		FreeLibrary(hDLL);
	}
	else
	{
		cerr << "Load library failed for: " << libraryName << endl;
	}

	return 0;
}