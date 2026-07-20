#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

class bigint
{
	private:

		std::string big;

	public:
	
		bigint(unsigned int number = 0)
		{
			std::ostringstream out;

			out << number;

			big = out.str();
		}
		bigint(const bigint &other): big(other.big) {};

		std::string getBig() const { return (big); };

		//Additions
		bigint &operator+=(const bigint &other);
		bigint operator+(const bigint &other) const;

		//Increments
		bigint &operator++();	// ++x
		bigint operator++(int);	// x++

		//Shifts
		bigint &operator<<=(unsigned int amount);
		bigint operator<<(unsigned int amount) const;
		bigint &operator>>=(const bigint &other);

		//Comparisons
		bool operator<(const bigint &other) const;
		bool operator>(const bigint &other) const;
		bool operator<=(const bigint &other) const;
		bool operator>=(const bigint &other) const;
		bool operator==(const bigint &other) const;
		bool operator!=(const bigint &other) const;
};

std::ostream &operator<<(std::ostream &out, const bigint &other);