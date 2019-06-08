#ifndef TESTLIBRARY_H
#define TESTLIBRARY_H


#ifdef TESTLIBRARY_EXPORTS
#define TESTLIBRARY_API __declspec(dllexport)
#else
#define TESTLIBRARY_API __declspec(dllimport)
#endif


extern "C" TESTLIBRARY_API bool testLibFunc1();

extern "C" TESTLIBRARY_API bool testLibFunc2(bool b);

extern "C" TESTLIBRARY_API bool testLibFunc3(bool b);

#endif
