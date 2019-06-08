/////////////////////////////////////////////////////////////////////////////////
// TestLogger.cpp - TestLogger class definition                                //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <ctime>
#include "../Includes/TestHarness.h"

#define ROOT_CTX_STR ""

using std::string;

std::string asString (const std::chrono::system_clock::time_point& tp)
{

	// convert to system time:
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	char str[26];
	ctime_s(str, sizeof str, &t);
	std::string ts = str;// convert to calendar time
	ts.resize(ts.size() - 1);         // skip trailing newline
	return ts;
}

std::ostream& TestLogger::currentTimeStamp(std::ostream& out) const
{
	if (showTimestamp)
	{
		std::time_t tp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		out << asString(std::chrono::system_clock::now()) << ":  ";
	}

	return out;
}

static string trim(const string& ctxStr)
{
	return ctxStr;
}

static string getRunnerName(const string& ctxStr)
{
	return ctxStr;
}

// Default no argument constructor to create a root logger
TestLogger::TestLogger()
	: testResults(true), showTimestamp(true), 
	  showDetailMessages(true), showAssertionDetails(true),
	  _logger(nullptr) 
{

	this->_logger = LoggerX(&std::cout);
}

// Default no arg constructor to create a root logger
TestLogger::TestLogger(const LoggerX& logger)
	: testResults(true), showTimestamp(true),
	showDetailMessages(true), showAssertionDetails(true),
	_logger(logger) {}

// Copy Constructor
TestLogger::TestLogger(const TestLogger& orig)
	: testResults(orig.testResults), showTimestamp(orig.showTimestamp),
	showDetailMessages(orig.showDetailMessages), showAssertionDetails(orig.showAssertionDetails),
	_logger(orig._logger) {}

// Factory method to create a test logger for running a test suite
std::unique_ptr<TestLogger> TestLogger::createLogger()
{
	return std::make_unique<TestLogger>(*this);
}

// Returns the Pass fail status of the testing suite
bool TestLogger::getStatus()
{

	return this->testResults;
}

// Set the Pass fail status of the testing suite
void TestLogger::setStatus(bool testResults)
{

	this->testResults = testResults;
}

// prints a summary of the results for running this test suite
void TestLogger::logSummary() const
{
	std::stringstream s;
	currentTimeStamp(s) << "TEST RESULT SUMMARY: ";

	if (testResults)
	{
		s << "All tests passed successfully.";
		_logger.info(ROOT_CTX_STR, s.str()); // logging to root context ""
	}
	else
	{
		s << "The tests had failures.";
		_logger.error(ROOT_CTX_STR, s.str());
	}
}

// Adds a informational message to the test log
void TestLogger::log(const std::string& msg) const
{
	std::stringstream s;
	currentTimeStamp(s) << msg;

	_logger.info(ROOT_CTX_STR, s.str());
}

// Utility method that creates a child context string from a valid context string, and a child's name
//   usage string childContext = logger.getChildContext(currentContext, "child name");
const std::string TestLogger::getChildLoggingContext(const std::string& originalCtx, const std::string& childName)
{
	if (trim(originalCtx).size() > 0)
	{
		return(trim(originalCtx) + "." + trim(childName));
	}
	else
	{
		return trim(childName);
	}
}

/*************************** Test Case logging methods **********************************************/

// Logs the beginning of a new Test Case
//   usage logTestCaseBegin(contextStr);
void TestLogger::logTestCaseBegin(const std::string& ctxStr)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Starting test case: " << getRunnerName(ctxStr);

		_logger.info(ctxStr, s.str());
	}
}

// Logs an assertion from a test case
//   usage logTestCaseAssertion(contextStr, "variable/assertion name", 
//				"string to represent expected value", "string to represent actual value", 
//				boolean result);
void TestLogger::logTestCaseAssertion(const std::string&ctxStr, const std::string& assertMsg, bool assertRslt)
{
	if (showAssertionDetails)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Case " << getRunnerName(ctxStr) << ": " << assertMsg;

		if (assertRslt) {
			// log as info
			_logger.info(ctxStr, s.str());
		}
		else {
			// log as error
			_logger.error(ctxStr, s.str());

			//set status to false ??
			this->testResults = false;
		}
	}
}

// Logs that the current test case completed with a Success return
//   usage logTestCaseSuccess(contextStr);
void TestLogger::logTestCaseSuccess(const std::string& ctxStr)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Case " << getRunnerName(ctxStr) << " finished sucessfully";
		_logger.info(ctxStr, s.str());
	}
}

// Logs that the current test case completed with a (general) failure return
//   usage logTestCaseFail(contextStr);
void TestLogger::logTestCaseFail(const std::string& ctxStr)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Case " << getRunnerName(ctxStr) << " finished with errors";
		_logger.error(ctxStr, s.str());

		//set status to false ??
		this->testResults = false;
	}
}

// Logs that the current test case completed with a (general) failure return
//   usage logTestCaseFailWithException(contextStr, e);
void TestLogger::logTestCaseFailWithException(const std::string& ctxStr, const std::exception& e)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Case " << getRunnerName(ctxStr) << " failed with exception: message =\"" 
			<< e.what() << "\"";
		_logger.error(ctxStr, s.str());

		//set status to false ??
		this->testResults = false;
	}
}

// Logs that the current test case completed with a (general) failure return
//   usage logTestCaseFailWithException(contextStr, e);
void TestLogger::logTestCaseFailWithSpecificException(const std::string& ctxStr,
	const std::string& exceptioName, const std::exception& e)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Case " << getRunnerName(ctxStr) << " failed with " << exceptioName << ": message =\"" << e.what()
			<< "\"";
		_logger.error(ctxStr, s.str());

		//set status to false ??
		this->testResults = false;
	}
}

// Logs that the current test case failed with an unknown exception return
//   usage logTestCaseFailWithUnknownException(contextStr);
void TestLogger::logTestCaseFailWithUnknownException(const std::string& ctxStr)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Case " << getRunnerName(ctxStr) << " failed with unknown exceptions";
		_logger.error(ctxStr, s.str());

		//set status to false ??
		this->testResults = false;
	}
}

/***************************** Test Suite Logging methods *************************************************/

// Logs the beginning of a new Test Suite
//   usage logTestSuiteBegin(contextStr);
void TestLogger::logTestSuiteBegin(const std::string& ctxStr)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Starting test suite: " << getRunnerName(ctxStr);
		_logger.info(ctxStr, s.str());
	}
}

// Logs that the current test suite completed with a success return
//   usage logTestSuiteSuccess(contextStr);
void TestLogger::logTestSuiteSuccess(const std::string& ctxStr)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Suite " << getRunnerName(ctxStr) << " finished sucessfully";
		_logger.info(ctxStr, s.str());
	}
}

// Logs that the current test case completed with a failure return
//   usage logTestSuiteFail(contextStr);
void TestLogger::logTestSuiteFail(const std::string& ctxStr)
{
	if (showDetailMessages)
	{
		std::stringstream s;
		currentTimeStamp(s) << "Test Suite " << getRunnerName(ctxStr) << " finished with errors";
		_logger.error(ctxStr, s.str());

		//set status to false ??
		this->testResults = false;
	}
}

// accessors and mutators for detail levels
void TestLogger::setShowDetailMessages(bool val)
{
	showDetailMessages = val;
}
bool TestLogger::getShowDetailMessages()
{
	return showDetailMessages;
}

void TestLogger::setShowTimestamp(bool val)
{
	showTimestamp = val;
}
bool TestLogger::getShowTimestamp()
{
	return showTimestamp;
}

void TestLogger::setShowAssertionDetails(bool val)
{
	showAssertionDetails = val;
}
bool TestLogger::getShowAssertionDetails()
{
	return showAssertionDetails;
}

LoggerX& TestLogger::getLogger() {
	return this->_logger;
}
void TestLogger::setLogger(LoggerX& logger) {
	this->_logger = LoggerX(logger);
}
