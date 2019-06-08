/////////////////////////////////////////////////////////////////////////////////
// Assertion.h - Assertion class definition                                    //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ASSERTION_H
#define ASSERTION_H

#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include "TestLogger.h"

#ifdef TESTINGHARNESS_EXPORTS // inside the dll
#define TESTING_HARNESS_API    __declspec(dllexport)
#else // outside dll
#define TESTING_HARNESS_API    __declspec(dllimport)
#endif

/*
 * Definition of Assertion methods, to be used inside of test cases for detailed logging of expected tesing conditions 
 */

/*
 * Assert equals asserts that the expected value is equal to the observed value, for the codition being tested.  If both values are equal
 *	the assertion will log an info message based on the condition, 
 *	if not equal an error message will be logged, and the test case will be reported as failed.
 *
 * Usage: assertEquals(const std::string& condition, const T1 expectedValue, const T2 actualValue) 
 *	condition     - a string representing the condition being tested
 *	expectedValue - the value that is expected for this test
 *	actualValue   - the value that was observed while running this test
 */
template<typename T1, typename T2> void TESTING_HARNESS_API assertEquals(const std::string&, const T1&, const T2&);

/*
* Assert not equals asserts that the expected value is different to the observed value, for the codition being tested.  
*	If the values are not equal the assertion will succeed and log an info message based on the condition,
*	if not equal an error message will be logged, and the test case will be reported as failed.
*
* Usage: assertNotEquals(const std::string& condition, const T1 expectedValue, const T2 actualValue)
*	condition     - a string representing the condition being tested
*	expectedValue - the value that is expected for this test
*	actualValue   - the value that was observed while running this test
*/
template<typename T1, typename T2> void TESTING_HARNESS_API assertNotEquals(const std::string&, const T1&, const T2&);

/*
* Assert less than asserts that the testValue value is less than to the comparison value, for the codition being tested.
*	If the inequality holds then the assertion will pass and log an info message based on the condition,
*	if not equal an error message will be logged, and the test case will be reported as failed.
*
* Usage: assertLessThan(const std::string& condition, const T1 testValue, const T2 comparisonValue)
*	condition     - a string representing the condition being tested
*	testValue - the value that is expected for this test
*	comparisonValue   - the value that was observed while running this test
*/
template<typename T1, typename T2> void TESTING_HARNESS_API assertLessThan(const std::string&, const T1&, const T2&);

/*
* Assert less than asserts that the testValue value is grater than than to the comparison value, for the codition being tested.
*	If the inequality holds then the assertion will pass and log an info message based on the condition,
*	if not equal an error message will be logged, and the test case will be reported as failed.
*
* Usage: assertLessThan(const std::string& condition, const T1 testValue, const T2 comparisonValue)
*	condition     - a string representing the condition being tested
*	testValue - the value that is expected for this test
*	comparisonValue   - the value that was observed while running this test
*/
template<typename T1, typename T2> void TESTING_HARNESS_API assertLessThan(const std::string&, const T1&, const T2&);

/*
* Assert not Null asserts that the expected pointer value is not null, for the condition being tested.
*	If this assertion is true it will log an info message based on the condition,
*	if not equal an error message will be logged, and the test case will be reported as failed.
*
* Usage: assertNotNull(const std::string& condition, const T* const testPointer)
*	condition     - a string representing the condition being tested
*	testPointer   - the pointer value being tested
*/
template<typename T> void TESTING_HARNESS_API assertNotNull(const std::string&, const T* const);

/*
* Assert Null asserts that the expected pointer value is null, for the condition being tested.
*	If this assertion is true it will log an info message based on the condition,
*	if not equal an error message will be logged, and the test case will be reported as failed.
*
* Usage: assertNull(const std::string& condition, const T* const testPointer)
*	condition     - a string representing the condition being tested
*	testPointer   - the pointer value being tested
*/
template<typename T> void TESTING_HARNESS_API assertNull(const std::string&, const T* const);

/*
* Assert false asserts that the expected boolean value is false, for the condition being tested.
*	If this assertion is true it will log an info message based on the condition,
*	if not equal an error message will be logged, and the test case will be reported as failed.
*
* Usage: assertFalse(const std::string& condition, bool value)
*	condition     - a string representing the condition being tested
*	value         - the boolean value being tested
*/
void TESTING_HARNESS_API assertFalse(const std::string&, bool);

/*
* Assert true asserts that the expected boolean value is true, for the condition being tested.
*	If this assertion is true it will log an info message based on the condition,
*	if not equal an error message will be logged, and the test case will be reported as failed.
*
* Usage: assertTrue(const std::string& condition, bool value)
*	condition     - a string representing the condition being tested
*	value         - the boolean value being tested
*/
void TESTING_HARNESS_API assertTrue(const std::string& s, bool val);

/*
* Assert fail asserts condition being tested has failed.
*	Will always log an error message based on the condition,
*	and the test case will in all cases be reported as failed.
*
* Usage: assertFail(const std::string& condition)
*	condition     - a string representing the condition being tested
*/
void TESTING_HARNESS_API assertFail(const std::string&);

/*
* Assert success asserts condition being tested has succeeded.
*	Will always log an info message based on the condition.
*
* Usage: assertSuccess(const std::string& condition)
*	condition     - a string representing the condition being tested
*/
void TESTING_HARNESS_API assertSuccess(const std::string&);

/*
 * The AssertionManager is a Singleton class that provided the functionality required for the assertion methods.
 *	This is a internal utility class that is not meant to be publicly accessed or modified.
 */
class TESTING_HARNESS_API AssertionManager
{
public:
	static AssertionManager& getInstance();
	~AssertionManager();

	template<typename T1, typename T2> friend void assertEquals(const std::string& msg, const T1& t1, const T2& t2);

	template<typename T> friend void assertNotNull(const std::string& s, const T* const tPtr);

	friend void assertTrue(const std::string&, bool);
	friend void assertFalse(const std::string&, bool);
	friend void assertFail(const std::string&);
	friend void assertSuccess(const std::string&);

	void pushCntx(const std::string&, TestLogger*);
	bool popCntx();

private:
	AssertionManager();

	bool assertCntx(const std::string& msg, TestLogger* logger);

	void logSuccess(const std::string&);

	void logFail(const std::string&);

	static AssertionManager *instance;

	std::stack<std::pair<std::pair<std::string, bool>, TestLogger*>> ctxStack;
};

// see declarations above
template<typename T1, typename T2>
void assertEquals(const std::string& msg, const T1& t1, const T2& t2)
{

	AssertionManager& assertM = AssertionManager::getInstance();

	if (t2 == t1) 
	{
		std::stringstream s;
		s << "assertEquals passed: " << msg << " received " << t1 << " while expecting " << t2;
		assertM.logSuccess(s.str());
	}
	else 
	{
		std::stringstream s;
		s << "assertEquals failed: " << msg << " received " << t1 << " while expecting " << t2;
		assertM.logFail(s.str());
	}
}

// see declarations above
template<typename T>
void assertNotNull(const std::string& s, const T* const tPtr)
{

	AssertionManager& assertM = AssertionManager::getInstance();

	if (tPtr != nullptr_t)
	{
		std::stringstream s;
		s << "assertNotNull passed for " << msg;
		assertM.logSuccess(s.str());
	}
	else 
	{
		std::stringstream s;
		s << "assertNotNull failed " << msg;
		assertM.logFail(s.str());
	}
}

#endif // ASSERTION_H
