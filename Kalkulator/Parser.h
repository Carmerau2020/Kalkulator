#pragma once
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>
#include <cmath>
#include "Operator.h"

class Parser
{
	std::string errorMassage = "Invalid equation format";
	void checkEquationErrors(std::vector<Operator>& ops) const
	{
		if (ops.size() == 2 || ops.size() == 0)
			throw std::runtime_error(errorMassage);
		if (ops[0].type != Operator::Type::Number)
			if (ops[0].type != Operator::Type::OpenBracket)
				throw std::runtime_error(errorMassage);
		if(ops[ops.size()-1].type!=Operator::Type::Number && ops[ops.size() - 1].type != Operator::Type::CloseBracket)
			throw std::runtime_error(errorMassage);
		for (int i = 1; i < (int)ops.size(); i++)
		{
			if (ops[i-1].type != Operator::Type::Number && ops[i].type != Operator::Type::Number)
			{
				if (ops[i-1].type != Operator::Type::OpenBracket && ops[i].type != Operator::Type::OpenBracket)
				{
					if (ops[i-1].type != Operator::Type::CloseBracket && ops[i].type != Operator::Type::CloseBracket)
						throw std::runtime_error(errorMassage);
				}
			}
		}
	}
	void doOperation(std::vector<Operator>& ops, Operator::Type type, std::function<double(double, double)> fn)const
	{
		double result = 0.0;
		for (int i = 0; i < (int)ops.size(); i++)
		{
			if (ops[i].type == type)
			{
				result = fn(ops[i - 1].getValue(), ops[i + 1].getValue());
				ops.erase(ops.begin() + (i - 1), ops.begin() + (i + 2));
				if (ops.size() == 0 || (i > (int)ops.size()))
					ops.push_back(Operator(result));
				else
					ops.insert(ops.begin() + i - 1, Operator(result));
				i--;
			}
		}
	}

public:
	Parser() = default;
	//************ Calculate equation **************
	double counter(std::vector<Operator>& ops) const
	{
		try {
			checkEquationErrors(ops);
			for (int i = 0; i < (int)ops.size(); i++)
			{
				int bracketCounter{ 0 }, posOpenBracket{ 0 }, posCloseBracket{ 0 };
				bool isSearchingStarted{ false };

				//******** Searching brackets ***********
				for (int i = 0; i < (int)ops.size(); i++)
				{
					//********  Opening bracket search ***********
					if (ops[i].type == Operator::Type::OpenBracket)
					{
						if (isSearchingStarted)
							bracketCounter++;
						else
						{
							posOpenBracket = i;
							bracketCounter = 1;
							isSearchingStarted = true;
						}
					}
					//******** Closing bracket search *************
					if (ops[i].type == Operator::Type::CloseBracket)
					{
						bracketCounter--;
						if (bracketCounter == 0)
						{
							posCloseBracket = i;
							break;
						}
					}
				}
				if(bracketCounter!=0)
					throw std::runtime_error(errorMassage);

				//*********** Calculate value in brackets ****************
				if (!isSearchingStarted) break;
				std::vector<Operator> bracketsContent;
				for (int i = posOpenBracket + 1; i < posCloseBracket; i++)
				{
					bracketsContent.push_back(ops[i]);
				}
				ops.erase(ops.begin() + posOpenBracket, ops.begin() + posCloseBracket + 1);

				double bracketsValue = counter(bracketsContent);
				if (ops.size() == 0 || (i > (int)ops.size()))
					ops.push_back(bracketsValue);
				else
					ops.insert(ops.begin() + posOpenBracket, bracketsValue);
			}

			// **************************** MULTIPLY AND DIVIDE *******************************
			doOperation(ops, Operator::Type::Multiply, [](double a, double b) { return a * b; });// ,
			doOperation(ops, Operator::Type::Divide, [](double a, double b)
				{ if (b == 0) 
					{throw std::runtime_error("You shoudn't divide by 0");}
				  else 
					return a / b; });

			// ******************************** PLUS AND MINUS *******************************
			doOperation(ops, Operator::Type::Plus, [](double a, double b) { return a + b; });//,
			doOperation(ops, Operator::Type::Minus, [](double a, double b) { return a - b; });
			
			return ops[0].getValue();
		}
		catch (const std::runtime_error& e)
		{
			throw e;
		}
		catch (...)
		{
			throw std::runtime_error("Unexpected error");
		}
	}
};