/////////////////////////////////////////////////////////////////////////////////
// Assertion.cpp - Assertion class definition                                  //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#include "../Includes/Assertion.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

AssertionManager* AssertionManager::instance;

AssertionManager& AssertionManager::getInstance()
{
	if (AssertionManager::instance == nullptr)
	{
		AssertionManager::instance = new AssertionManager();
	}
	return *AssertionManager::instance;
}

AssertionManager::AssertionManager() {}

AssertionManager::~AssertionManager()
{
	if (AssertionManager::instance != nullptr)
	{
		delete AssertionManager::instance;
		AssertionManager::instance = nullptr;
	}
}

void AssertionManager::pushCntx(const std::string& ctxStr, TestLogger* logger)
{
	// add this new context to the stack
	ctxStack.push(std::make_pair(std::make_pair(ctxStr, true), logger));
}

bool AssertionManager::assertCntx(const string& msg, TestLogger* logger)
{
	return true;
}

bool AssertionManager::popCntx()
{
	// grab the current context, and remove from stack
	auto ctxEntry = ctxStack.top();
	ctxStack.pop();

	// return the assertion status
	return ctxEntry.first.second;
}


void AssertionManager::logSuccess(const string& msg)
{
	// grab the current context
	auto ctxEntry = ctxStack.top();

	// get the current context logger and string
	TestLogger* logger = ctxEntry.second;
	string& ctxStr = ctxEntry.first.first;

	// add the assertion result to the logger
	logger->logTestCaseAssertion(ctxStr, msg, true);
}

void AssertionManager::logFail(const string& msg)
{
	// grab the current context
	auto ctxEntry = ctxStack.top();

	// get the current context logger and string
	TestLogger* logger = ctxEntry.second;
	string& ctxStr = ctxEntry.first.first;

	// add the assertion result to the logger
	logger->logTestCaseAssertion(ctxStr, msg, false);

	// make sure to set the assertion status to false for this entry
	ctxEntry.first.second = false;
	//replace top entry
	ctxStack.pop();
	ctxStack.push(ctxEntry);
}

void assertFalse(const std::string& msg, bool val)
{

	AssertionManager& assertM = AssertionManager::getInstance();

	if (!val) {
		std::stringstream s;
		s << "assertFalse passed for " << msg;
		assertM.logSuccess(s.str());
	}
	else {
		std::stringstream s;
		s << "assertFalse failed " << msg;
		assertM.logFail(s.str());
	}
}

void assertTrue(const std::string& msg, bool val)
{

	AssertionManager& assertM = AssertionManager::getInstance();

	if (val) {
		std::stringstream s;
		s << "assertTrue passed for " << msg;
		assertM.logSuccess(s.str());
	}
	else {
		std::stringstream s;
		s << "assertTrue failed " << msg;
		assertM.logFail(s.str());
	}
}

void assertFail(const std::string& msg)
{
	AssertionManager& assertM = AssertionManager::getInstance();

	std::stringstream s;
	s << "assertFail called for " << msg;
	assertM.logFail(s.str());
}

void assertSuccess(const std::string& msg)
{
	AssertionManager& assertM = AssertionManager::getInstance();

	std::stringstream s;
	s << "assertTrue called for " << msg;
	assertM.logSuccess(s.str());
}
