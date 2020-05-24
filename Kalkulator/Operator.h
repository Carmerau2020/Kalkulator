#pragma once
#include <stdexcept>
#include <string>

class Operator
{
	double value;

public:
	enum class Type
	{
		Plus = '+',
		Minus = '-',
		Divide = '/',
		Multiply = '*',
		OpenBracket = '(',
		CloseBracket = ')',
		
		Number = -1,
	};

	Type type;

	Operator(char character)
		: type(Type(character)), value(-1)
	{
		if(character < 40 || character > 47 || character == 44)
			throw std::runtime_error("Invalid equation format");
	}

	Operator(double number)
		: type(Type::Number), value(number)
	{
	}

	Operator(int number)
		: type(Type::Number), value(number)
	{
	}

	double getValue() const
	{
		return value;
	}

	bool operator==(const Operator& rhs) const
	{
		if (value != rhs.value || type != rhs.type)
			return false;
		return true;
	}

	bool operator!=(const Operator& rhs) const
	{
		if (value != rhs.value || type != rhs.type)
			return true;
		return false;
	}
};