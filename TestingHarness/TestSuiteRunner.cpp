/////////////////////////////////////////////////////////////////////////////////
// TestSuiteRunner.cpp - TestSuiteRunner class definition                      //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include "../Includes/TestHarness.h"

using std::string;

std::unique_ptr<TestSuiteRunner> TestSuiteRunner::defaultTestSuiteRunnerFactory()
{
	return std::make_unique<TestSuiteRunner>("");
}

TestSuiteRunner::TestSuiteRunner(const char* name) : TestRunner((const char*)name) {};

TestSuiteRunner::TestSuiteRunner(const std::string& name) : TestRunner((const std::string&) name) {};

bool TestSuiteRunner::run(const string& context, TestLogger& testLogger) const
{
	// Log starting test suite
	testLogger.logTestSuiteBegin(context);
	AssertionManager::getInstance().pushCntx(context, &testLogger);

	// a variable to hold the cumulative result of the tests in this testing suite
	bool testResult = true;

	// Running a test suite runner, simply means running all if it's test runners
	for (const std::unique_ptr<TestRunner>& runnerPtr : this->testRunnerList)
	{
		const string& runnerName = runnerPtr->getRunnerName();

		const string& ctx = testLogger.getChildLoggingContext(context, runnerName);

		TestRunner& runner = *runnerPtr;

		testResult &= runner.run(ctx, testLogger);
	}

	bool assertResults = AssertionManager::getInstance().popCntx();

	testResult &= assertResults;

	// log completion of the test suite
	if (testResult) 
	{
		testLogger.logTestSuiteSuccess(context);
	}
	else 
	{
		testLogger.logTestSuiteFail(context);
	}

	return testResult;
}

TestSuiteRunner* TestSuiteRunner::addTestSuiteRunner(std::string name)
{

	this->testRunnerList.push_back(std::make_unique<TestSuiteRunner>(name));

	return (TestSuiteRunner*)(testRunnerList.back()).get();
}

void TestSuiteRunner::addTestCaseRunner(std::string name, testExecutor xtor)
{

	this->testRunnerList.push_back(std::make_unique<TestCaseRunner>(name, xtor));
}