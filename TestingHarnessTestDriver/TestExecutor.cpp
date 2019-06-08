/////////////////////////////////////////////////////////////////////////////////
// TestExecutor.cpp - TestExecutor class definition                            //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2019                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////


#include "../Includes/Executive.h"
#include "../Includes/FileManager.h"
#include "../Includes/TestDriver.h"
#include "../Includes/TestHarness.h"
#include "../Includes/TestLogger.h"
#include "../Includes/Util.h"


#include "../MsgPassingComm/Comm.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

#include <iostream>
#include <functional>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

using namespace Executive;


using std::cerr;
using std::cout;
using std::endl;
using std::logic_error;
using std::string;

using MsgPassingCommunication::Comm;
using MsgPassingCommunication::Message;
using MsgPassingCommunication::EndPoint;

ITest* loadTestDriver(const char* , const string);
bool runTestDriver(const TestingContext&, const TestDriver&);
void runTestCommand(const Message&, Message&);

static void usage(void);

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

// Constructor
Executor::Executor()
{
	_log.write("Executor object created.\n");
}

// Destructor
Executor::~Executor()
{
}


// Private test function 1 fails
bool testfunc1() {
	return false;
}

// Private test function 2 passes
bool testfunc2() {
	return true;
}

// Private test function 3 throws an exception
bool testfunc3() {
	throw "Exception thrown from testfunc3...\n";
	return true;
}



int main(int argc, char** argv)
{
	char *serverAddr, *clientAddr;
	int serverPort, clientPort;
	char* commNm;

	DEBUG(
		cout << argv[0] << " started with argc = " << argc << "." <<endl;
		cout.flush();
	);

	/* parse cmd line option to get the:
		a) address and server port to connect to
		b) client port to listen on
		c) unique identifier for communication with server
	    Assume this application is invokes as follows:
	    > TestExecutor serverAddr serverPort clientAddr clientPort commNm
	*/
	if (argc != 6) 
	{
		usage();
		return -1;
	}

	serverAddr = argv[1];
	serverPort = atoi(argv[2]);
	clientAddr = argv[3];
	clientPort = atoi(argv[4]);
	commNm = argv[5];

	DEBUG(
		cout << "createTestExecutor called with: "
		<< "serverAddr = " << serverAddr
		<< ", serverPort = " << serverPort
		<< ", clientAddr = " << clientAddr
		<< ", clientPort = " << clientPort
		<< ", commNm = " << commNm
		<< endl;
	);

	StaticLogger<1>::attach(&std::cout);

	SocketSystem ss;
	
	// define our endpoints
	EndPoint serverEP(serverAddr, serverPort);
	EndPoint clientEP(clientAddr, clientPort);

	DEBUG(
		cout << "creating Comm object to start listening for messages on "
		<< clientAddr << ":" << clientPort << "" << endl);

	// create a Comm object to start listening for messages on clientAddr:clientPort
	Comm comm(EndPoint(clientAddr, clientPort), commNm);
	comm.start();
	DEBUG(cout << "now listening on " << clientAddr << ":" << clientPort << "" << endl);

	// send a ready message
	DEBUG(cout << "sending ready message" << endl);
	Message ready(serverEP, clientEP);
	ready.name(commNm);
	ready.command("ready");
	comm.postMessage(ready);
	DEBUG(cout << "ready message sent" << endl);

	// main message loop
	DEBUG(cout << "starting main loop" << endl);
	while (true) 
	{

		// wait for message
		DEBUG(cout << "waiting for next message" << endl);
		Message cmdMsg = comm.getMessage();

		// get the command message
		string cmdNm = cmdMsg.command();

		if (cmdNm == "runTest") 
		{
			cout << "\n\nnew runTest message recieved:" << endl;
			DEBUG(cout << "\tto = " << cmdMsg.to().toString() << endl);
			DEBUG(cout << "\tfrom = " << cmdMsg.from().toString() << endl);
			DEBUG(cout << "\tname = " << cmdMsg.name() << endl);
			DEBUG(cout << "\tcommand = " << cmdMsg.command() << endl);
			DEBUG(cout << "\tid = " << cmdMsg.id() << endl);
			DEBUG(cout << "\treportingLevel = " << cmdMsg.get("reportingLevel") << endl);
			DEBUG(cout << "\tseverity = " << cmdMsg.get("severity") << endl);
			DEBUG(cout << "\tshowTimestamp = " << cmdMsg.get("showTimestamp") << endl);
			DEBUG(cout << "\ttestDriver.name = " << cmdMsg.get("testDriver.name") << endl);
			DEBUG(cout << "\ttestDriver.dllName = " << cmdMsg.get("testDriver.dllName") << endl);
			DEBUG(cout << "\ttestDriver.factoryMethod = " << cmdMsg.get("testDriver.factoryMethod") << endl << endl);

			// process message by running test, or exiting
			Message resultMsg(serverEP, clientEP);
			resultMsg.command("runTestResult");
			resultMsg.reply_to(cmdMsg.id());

			// now we run the test
			DEBUG(cout << "running test" << endl);
			runTestCommand(cmdMsg, resultMsg);
			DEBUG(cout << "finished running test" << endl << endl);

			// send response
			comm.postMessage(resultMsg);
		}
		else if (cmdNm == "stop") 
		{
			// received stop command so stop waiting for new messages and exit
			// send a stopping message acknowledgement
			Message resultMsg(serverEP, clientEP);
			resultMsg.command("stopping");
			resultMsg.reply_to(cmdMsg.id());

			// send response
			comm.postMessage(resultMsg);

			// break out of main loop
			break;
		}
		else 
		{
			//unknown message
		}
	}

	DEBUG(cout << "exited main loop, ending program" << endl);
	DEBUG(cout << "********************************" << endl);

	return 0;
}

void simulateTestmessagesv(void) 
{

	// parsing the xml files into a collection of test entries each with a test driver and one or more tested code enties

	bool cumulativeTestingResult = true;

	// setup the testing context for controlling the testing output

	// tests is the results
	Tests tests = FileManager::readTestFile("testXML.xml");

	//For each test entry in test
	for(Test test: tests)
	{
		// loop over test drivers in each test
		for(TestDriver test_driver : test._testdrivers)
		{
			// Message buffers
			Message cmdMsg, resultMsg;

			// build a simulated message
			//cmdMsg.from();
			//cmdMsg.to();

			// step 1 - put the reporting level and timestamp settings into the command message
			cmdMsg.attribute("reportingLevel", "SHOW_ALL_MESSAGES");
			cmdMsg.attribute("severity", "LOGGER_INFO");
			cmdMsg.attribute("showTimestamp", "true");

			// step 2 - put a simulated TestDriver into the command message
			cmdMsg.attribute("testDriver.name", test_driver._test_driver_name);
			cmdMsg.attribute("testDriver.dllName", test_driver._dll_nm);
			cmdMsg.attribute("testDriver.factoryMethod", test_driver._factory_method_nm);

			// step 2b - put the tested code entries into the simulated TestDriver, in the command message
			int i = 0;
			for (const TestedCode& testedCode : test_driver._tested_codes)
			{
				string s("testDriver.testedCodes.");
				(s += ('0' + i))+=".dllName";
				cmdMsg.attribute(s, testedCode._dll_nm);
			}

			// run the test
			runTestCommand(cmdMsg, resultMsg);

			// extract the result from the result message
			bool result = (resultMsg.get("resultValue")=="true");

			cumulativeTestingResult |= result;

			// extract the result logs from the result message
			string resultLog = resultMsg.get("resultLog");

		cout << resultLog << endl;
		}
	}
}

/*
 * Runs a given test defined in the TestDriver element, using the given TestingCtx
 */
void runTestCommand(const Message& cmdMsg, Message& resultMsg)
{
	// Execute a run Test Command
	std::stringstream resultBuffer;
	// create a logger for logging the testing output
	LoggerX logger;
	logger.attach(&resultBuffer);

	// create a testing contex to bundle the testing settings
	TestingContext testingCtx;
	testingCtx.logger = &logger;

	// get the reporting level from the command message
	string reportingLevelStr = cmdMsg.get("reportingLevel");
	
	if(reportingLevelStr == "SHOW_ALL_MESSAGES")
	{
		testingCtx.reportingLevel = TestHarness::SHOW_ALL_MESSAGES;
	}
	else if (reportingLevelStr == "SHOW_DETAIL_MESSAGES") 
	{
		testingCtx.reportingLevel = TestHarness::SHOW_DETAIL_MESSAGES;
	}
	else {
		testingCtx.reportingLevel = TestHarness::PASS_FAIL_ONLY;
	}

	// get the severity code from the command message
	string severityStr = cmdMsg.get("severity");

	if (severityStr == "LOGGER_ERROR") 
	{
		testingCtx.severity = LOGGER_ERROR;
	}
	else if (severityStr == "LOGGER_WARN") 
	{
		testingCtx.severity = LOGGER_WARN;
	}
	else if (severityStr == "LOGGER_DEBUG")
	{
		testingCtx.severity = LOGGER_DEBUG;
	}
	else if (severityStr == "LOGGER_TRACE") 
	{
		testingCtx.severity = LOGGER_TRACE;
	}
	else
	{
		testingCtx.severity = LOGGER_INFO;
	}

	testingCtx.showTimestamp  = cmdMsg.get("showTimestamp") == "true";

	TestDriver test_driver(
		cmdMsg.get("testDriver.name"), 
		cmdMsg.get("testDriver.dllName"),
		cmdMsg.get("testDriver.factoryMethod"));

	// add the tested code entries to our testdriver
	int i = 0;
	while(true) 
	{
		string s("testDriver.testedCodes.");
		(s += ('0' + i)) += ".dllName";
		if (cmdMsg.containsKey(s)) 
		{
			test_driver._tested_codes.push_back(TestedCode(cmdMsg.get(s)));
			i++;
		}
		else 
		{
			break;
		}
	}

	// run the test
	bool result = runTestDriver(testingCtx, test_driver);

	// add the results to the message
	resultMsg.attribute("resultValue", (result) ? "true" : "false");
	resultMsg.attribute("resultLog", UTIL::msgEncode(resultBuffer.str()));
}

bool runTestDriver(const TestingContext& testingCtx, const TestDriver& test_driver)
{
	// run the test, first loading any required dll's and then executing the test driver

	// load the test driver
	const char* libName = test_driver._dll_nm.c_str();
	const string factoryName = test_driver._factory_method_nm;
	ITest* itestHandler = loadTestDriver(libName, factoryName);

	if (itestHandler != nullptr) 
	{
		// load any required libraries for our test driver

		// run the test
		itestHandler->setup(&testingCtx, test_driver._tested_codes);
		bool result = itestHandler->runTests();
		itestHandler->teardown();

		// garbage collect the test driver
		if (itestHandler != nullptr)
		{
			delete itestHandler;
			itestHandler = nullptr;
		}

		return result;
	}
	else 
	{
		cerr << "failed to get test driver" << endl;
	}

	return false;
}

/*
*  Loads the Itest using "Run-Time Dynamic Linking," by loading a dll via LoadLibrary and
*    resolving functions using GetProcAddress.
*
*   Note: The Itest instance must be freed using delete.
*/
ITest* loadTestDriver(const char* libraryName, const string factoryFnName)
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
		TestFactory factory = (TestFactory)GetProcAddress(hDLL, factoryFnName.c_str());

		// see if we successfully loaded our library
		if (factory != nullptr) 
		{
			// success print happy message
			//cout << "Loaded factory method " << factoryFnName << "() from library: " << libraryName << endl;

			// now get the ITest interface
			ITest* itest = nullptr;

			factory(&itest);

			if (itest != nullptr) 
			{
				return itest;
			}
			else 
			{
				// fail print unhappy message
				cerr << "factory method " << factoryFnName << "(): did not return an ITest instance from testing library: " << libraryName << endl;
			}
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

void usage(void) {
	// print out a usage message
}