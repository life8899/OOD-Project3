/////////////////////////////////////////////////////////////////////////////////
// TestHarness.h - TestHarness class definition                                //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* The TestHarness package provides the Harness object which handles the
* execution of test functions in a safe environment. The Harness object
* also creates a Log object that will print messages to the log.
*
* Maintenance History:
* --------------------
* Version 1.0:
*   Initial version. Harness object executes functions in a try-catch block.
*
* Version 2.0:
* 
*/

#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#include "Assertion.h"
//#include "ITest.h"

#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "..\includes\Logger.h"
#include "..\includes\Executive.h"
#include "..\includes\ITest.h"
#include "..\includes\TestDriver.h"


#ifdef TESTINGHARNESS_EXPORTS // inside the dll
#define TESTING_HARNESS_API    __declspec(dllexport)
#define TESTING_HARNESS_API_TEMPLATE
#else // outside dll
#define TESTING_HARNESS_API    __declspec(dllimport)
#define TESTING_HARNESS_API_TEMPLATE extern
#endif

/*
* This struct provides hold the values used to configure the testing harness
*	logger          - an optional logger to use for the testing output,
Loggers can output to multiple ostreams, and acept logging messages
at multiple severity levels.
*	severity        - controls the logging reporting level. values are in increasing order:
LOGGER_ERROR, LOGGER_WARN, LOGGER_INFO, LOGGER_DEBUG, LOGGER_TRACE
*  reportingLevel  - controls the reporting level for the testing harness,
*	showTimestamp   - boolean flag to set whether to add an optional timestamp to logging messages.
*/
typedef struct _testingContext 
{
	LoggerX* logger;
	bool showTimestamp;
	int reportingLevel;
	int severity;
} TestingContext;

/*
* Typedef for our test executors.  Which are function pointers, to a function
*   which returns a bool (true/false) value and takes no arguments.
*/
//typedef bool (*testExecutor)(void);
typedef TESTING_HARNESS_API std::function<bool(void)> testExecutor;






//--------------------------------------------------------------------------//

namespace TestHarness
{
	// Object that contains all of the information to execute our function under test
	class Harness {
	public:
		bool execute();
		void parseTestXML(std::string path);
		void addTest(Executive::TestedCode t);

		// Constructors
		Harness();
		~Harness();

	private:
		Logging::Logger& _log = Logging::StaticLogger<1>::instance();
		Executive::ITest* _testDriver;
	};
}

//--------------------------------------------------------------------------//

/*
* A data structure to hold the metadata and executor for a single test case.
*/

// TestRunner base class definition
class TESTING_HARNESS_API TestRunner
{
public:
	TestRunner(const char* name) : runnerName(name) {};

	TestRunner(const std::string& name) : runnerName(name) {};

	virtual bool run(const std::string& runContext, TestLogger& testLogger) const = 0;

	std::string getRunnerName() const 
	{ 
		return this->runnerName;
	};

private:
	TestRunner(const TestRunner&) = delete;

	TestRunner& operator=(const TestRunner&) = delete;

	std::string runnerName;
};

// TestSuiteRunner class definition
class TESTING_HARNESS_API TestSuiteRunner : public TestRunner
{
public:
	TestSuiteRunner(const char* name);

	TestSuiteRunner(const std::string& name);

	bool run(const std::string& runContext, TestLogger& testLogger) const;

	TestSuiteRunner* addTestSuiteRunner(std::string name);

	void addTestCaseRunner(std::string, testExecutor);

	static std::unique_ptr<TestSuiteRunner> defaultTestSuiteRunnerFactory();

private:
	TestSuiteRunner(const TestSuiteRunner&) = delete;

	TestSuiteRunner& operator=(const TestSuiteRunner&) = delete;

	std::vector<std::unique_ptr<TestRunner>> testRunnerList;
};

// TestCaseRunner class definition
class TESTING_HARNESS_API TestCaseRunner : public TestRunner
{
public:
	TestCaseRunner(const char* name, testExecutor testExecutor)
		: testExecutor(testExecutor), TestRunner((const char*)name) {};

	TestCaseRunner(const std::string& name, testExecutor testExecutor)
		: testExecutor(testExecutor), TestRunner((const std::string&) name) {};

	bool run(const std::string& runContext, TestLogger& testLogger) const;

private:
	TestCaseRunner(const TestCaseRunner&) = delete;
	TestCaseRunner& operator=(const TestCaseRunner&) = delete;

	testExecutor testExecutor;
};

// TestHarness class definition
class TESTING_HARNESS_API TestHarness
{
public:

	// Reporting Level Constants
	static const int PASS_FAIL_ONLY = 0;

	static const int SHOW_DETAIL_MESSAGES = 1;

	static const int SHOW_ALL_MESSAGES = 2;

	// default (no arg) constructor
	TestHarness();

	// creates a testing harness with a given Logger
	TestHarness(const LoggerX&);

	// creates a testing harness with a given TestLogger
	TestHarness(const TestLogger&);

	// no copy constructor
	TestHarness(const TestHarness&) = delete;

	// no copy assignment
	TestHarness& operator=(const TestHarness&) = delete;

	/*
	* Method to run all the tests in the test harness, returns true if all test pass
	*/
	std::unique_ptr<TestLogger> runTests();

	/*
	* This method creates a test suite to run a collection of tests or other test suites.  We can choose to include this
	*   test suit in any test suite we create using the test suite Id, and passing it in as parentTestSuiteRunnerId.
	*   Alternatively add it to the default test suite by passing in a 0.
	*/
	int createTestSuiteRunner(int parentTestSuiteRunnerId, std::string testSuiteRunnerName);

	/*
	* This method creates a test runner to run a single test.  We can choose to include this
	*   test in any test suite we create using the test suite Id, and passing it in as parentTestSuiteRunnerId.
	*   Alternatively add it to the default test suite by passing in a 0.
	*/
	void createTestRunner(int parentTestSuiteRunnerId, std::string testRunnerName, testExecutor testExecutor);

	void setTestReportingLevel(int level);

	void setShowTimestamp(bool val);

	void attach(std::ostream*);
	void flush() const;


	/*
	* logging methods by severity, takes in the context string and a message to log
	*/
	void log(int severity, const std::string&ctxStr, const std::string& msg) const;
	void trace(const std::string&ctxStr, const std::string& msg) const;
	void debug(const std::string&ctxStr, const std::string& msg) const;
	void info(const std::string&ctxStr, const std::string& msg) const;
	void warn(const std::string&ctxStr, const std::string& msg) const;
	void error(const std::string&ctxStr, const std::string& msg) const;

	// A method to set all of the testing parameters all at once
	void setTestingContext(const TestingContext&);

	// gets and sets the severity for filtering messages
	int  getSeverity(void) const;
	void setSeverity(int);

private:
	std::unique_ptr<TestLogger> testLogger;

	std::vector<TestSuiteRunner*> testSuiteRunnerIndex;

	std::unique_ptr<TestSuiteRunner> rootRunner;
};

/*
* Template instantiations for exports
*/
//TESTING_HARNESS_API_TEMPLATE template<> TESTING_HARNESS_API bool std::function<bool(void)>;
//TESTING_HARNESS_API_TEMPLATE template class TESTING_HARNESS_API std::unique_ptr<TestRunner>;
//TESTING_HARNESS_API_TEMPLATE template class TESTING_HARNESS_API std::unique_ptr<TestSuiteRunner>;
//TESTING_HARNESS_API_TEMPLATE template class TESTING_HARNESS_API std::vector<std::unique_ptr<TestRunner>>;

#endif // TESTHARNESS_H