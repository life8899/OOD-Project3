

#include "pch.h"
#include "TestLibrary.h"

// Function always passes
bool testLibFunc1()
{
	return true;
}

// Function only returns the boolean value given
bool testLibFunc2(bool b)
{
	return b;
}

// Function throws an exception if b == true
bool testLibFunc3(bool b)
{
	if (b)
		throw "EXCEPTION THROWN WITHIN TESTLIBRARY!\n";
	
	return true;
}
