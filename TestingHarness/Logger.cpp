/////////////////////////////////////////////////////////////////////////////////
// Logger.cpp - Logger class definition                                        //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Platform:    HP G1 800, Windows 10                                          //
// Application: Server Process Pools Project3, CSE687 - Object Oriented Design //
// Author:      Eric Voje, Kuohsun Tsai, Chaulladevi Bavda                     //
//              ervoje@syr.edu, kutsai@syr.edu, cvbavda@syr.edu                //
/////////////////////////////////////////////////////////////////////////////////

#include "../Includes/TestLogger.h"

#include <iostream>
#include <string>

using std::endl;
/*
* simple no arg contructor creates a Logger with no attached ostreams,
*   and a default severity of INFO
*/
LoggerX::LoggerX(void) 
	: log_streams(), severity(LOGGER_INFO) {}

/*
* creates a Logger with a single given attached ostream to output to,
*   and a default severity of INFO
*/
LoggerX::LoggerX(std::ostream* out_strm)
	: log_streams(), severity(LOGGER_INFO)
{
	// attach the given ostream
	this->attach(out_strm);
}

// stream management functions
void LoggerX::attach(std::ostream* out_strm)
{
	// first make sure the out_strm points to a real (open) ostream and then
	//   add it to our list of streams
	if(out_strm!=nullptr && *out_strm) this->log_streams.push_back(out_strm);
}

void LoggerX::flush() const
{
	for (std::ostream* out_strm: this->log_streams) 
	{
		// make sure the stream is valid
		if (out_strm != nullptr && *out_strm) 
		{
			out_strm->flush();
		}
		else
		{
			// remove this stream from our vector
			_logErr(LOGGER_ERROR, "Error - invalid stream");
		}
	}
}

/*
* logging methods by severity, takes in the context string and a message to log
*/
void LoggerX::log(int severity, const std::string&ctxStr, const std::string& msg) const
{
	// first check the severity code
	if (severity <= this->severity) 
	{

		// append this message to our Logger ostreams
		for (std::ostream* out_strm : this->log_streams) 
		{
			// make sure the stream is valid
			if (out_strm != nullptr && (*out_strm)) 
			{
				*out_strm << msg << endl;
				out_strm->flush();
			}
			else
			{
				// remove this stream from our vector
				_logErr(LOGGER_ERROR, "Error - invalid stream");
			}
		}
	}
}
void LoggerX::trace(const std::string&ctxStr, const std::string& msg) const
{
	this->log(LOGGER_TRACE, ctxStr, msg);
}
void LoggerX::debug(const std::string&ctxStr, const std::string& msg) const
{
	this->log(LOGGER_DEBUG, ctxStr, msg);
}
void LoggerX::info(const std::string&ctxStr, const std::string& msg) const
{
	this->log(LOGGER_INFO, ctxStr, msg);
}
void LoggerX::warn(const std::string&ctxStr, const std::string& msg) const
{
	this->log(LOGGER_WARN, ctxStr, msg);
}
void LoggerX::error(const std::string&ctxStr, const std::string& msg) const
{
	this->log(LOGGER_ERROR, ctxStr, msg);
}

// gets and sets the severity for filtering messages
int  LoggerX::getSeverity(void) const
{
	return severity;
}
void LoggerX::setSeverity(int val)
{
	severity = val;
}

void LoggerX::_logErr(int severity, const std::string& msg) const 
{

	// first check the severity code
	if (severity <= this->severity)
	{
		// first print to stderr
		std::cerr << msg << endl;
		std::cerr.flush();

		// append this message to our Logger ostreams
		for (std::ostream* out_strm : this->log_streams)
		{
			// make sure the stream is valid
			if (out_strm != nullptr && *out_strm)
			{
				*out_strm << msg << endl;
				out_strm->flush();
			}
		}
	}
}