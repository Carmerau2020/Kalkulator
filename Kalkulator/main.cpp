#include "StringToOperators.h"
#include "Parser.h"
#include <iostream>

int main()
{
	std::string equation;
	StringToOperators converter;
	Parser parser;
	std::cout << "(To exit program type q) \n\n";
	while (true)
	{
		try
		{
			std::cout << "Enter equation:\n";
			std::getline(std::cin, equation);
			if (equation == "q")break;
		
			auto x = converter.recognize(equation);
			std::cout << equation << " = " << parser.counter(x)<<std::endl;
		}
		catch(const std::runtime_error& e)
		{
			std::cout << e.what();
		}
		catch (...)
		{
			std::cout << "Unexpected error";
		}
		std::cout << "\n\n";
	}
	return 0;
}