#include "../Includes/FileManager.h"
#include "../Includes/TestDriver.h"
#include "../Includes/XMLReader.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

Tests FileManager::readTestFile(std::string filename) {

	std::cout << "\n  Testing reading test xml file";
	std::cout << "\n ========================\n";

	// open our test xml file
	std::string fileName("testXML.xml");
	std::ifstream inFileStrm(fileName);

	Tests tests;
	bool root = true;

	if (inFileStrm) {
		std::string test1;
		std::string buffer;

		while (std::getline(inFileStrm, buffer)) {
			//test1.insert(buffer);
			test1 += buffer;
		}
		inFileStrm.close();

		XmlReader rdr(test1);

		std::string lastTag("none");

		while (rdr.next())
		{
			//std::cout << "\ntag:     " << rdr.tag().c_str();
			const std::string tag = rdr.tag();

			if ("tests" == tag) {

				// simple validation check
				if (!root || "none" != lastTag) {
					// oops big error throw exception
					throw std::runtime_error("<tests> must be the root element.");
				}
				root = false;
				lastTag = "tests";
			}
			else if ("test" == tag) {

				// simple validation check
				if (root || ("unknown" == lastTag)) {
					// oops big error throw exception
					throw std::runtime_error("XML parsing error.");
				}

				// get the name attribute (optional)
				std::string name;

				XmlReader::attribElems attribs = rdr.attributes();
				for (size_t i = 0; i < attribs.size(); ++i) {

					if ("name" == attribs[i].first) {
						name = attribs[i].second;
					}
				}

				tests.push_back(Test(name));
				root = false;
				lastTag = "test";
			}
			else if ("testdriver" == tag) {

				if (root || ("unknown" == lastTag)) {
					// oops big error throw exception
					throw std::runtime_error("XML parsing error.");
				}

				// get the name attribute (optional)
				std::string name;
				// get the dllname and dllmethod attribute (required)
				std::string dll_name;
				std::string dll_method;

				XmlReader::attribElems attribs = rdr.attributes();
				for (size_t i = 0; i < attribs.size(); ++i) {

					if ("name" == attribs[i].first) {
						name = attribs[i].second;
					}
					else if ("dllname" == attribs[i].first) {
						dll_name = attribs[i].second;
					}
					else if ("dllmethod" == attribs[i].first) {
						dll_method = attribs[i].second;
					}
				}

				if (tests.size() < 1) {
					// oops big error throw exception
					throw std::runtime_error("<testdriver> must be a child of <test>.");
				}
				// create a test driver and add it to the last test
				tests.back()._testdrivers.push_back(TestDriver(name, dll_name, dll_method));

				root = false;
				lastTag = "testdriver";
			}
			else if ("testedcode" == tag) {

				if (root || !("testdriver" == lastTag || "testedcode" == lastTag)) {
					// oops big error throw exception
					throw std::runtime_error("XML parsing error.");
				}
				// get the dllname (required)
				std::string dll_name;

				XmlReader::attribElems attribs = rdr.attributes();
				for (size_t i = 0; i < attribs.size(); ++i) {
					if ("dllname" == attribs[i].first) {
						dll_name = attribs[i].second;
					}
				}

				if (tests.size() < 1) {
					// oops big error throw exception
					throw std::runtime_error("<testdriver> must be a child of <test>.");
				}
				// create a test driver and add it to the last test
				Test& t = tests.back();

				if(t._testdrivers.size() <1) {
					// oops big error throw exception
					throw std::runtime_error("<testedcode> must be a child of <testdriver>.");
				}

				t._testdrivers.back()._tested_codes.push_back(TestedCode(dll_name));

				root = false;
				lastTag = "testedcode";
			}
			else {

				// simple validation check
				if (root) {
					// oops big error throw exception
					throw std::runtime_error("<tests> must be the root element.");
				}

				// unknown element ignore
				root = false;
				lastTag = "unknown";
			}
		}
	}
	else {
		std::cout << "Could not open input file: " << fileName << std::endl << std::endl << std::endl;
	}

	return tests;
}