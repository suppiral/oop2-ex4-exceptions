#include "Calculator.h"
#include <exception>
#include <sstream>
#include "Macros.h"
#include <cctype>
#include "MyException.h"


using std::cout;
using std::cin;
using std::endl;
using std::stringstream;
using std::exception;
using std::getline;

// opens file and gets range of exercises
Calculator::Calculator()
{
	string str;
	do {
		cout << "Enter a filename to read exercised from: ";
		getline(cin, str); // get filename
	} while (openFile(_file, str) == BAD);
	
	cout << "Which exercises should I calculate? ";
	getRange();
}

Calculator::~Calculator()
{
	safeDestruct();
}

void Calculator::safeDestruct()
{
	_file.close();
}


// open file 
// gets: ifstream and filename
// returns: 0 if succeeded, BAD if not
int Calculator::openFile(ifstream& fd, string& filename)
{
	fd.open(filename);
	if (!fd) // if failed:
	{
		std::cerr << "Failed to open file : " << filename << std::endl;
		std::cout << "try again: ";
		return BAD;
	}
	return 0;
}

// read the range of exercised the user wants to calculate
void Calculator::getRange()
{
	string str;
	getline(cin, str);


	bool ranged = false; // checks for 'a-b' pattern of range
	unsigned ex_num, tmp_num;
	stringstream temp, linestream(str);

	

	char ch;

	while (!linestream.eof()) // read all characters in line
	{
		ch = linestream.get(); // get next character
		if (isdigit(ch))
			temp << ch; // add to temp num
		else if (isspace(ch) || ch == BAD) // BAD is eof
		{
			temp >> ex_num; // this is a number of an exercise
			temp.clear(); 

			if (ranged && tmp_num <= ex_num) // check if 'a-b' pattern and a<=b
			{
				if (tmp_num == 0)
					tmp_num++;
				for (unsigned i = tmp_num; i < ex_num; i++) // add all numbers in range
					addEx(i);
				ranged = false;
			}
			if (ex_num > 0)
				addEx(ex_num); // add this number
		}
		else if (ch == '-') // 'a-b' pattern
		{
			temp >> tmp_num;
			temp.clear();
			ranged = true;
		}
		else { /* throw */ } // bad input
	}
}

// adds a new exercise to calculate
void Calculator::addEx(unsigned ex_num)
{
	// check for duplicates
	for (list<unsigned>::iterator it = _range.begin(); it != _range.end(); it++)
		if (*it == ex_num)
			return;

	_range.push_back(ex_num);
}

void Calculator::calculate()
{
	// sort exercises
	_range.sort();

	// calculate lines in range
	for (unsigned line_num = 0; !_file.eof() && !_range.empty(); line_num++)
	{
		string line_str;
		getline(_file, line_str);

		if (_range.front() == line_num+1) // if this line number is in range
		{
			cout << line_num+1 << ". " << line_str << " " << calculateLine(line_str) << endl;
			_range.erase(_range.begin());
		}
	}
	while (!_range.empty())
	{
			cout << _range.front() << ". ERROR: Exercise doesn't exist"  << endl;
			_range.erase(_range.begin());
	}
}

string Calculator::calculateLine(string& line_str)
{
	double opperandA, opperandB;
	char action;

	try // parse opperands and action
	{
		stringstream line(line_str);
		line.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
		line >> opperandA;
		line.get(); // read space
		action = line.get();
		line >> opperandB;
	}
	catch (exception&) // catch invaild opperands
	{
		return "ERROR: Not a number operand";
	}

	double result;

	try // calculating
	{
		switch (action)
		{
		case ADD_CHAR:
			result = opperandA + opperandB;
			break;
		case SUB_CHAR:
			result = opperandA - opperandB;
			break;
		case MUL_CHAR:
			result = opperandA * opperandB;
			break;
		case DIV_CHAR:
			if (opperandB == 0)
			{
				DivByZeroException e;
				throw e;
			}
			result = opperandA / opperandB;
			break;
		default:
			{
				exception e("ERROR: Unrecognized operator");
				throw e;
			}
		}
	} 
	// catch exceptions
	catch (DivByZeroException&) { return "ERROR: Divide by zero"; }
	catch (exception& e) { return e.what(); }

	// generate result as a string
	stringstream result_line;
	result_line << "= " << result;

	return result_line.str();
}