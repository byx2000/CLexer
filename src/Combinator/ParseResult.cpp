#include "ParseResult.h"

using namespace std;
using namespace byx;

// ParseResult

byx::ParseResult::ParseResult(const std::string& recognized, const std::string& remaining, bool succeeded)
	: recognized(recognized), remaining(remaining), succeeded(succeeded) {}

std::string byx::ParseResult::getRecognized() const
{
	return recognized;
}

std::string byx::ParseResult::getRemaining() const
{
	return remaining;
}

bool byx::ParseResult::isSucceeded() const
{
	return succeeded;
}

byx::ParseResult::operator bool() const
{
	return isSucceeded();
}

ParseResult byx::ParseResult::CreateFail()
{
	return ParseResult("", "", false);
}

ParseResult byx::ParseResult::CreateSucceed(const std::string& recognized, const std::string& remaining)
{
	return ParseResult(recognized, remaining, true);
}

std::string byx::ParseResult::toString() const
{
	string s = "";
	if (succeeded)
	{
		s = "true:  ";
	}
	else
	{
		s = "false: ";
	}
	s += recognized + " | " + remaining;
	return s;
}
