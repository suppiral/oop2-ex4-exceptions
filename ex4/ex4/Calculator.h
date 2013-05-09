#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <list>

using std::list;
using std::ifstream;
using std::string;

class Calculator
{
public:
	Calculator();
	~Calculator();
	void calculate();
	void addEx(unsigned ex_num);

private:
	ifstream _file;
	list<unsigned> _range;

	void getRange();
	int openFile(ifstream& fd, string& filename);
	void safeDestruct();

	string calculateLine(string& line_str);
};