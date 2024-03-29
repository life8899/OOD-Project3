/////////////////////////////////////////////////////////////////////////////////
// TestDriver1.cpp - TestDriver1 class definition                              //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

// TestDriver1.cpp : Defines a number of tests for the TestedCode1.dll, 
//   Note this tesing driver is implicitly linked to the TestedCode.dll.
//
// DLLTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "../Includes/TestHarness.h"
#include "../TestedCode1/TestedCode1.h"
#include "../Includes/TestDriver.h"

#include<iostream>
#include<stdexcept>
#include <Windows.h>

using std::logic_error;
using std::string;

/*
 * Tests that the method returnsFalse() for our testing class TestedCode1 returns false
 */
bool testTestClass_returnsFalse()
{
	// class under test
	TestedCode1 testClass;

	// the class method under test
	bool result = testClass.returnsFalse();

	// now use the assertion library to test that the method returns true
	assertFalse("testClass.returnsFalse", result);

	return !result;
}

/*
 * Tests that the method returnsTrue() for our testing class TestedCode1 returns false
 */
bool testTestClass_returnsTrue()
{
	// class under test
	TestedCode1 testClass;

	// the class method under test
	bool result = testClass.returnsTrue();

	// now use the assertion library to test that the method returns false
	assertTrue("testClass.returnsTrue", result);

	return result;
}

/*
 * Tests that the method returnsOne() for our testing class TestedCode1 returns one
 */
bool testTestClass_returnsOne()
{
	// class under test
	TestedCode1 testClass;

	// the class method under test
	int result = testClass.returnsOne();

	// now use the assertion library to test that the method returns false
	assertEquals("testClass.returnsOne", result, 1);

	return 1 == result;
}

/*
 * Tests that the method returnTwo() for our testing class TestedCode1 returns two
 */
bool testTestClass_returnsTwo()
{
	// class under test
	TestedCode1 testClass;

	// the class method under test
	int result = testClass.returnsTwo();

	// now use the assertion library to test that the method returns false
	assertEquals("testClass.returnsTwo", result, 2);

	return 2 == result;
}

/*
 * Tests that the method throwsException() for our testing class TestedCode1 throws an exception
 */
bool testTestClass_throwsException()
{
	// class under test
	TestedCode1 testClass;

	try
	{
		// the class method under test
		int result = testClass.throwsException();
		assertFail("Expecting exception");
		return false;
	}
	catch (std::exception e) 
	{
		assertSuccess("Exception Thrown");
		return true;
	}
}

/*
 * This class run a sequence of tests that test the proper execution of TestClass1,
 *  in a snigle testing suite.
 */
class TestClass1TestingClass : public ITest 
{
public:

	void setup(const TestingContext* const ctx, const std::vector<TestedCode>& testedCodes) 
	{
		//cout << "setup(const TestingContext&): called" << endl;

		if(ctx!=nullptr) tharness.setTestingContext(*ctx);

		int testSuiteId = tharness.createTestSuiteRunner(0, "TestClass testing suite");

		// Setup the test cases 
		tharness.createTestRunner(testSuiteId, "test TestClass.returnsFalse()", testTestClass_returnsFalse);

		tharness.createTestRunner(testSuiteId, "test TestClass.returnsTrue()", testTestClass_returnsTrue);

		tharness.createTestRunner(testSuiteId, "test TestClass.returnsOne()", testTestClass_returnsOne);

		tharness.createTestRunner(testSuiteId, "test TestClass.returnsTwo()", testTestClass_returnsTwo);

		tharness.createTestRunner(testSuiteId, "test TestClass.throwsException()", testTestClass_throwsException);

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