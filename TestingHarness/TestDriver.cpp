#include "../Includes/TestDriver.h"

#include <Windows.h>
#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

TestDriver* TestDriver::loadTestDriver(const char* libraryName, const char* factoryFnName)
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
	if (NULL != hDLL) {

		// Load the factory method from out library
		TestFactory factory = (TestFactory)GetProcAddress(hDLL, factoryFnName);

		// see if we successfully loaded our library
		if (factory != nullptr) {
			// success print happy message
			cout << "Loaded factory method " << factoryFnName << "() from library: " << libraryName << endl;

			// now get the ITest interface
			ITest* itest = nullptr;

			factory(&itest);

			if (itest != nullptr) {
				//setup and run the test
				itest->setup(nullptr);
				bool results = itest->runTests();
				itest->teardown();
			}
			else {
				// fail print unhappy message
				cerr << "factory method " << factoryFnName << "(): did not return an ITest instance from testing library: " << libraryName << endl;
			}
		}
		else {
			// fail print unhappy message
			cerr << "could not load factory method " << factoryFnName << "() from library: " << libraryName << endl;
		}

		// unload library
		FreeLibrary(hDLL);
	}
	else {
		cerr << "Load library failed for: " << libraryName << endl;
	}

	return 0;
}

TestDriver::TestDriver(ITest* itest): _itest(itest)
{

}

void TestDriver::setup(const TestingContext& ctx)
{
	_itest->setup(ctx);
}

bool TestDriver::runTests()
{
	return _itest->runTests();
}

void TestDriver::teardown()
{
	_itest->teardown();
}

TestDriver::~TestDriver() {};