/////////////////////////////////////////////////////////////////////////////////
// TestLogger.h - TestLogger class definition                                  //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#ifndef TEST_LOGGER_H
#define TEST_LOGGER_H

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#ifdef TESTINGHARNESS_EXPORTS // inside the dll
#define TESTING_HARNESS_API    __declspec(dllexport)
#else // outside dll
#define TESTING_HARNESS_API    __declspec(dllimport)
#endif

/*
 *The definition of the grqnularity levels for reporting and filtering log messages
 */
#define LOGGER_ERROR  0
#define LOGGER_WARN   1
#define LOGGER_INFO   2
#define LOGGER_DEBUG  3
#define LOGGER_TRACE  4

/*
 * A basic Logger class used by the test Logger, allows the user to attach multiple ostreams for output,
 *	and provides for logging messages at different granularities, and filtering them for output.
 *  The granularity levels are defined above:
 */
class TESTING_HARNESS_API LoggerX 
{
public:

	/*
	* simple no arg contructor creates a Logger with no attached ostreams
	*/
	LoggerX(void);

	/*
	* creates a Logger with a single given attached ostream to output to
	*/
	LoggerX(std::ostream*);

	// stream management functions
	/*
	 *  Attaches a given ostream for output.
	 */
	void attach(std::ostream*);
	/*
	*  Flushes all the attached ostreams.
	*/
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

	// gets and sets the severity for filtering messages
	int  getSeverity(void) const;
	void setSeverity(int);

	~LoggerX() = default;
	LoggerX(const LoggerX&) = default;
	LoggerX& operator=(const LoggerX&) = default;
private:
	// private method for informing internal error
	void _logErr(int, const std::string&) const;

	std::vector<std::ostream*> log_streams;
	int severity;
};

/*
*  The TestLogger formats the messages produced by the testing harness, using a logger object for output.
*/
class TESTING_HARNESS_API TestLogger
{
public:
	// Default no arg constructor to create a root logger
	TestLogger();

	// Default no arg constructor to create a root logger
	TestLogger(const LoggerX&);

	// Copy constructor
	TestLogger(const TestLogger&);

	// Factory method to create a test logger for running a test suite
	std::unique_ptr<TestLogger> createLogger();

	// Returns the Pass fail status of the testing suite
	bool getStatus();

	// Set the Pass fail status of the testing suite
	void setStatus(bool);

	// prints a summary of the results for running this test suite
	void logSummary() const;

	// Adds a informational message to the test log
	void log(const std::string&) const;

	// Utility method that creates a child context string from a valid context string, and a child's name
	//   usage string childContext = logger.getChildContext(currentContext, "child name");
	const std::string getChildLoggingContext(const std::string&, const std::string&);

	/*************************** Test Case logging methods **********************************************/

	// Logs the beginning of a new Test Case
	//   usage logTestCaseBegin(contextStr);
	void logTestCaseBegin(const std::string&);

	// Logs an assertion from a test case
	//   usage logTestCaseAssertion(contextStr, "variable/assertion msg",  
	//				boolean result);
	void logTestCaseAssertion(const std::string&, const std::string&, bool);

	// Logs that the current test case completed with a Success return
	//   usage logTestCaseSuccess(contextStr);
	void logTestCaseSuccess(const std::string&);

	// Logs that the current test case completed with a (general) failure return
	//   usage logTestCaseFail(contextStr);
	void logTestCaseFail(const std::string&);

	// Logs that the current test case completed with a (general) failure return
	//   usage logTestCaseFailWithException(contextStr, e);
	void logTestCaseFailWithException(const std::string&, const std::exception&);

	// Logs that the current test case completed with a (general) failure return
	//   usage logTestCaseFailWithException(contextStr, e);
	void logTestCaseFailWithSpecificException(const std::string&, const std::string&, const std::exception&);

	// Logs that the current test case failed with an unknown exception return
	//   usage logTestCaseFailWithUnknownException(contextStr);
	void logTestCaseFailWithUnknownException(const std::string&);

	/***************************** Test Suite Logging methods *************************************************/

	// Logs the beginning of a new Test Suite
	//   usage logTestSuiteBegin(contextStr);
	void logTestSuiteBegin(const std::string&);

	// Logs that the current test suite completed with a success return
	//   usage logTestSuiteSuccess(contextStr);
	void logTestSuiteSuccess(const std::string&);

	// Logs that the current test case completed with a failure return
	//   usage logTestSuiteFail(contextStr);
	void logTestSuiteFail(const std::string&);

	// accessors and mutators for detail levels

	void setShowDetailMessages(bool);

	bool getShowDetailMessages();

	void setShowTimestamp(bool);

	bool getShowTimestamp();

	void setShowAssertionDetails(bool);

	bool getShowAssertionDetails();

	LoggerX& getLogger();
	void setLogger(LoggerX&);

private:
	std::ostream& currentTimeStamp(std::ostream& out) const;

	// holds the results from assertions for the running tests
	bool testResults;

	bool showTimestamp;

	bool showDetailMessages;

	bool showAssertionDetails;

	LoggerX _logger;
};

#endif // TEST_LOGGER_H