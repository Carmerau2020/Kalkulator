#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Operator.h"
#include <iostream>

class StringToOperators {
public:
	std::vector<Operator> recognize(std::string str) const {

		// insert spaces in the right places in order to properly parse expression
		for (int i{ 0 }; i < str.length(); i++)
		{
			if (!isdigit(str[i]))
			{
				if (str[i] != '.')
					if(str[i] != ' ')
						str.insert(i + 1, 1, ' '); 
			}
			else if (isdigit(str[i]) && !isdigit(str[i + 1]))
			{
				if (str[i+1] != '.')
					if (str[i] != ' ')
						str.insert(i + 1, 1, ' ');
			}
		}

		// parse expression
		std::stringstream parsed(str);  
		std::vector<Operator> charsVector;
		double number;
		char operationSign;
		std::string equation;

		while (true)
		{ 
			parsed >> equation;
			if (parsed.eof() || !parsed.good()) break;
			if (isdigit(equation[0]))
			{
				number = std::stod(equation);
				charsVector.push_back(Operator(number));
			}
			else
			{
				operationSign = equation[0];
				charsVector.push_back(Operator(operationSign));
			}
		}

		for (int i = 1; i < (int)charsVector.size(); i++)
		{
			if (charsVector[i - 1].type == Operator::Type::OpenBracket && charsVector[i].type == Operator::Type::Minus)
			{
				charsVector.insert(charsVector.begin() + i, Operator(0));
			}
			if (charsVector[i - 1].type == Operator::Type::CloseBracket && (charsVector[i].type == Operator::Type::OpenBracket || charsVector[i].type == Operator::Type::Number))
			{
				charsVector.insert(charsVector.begin() + i, Operator('*'));
			}
			if (charsVector[i - 1].type == Operator::Type::Minus && charsVector[i].type == Operator::Type::Minus)
			{
				charsVector.erase(charsVector.begin() + i - 1, charsVector.begin() + i + 1);
				charsVector.insert(charsVector.begin() + i - 1, Operator('+'));
			}
			if (charsVector[i - 1].type == Operator::Type::Plus && charsVector[i].type == Operator::Type::Minus)
			{
				charsVector.erase(charsVector.begin() + i - 1, charsVector.begin() + i + 1);
				charsVector.insert(charsVector.begin() + i - 1, Operator('-')); 
			}

		}	
		return charsVector;
	}
	
};