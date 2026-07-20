#include "bigint.hpp"

//Additions


bigint	&bigint::operator+=(const bigint &other)
{
	int carry = 0;
	int	i = big.size() - 1;
	int	j = other.big.size() - 1;
	
	std::string result;
	
	while (i >= 0 || j >= 0 || carry)
	{
		int sum = carry;
		
		if (i >= 0)
			sum += big[i--] - '0';
		if (j >= 0)
			sum += other.big[j--] - '0';
		
		carry = sum / 10;
		result += (char)('0' + sum % 10);
	}
	
	std::reverse(result.begin(), result.end());
	big = result;
	
	return(*this);
}


bigint	bigint::operator+(const bigint &other) const
{
	bigint result(*this);

	result += other;

	return (result);
}


//Increments

bigint	&bigint::operator++()
{
	bigint	plus_one(1);

	*this += plus_one;

	return (*this);
}


bigint	bigint::operator++(int)
{
	bigint	plus_one(1);
	bigint	copy(*this);

	*this += plus_one;

	return (copy);
}


//Shifts


bigint	&bigint::operator<<=(unsigned int amount)
{
	if (big != "0")
		big.append(amount, '0');

	return(*this);
}


bigint	bigint::operator<<(unsigned int amount) const
{
	bigint	copy(*this);
	
	copy <<= amount;

	return (copy);
}


bigint	&bigint::operator>>=(const bigint &other)
{
    size_t	amount = std::atol(other.big.c_str());

    if (amount >= big.size())
        big = "0";
    else
        big.erase(big.size() - amount);
    return (*this);
}



bool bigint::operator<(const bigint &other) const
{
    if (big.size() != other.big.size())
        return big.size() < other.big.size();
    return big < other.big;
}

bool bigint::operator>(const bigint &other) const{ return (other < *this); }

bool bigint::operator<=(const bigint &other) const{	return !(*this > other); } 

bool bigint::operator>=(const bigint &other) const { return !(*this < other); }

bool bigint::operator==(const bigint &other) const { return (big == other.big); }

bool bigint::operator!=(const bigint &other) const { return (big != other.big); }


std::ostream &operator<<(std::ostream &out, const bigint &other)
{
	out << other.getBig();
	return (out);
}


