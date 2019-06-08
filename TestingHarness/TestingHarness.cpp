/////////////////////////////////////////////////////////////////////////////////
// TestingHarness.cpp - TestingHarness class definition                        //
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


/*
* default (no arg) constructor, initializes the test harnesss with a Test Logger,
*   and a test suite for the root runner, also going to assign to the root  runner the
*   id of 0.
*/

TestHarness::TestHarness()
	: testLogger(std::make_unique<TestLogger>()), rootRunner(TestSuiteRunner::defaultTestSuiteRunnerFactory())
{

	testSuiteRunnerIndex.push_back(rootRunner.get());
}

// creates a testing harness with a given Logger
TestHarness::TestHarness(const LoggerX& logger)
	: testLogger(std::make_unique<TestLogger>(logger)), rootRunner(TestSuiteRunner::defaultTestSuiteRunnerFactory())
{

	testSuiteRunnerIndex.push_back(rootRunner.get());
}

TestHarness::TestHarness(const TestLogger& testLogger)
	: testLogger(std::make_unique<TestLogger>(testLogger)), rootRunner(TestSuiteRunner::defaultTestSuiteRunnerFactory())
{

	testSuiteRunnerIndex.push_back(rootRunner.get());
}

/*
* Method to run all the tests in the test harness, returns true if all test pass
*/
std::unique_ptr<TestLogger> TestHarness::runTests()
{

	// create a root logger for this test run
	std::unique_ptr<TestLogger> results = this->testLogger->createLogger();

	// Log begin of tests
	results->log("STARTING TESTS\n");

	// run the root runner, which should run all the tests
	bool testResults = rootRunner->run("", *results);

	// Log test results
	results->log("TESTS COMPLETED\n");

	//
	results->setStatus(testResults);

	results->logSummary();

	return results;
}

/*
* This method creates a test suite runner to run a suite of tests.  We can choose to include this
*   test suite in any test suite we create using the test suite Id, by passing it's id in as parentTestSuiteRunnerId.
*   Alternatively add it to the default test suite by passing in a 0.
*/
int TestHarness::createTestSuiteRunner(int parentTestSuiteRunnerId, std::string testSuiteRunnerName)
{
	// get the parent test suite using the index
	TestSuiteRunner* parent = testSuiteRunnerIndex[parentTestSuiteRunnerId];

	if (parent == nullptr)
	{
		string errorMsg("TestHarness::createTestSuiteRunner: Parent test suite not found for id = ");
		errorMsg += parentTestSuiteRunnerId;
		throw new std::out_of_range(errorMsg);
	}

	// create a new test suite with the given name of testSuiteRunnerName
	TestSuiteRunner* newTestSuiteRunner = parent->addTestSuiteRunner(testSuiteRunnerName);

	// now add the new test suite to the index
	testSuiteRunnerIndex.push_back(newTestSuiteRunner);

	// get the id of the test suite we just added
	int newTestSuiteRunnerIndex = testSuiteRunnerIndex.size() - 1;

	return newTestSuiteRunnerIndex;
}

/*
* This method creates a test runner to run a single test.  We can choose to include this
*   test in any test suite we create using the test suite Id, and passing it in as parentTestSuiteRunnerId.
*   Alternatively add it to the default test suite by passing in a 0.
*/
void TestHarness::createTestRunner(int parentTestSuiteRunnerId, std::string testCaseRunnerName, testExecutor testExecutor)
{
	// get the parent test suite using the index
	TestSuiteRunner* parent = testSuiteRunnerIndex[parentTestSuiteRunnerId];

	if (parent == nullptr)
	{
		string errorMsg("TestHarness::createTestSuiteRunner: Parent test suite not found for id = ");
		errorMsg += parentTestSuiteRunnerId;
		throw new std::out_of_range(errorMsg);
	}

	// create a new test case with the given name, and testExecutor
	parent->addTestCaseRunner(testCaseRunnerName, testExecutor);
}

void TestHarness::setTestReportingLevel(int level)
{
	switch (level)
	{
	case TestHarness::PASS_FAIL_ONLY:
		this->testLogger->setShowDetailMessages(false);
		this->testLogger->setShowAssertionDetails(false);
		break;

	case TestHarness::SHOW_DETAIL_MESSAGES:
		this->testLogger->setShowDetailMessages(true);
		this->testLogger->setShowAssertionDetails(false);
		break;

	case TestHarness::SHOW_ALL_MESSAGES:
		this->testLogger->setShowDetailMessages(true);
		this->testLogger->setShowAssertionDetails(true);
		break;

	default:
		break;
	}
}

void TestHarness::setShowTimestamp(bool val)
{
	this->testLogger->setShowTimestamp(val);
}

void TestHarness::attach(std::ostream* out)
{
	this->testLogger->getLogger().attach(out);
}
void TestHarness::flush() const
{
	this->testLogger->getLogger().flush();
}

//---------------------------------------------------------------------------------------//



//---------------------------------------------------------------------------------------//


/*
* logging methods by severity, takes in the context string and a message to log
*/
void TestHarness::log(int severity, const std::string&ctxStr, const std::string& msg) const
{
	this->testLogger->getLogger().log(severity, ctxStr, msg);
}
void TestHarness::trace(const std::string&ctxStr, const std::string& msg) const
{
	this->testLogger->getLogger().trace(ctxStr, msg);
}
void TestHarness::debug(const std::string&ctxStr, const std::string& msg) const
{
	this->testLogger->getLogger().debug(ctxStr, msg);
}
void TestHarness::info(const std::string&ctxStr, const std::string& msg) const
{
	this->testLogger->getLogger().info(ctxStr, msg);
}
void TestHarness::warn(const std::string&ctxStr, const std::string& msg) const
{
	this->testLogger->getLogger().warn(ctxStr, msg);
}
void TestHarness::error(const std::string&ctxStr, const std::string& msg) const
{
	this->testLogger->getLogger().error(ctxStr, msg);
}

// gets and sets the severity for filtering messages
int  TestHarness::getSeverity(void) const
{
	return this->testLogger->getLogger().getSeverity();
}
void TestHarness::setSeverity(int severity)
{
	this->testLogger->getLogger().setSeverity(severity);
}

// A method to set all of the testing parameters all at once
void TestHarness::setTestingContext(const TestingContext& testingCtx) {

	if (testingCtx.logger != nullptr) {
		this->testLogger->setLogger(*(testingCtx.logger));
	}

	this->setSeverity(testingCtx.severity);

	this->setShowTimestamp(testingCtx.showTimestamp);

	this->setTestReportingLevel(testingCtx.reportingLevel);
}