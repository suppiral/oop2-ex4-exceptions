#pragma once

#include <exception>

using std::exception;

class DivByZeroException : public exception
{
public:
	DivByZeroException() : exception("ERROR: Divide by zero") {}
};