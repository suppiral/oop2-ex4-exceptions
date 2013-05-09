#include <fstream>
#include <iostream>
#include <string>
#include "Calculator.h"


using std::cout;
using std::cin;
using std::string;
using std::ifstream;

int main()
{
	cout << "Welcome to My Calculator!\n\n";

	Calculator calc;
	calc.calculate();

	system("PAUSE");

	return EXIT_SUCCESS;
}