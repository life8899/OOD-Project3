// TestServer.cpp : Defines the entry point for the console application.
/////////////////////////////////////////////////////////////////////////////////
// TestServer.cpp - TestServer class definition                                //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#include "../Includes/Util.h"
#include "../Includes/FileManager.h"

#include "../MsgPassingComm/Comm.h"

#include <iostream>
#include <string>

using MsgPassingCommunication::Comm;
using MsgPassingCommunication::Message;
using MsgPassingCommunication::EndPoint;

using std::cerr;
using std::cout;
using std::endl;
using std::string;

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

// global variables
static bool cumulativeTestingResult = true;
//static bool keepRunning = true;

// need to add a queue for ready test executors
BlockingQueue<Message> readyMessages;

// need to add a queue for active executor endpointss
BlockingQueue<EndPoint> activeExecutorEndpoints;

// need another queue for test drivers yet to be run
BlockingQueue<TestDriver> waitingTestDrivers;

// total number of tests so we know whenb we are done
int totalTests = 0;
bool loopStarted = false;

// need another queue for keeping track of the results
BlockingQueue<bool> resultQueue;

// forward declarations
static void usage(string);
static bool keepRunning(void);
static bool createTestExecutor(string serverAddr, int serverPort, string executorAddr, int executorPort, string executorNm);
static wchar_t *convertCharArrayToLPCWSTR(const char* charArray);

//message handler functor
class MessageHandler 
{
public:
	MessageHandler(Comm& comm) : comm(&comm) {}

	void operator()(void);
private:
	Comm* comm;
};

// message handler
void MessageHandler::operator()(void)
{

	DEBUG(cout << "MessageHandler(): starting message handler." << endl);

	while (keepRunning()) 
	{
		// wait for message
		//DEBUG(cout << "MessageHandler(): waiting for next message" << endl);
		Message cmdMsg = (*comm).getMessage();

		// get the command message
		string cmdNm = cmdMsg.command();
		string sender = cmdMsg.name();

		if (cmdNm == "ready") 
		{
			cout << "\n\nnew ready message recieved:" << endl;
			DEBUG(cout << "\tto = " << cmdMsg.to().toString() << endl);
			DEBUG(cout << "\tfrom = " << cmdMsg.from().toString() << endl);
			DEBUG(cout << "\tname = " << cmdMsg.name() << endl);
			DEBUG(cout << "\tcommand = " << cmdMsg.command() << endl);
			DEBUG(cout << "\tid = " << cmdMsg.id() << endl);
			// add this client to the client ready queue
			readyMessages.enQ(cmdMsg);

			// add this client to the client active list, if it is not already listed
			activeExecutorEndpoints.enQ(cmdMsg.from());
		}
		else if (cmdNm == "runTestResult") 
		{
			cout << "\n\nnew result message recieved:" << endl;
			DEBUG(cout << "\tto = " << cmdMsg.to().toString() << endl);
			DEBUG(cout << "\tfrom = " << cmdMsg.from().toString() << endl);
			DEBUG(cout << "\tname = " << cmdMsg.name() << endl);
			DEBUG(cout << "\tcommand = " << cmdMsg.command() << endl);
			DEBUG(cout << "\tid = " << cmdMsg.id() << endl);
			DEBUG(cout << "\treply_to = " << cmdMsg.reply_to() << endl);
			cout << UTIL::msgDecode(cmdMsg.get("resultLog")) << endl;

			// update the testing results
			resultQueue.enQ(cmdMsg.get("resultValue") == "true");
		}
		else if (cmdNm == "stopping") 
		{
			// update the client status as stopped

			// pop the executor from the active list and ready list
		}
		else {
			//unknown message
		}
	}
}

int main(int argc, char** argv)
{
	// log to the stdout
	StaticLogger<1>::attach(&std::cout);

	// parse the cmd line options
	//	a) server port
	//  b) server addr
	//	c) server name
	//  d) file to read
	//  e) number of test executors to start
	if (argc < 6) 
	{
		usage(argv[0]);
	}

	char *serverAddr = argv[1];
	int serverPort = atoi(argv[2]);
	char *serverNm = argv[3];
	char *testFileName = argv[4];
	int numTestExecutors = atoi(argv[5]);

	// define the server endpoint
	EndPoint serverEP(serverAddr, serverPort);

	// create a Comm object to start listening for messages on clientAddr:clientPort
	DEBUG(
		cout << "creating Comm object to start listening for messages on "
		<< serverAddr << ":" << serverPort << "" << endl
	);

	SocketSystem ss; // !IMPORTANT: for some reasons this is required to work
	Comm comm(EndPoint(serverAddr, serverPort), serverNm);
	comm.start();
	DEBUG(cout << "now listening on " << serverAddr << ":" << serverPort << "" << endl;);

	// create a thread to listen for incoming client ready messages
	DEBUG(cout << "starting thread to listen for executor ready messages." << endl;);
	MessageHandler msgHandler(comm);
	std::thread clientAccepterThread(msgHandler);
	DEBUG(cout << "thread started." << endl;);

	::Sleep(100);

	// start the test executor processes
	DEBUG(cout << "starting test executor processes." << endl;);
	for (int i = 0; i < numTestExecutors; i++)
	{
		string executorAddr = "localhost";
		int executorPort = serverPort + 1 + i;
		string executorNm("testExecutor");
		executorNm += '0' + (i+1);

		createTestExecutor(serverAddr, serverPort, executorAddr, executorPort, executorNm);
	}
	DEBUG(cout << "done starting test executor processes." << endl;);

	// read in the testing script, and enque the tests
	DEBUG(cout << "reading test file: filename = " << testFileName << endl;);
	Tests tests = FileManager::readTestFile(testFileName);

	int testCnt = 0;
	int testDriverCnt = 0;

	//For each test entry in test
	for (Test test : tests)
	{
		testCnt++;
		int tmp = testDriverCnt;

		// loop over test drivers in each test
		for (TestDriver test_driver : test._testdrivers)
		{
			waitingTestDrivers.enQ(test_driver);
			testDriverCnt++; 
			totalTests++;
		}
		DEBUG(cout << "\tadded Test" << tests.size() << " with " << (testCnt-tmp) << " test drivers." << endl;);
	}
	DEBUG(cout << "found " << testCnt << " tests and " << testDriverCnt << "total test drivers." << endl;);


	// setup the testing context for controlling the testing output
	string reportingLevel = "SHOW_ALL_MESSAGES";
	string severity       = "LOGGER_INFO";
	bool showTimestamp    = true;

	// loop until all tests are completed or a forced exit
	while (waitingTestDrivers.size()>0) 
	{
		loopStarted = true;
		if(waitingTestDrivers.size()>0 && readyMessages.size()>0) 
		{
			// pop the next ready executor
			Message readyMessage = readyMessages.deQ();

			// lock the testqueue
			// check size if size > 0 the pop top testdriver and send runtTest command
			if (waitingTestDrivers.size() > 0) {
				TestDriver td = waitingTestDrivers.deQ();

				Message cmdMsg(readyMessage.from(), serverEP);
				cmdMsg.name(readyMessage.name());
				cmdMsg.command("runTest");

				// step 1 - put the reporting level and timestamp settings into the command message
				cmdMsg.attribute("reportingLevel", reportingLevel);
				cmdMsg.attribute("severity", severity);
				cmdMsg.attribute("showTimestamp", (showTimestamp)?"true":"false");

				// step 2 - put a simulated TestDriver into the command message
				cmdMsg.attribute("testDriver.name", td._test_driver_name);
				cmdMsg.attribute("testDriver.dllName", td._dll_nm);
				cmdMsg.attribute("testDriver.factoryMethod", td._factory_method_nm);

				// step 2b - put the tested code entries into the simulated TestDriver, in the command message
				int i = 0;
				for (const TestedCode& testedCode : td._tested_codes) 
				{
					string s("testDriver.testedCodes.");
					(s += ('0' + i)) += ".dllName";
					cmdMsg.attribute(s, testedCode._dll_nm);
				}

				// post the message
				comm.postMessage(cmdMsg);
			}
			// else return executor to ready queue
			else 
			{
				readyMessages.enQ(readyMessage);
			}
		}
		else
		{
			// sleep for 1 second
			::Sleep(1000);
		}
	}

	// now wait for the message handler thread to exit
	clientAccepterThread.join();

	// send a stop command to all active Executor processes
	while (activeExecutorEndpoints.size() > 0)
	{
		EndPoint executorEP = activeExecutorEndpoints.deQ();

		//send stop message
		DEBUG(cout << "sending stop message" << endl);
		Message stop(executorEP, serverEP);
		stop.command("stop");
		comm.postMessage(stop);
		DEBUG(cout << "stop message sent" << endl);
	}
	comm.stop();

	// print out the final result
	bool cumulativeTestingResult = true;
	while (resultQueue.size() > 0)
	{
		bool testRslt = resultQueue.deQ();
		cumulativeTestingResult &= testRslt;
	}

	if (cumulativeTestingResult) 
	{
		cout << "All tests passed." << endl;
	}
	else {
		cout << "Some Tests Failed." << endl;
	}

    return 0;
}

// creates a new test executor process with the given parameters
static bool createTestExecutor(string serverAddr, int serverPort, string executorAddr, int executorPort, string executorNm) 
{
	DEBUG(
		cout << "createTestExecutor: called with: "
		<< "serverAddr = " << serverAddr
		<< ", serverPort = " << serverPort
		<< ", executorAddr = " << executorAddr
		<< ", executorPort = " << executorPort
		<< ", executorNm = " << executorNm
		<< endl;
	);

	// Assume this application is invokes as follows:
	// > TestExecutor serverAddr serverPort executorAddr executorPort executorNm
	string cmdLine;

	char buffer[1024];

	cmdLine += "TestExecutor ";
	cmdLine += serverAddr + " ";

	_itoa_s(serverPort, buffer, 1024, 10); 
	(cmdLine += buffer) += " ";
	cmdLine += executorAddr + " ";
	_itoa_s(executorPort, buffer, 1024, 10);
	(cmdLine += buffer) += " ";
	cmdLine += executorNm + " ";

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	DEBUG(cout << "\tstarting process with cmd line: " << cmdLine << endl;);

	/* Start the child process. 
	 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms682512(v=vs.85).aspx
	 */
	if (!CreateProcess(NULL,   // No module name (use command line)
		convertCharArrayToLPCWSTR(cmdLine.c_str()),        // Command line
		NULL,                                              // Process handle not inheritable
		NULL,                                              // Thread handle not inheritable
		FALSE,                                             // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,                                // No creation flags
		NULL,                                              // Use parent's environment block
		NULL,                                              // Use parent's starting directory 
		&si,                                               // Pointer to STARTUPINFO structure
		&pi)                                               // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return false;
	}

	DEBUG(cout << "\tprocess started." << endl;);

	return true;
}

static void usage(string appName) 
{
	// print out a usage message
	cerr << "Usage: " << appName << " serverAddress serverPort serverName testFileName numExecutors" << endl;
}

static bool keepRunning(void)
{
	return ((waitingTestDrivers.size()>0) || (totalTests>resultQueue.size()) || !loopStarted);
}

static wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}