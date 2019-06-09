// TestApp.cpp : Example file to test. Includes library functions

#include <iostream>
#include "..\TestLibrary\TestLibrary.h"

#ifdef TEST_TESTFILES

using namespace std;

int main()
{
	cout << "Begin test application" << endl;

	testLibFunc1();

	testLibFunc2(true);

	return 0;
}

#endif