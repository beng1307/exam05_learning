#include "vect2.hpp"

vect2	&vect2::operator=(const vect2 &other)
{
	if (this != &other)
	{
		x = other.x;
		y = other.y;
	}
	return (*this);
}

//Accesses

int	vect2::operator[](int index) const {return index == 0 ? x : y;};
int	&vect2::operator[](int index) {return index == 0 ? x : y;};


//increments & decrements

vect2	&vect2::operator++()
{
	++x;
	++y;

	return (*this);
}

vect2	vect2::operator++(int)
{
	vect2	copy(*this);

	++x;
	++y;
	
	return (copy);
}

vect2	&vect2::operator--()
{
	--x;
	--y;

	return (*this);
}

vect2	vect2::operator--(int)
{
	vect2	copy(*this);

	--x;
	--y;

	return (copy);
}


//Maths

vect2	vect2::operator+(const vect2 &other) const
{
	vect2	copy(*this);

	copy.x += other.x;
	copy.y += other.y;

	return (copy);
}

vect2	&vect2::operator+=(const vect2 &other)
{
	x += other.x;
	y += other.y;

	return (*this);
}

vect2	vect2::operator-(const vect2 &other) const
{
	vect2	copy(*this);

	copy.x -= other.x;
	copy.y -= other.y;

	return (copy);
}

vect2	&vect2::operator-=(const vect2 &other)
{
	x -= other.x;
	y -= other.y;

	return (*this);
}

vect2	vect2::operator*(int scalar) const
{
	vect2	copy(*this);

	copy.x *= scalar;
	copy.y *= scalar;

	return (copy);
}

vect2	&vect2::operator*=(int scalar)
{
	x *= scalar;
	y *= scalar;

	return (*this);
}


//Unary Minus

vect2	vect2::operator-() const
{
	vect2	copy(*this);

	copy.x = -copy.x;
	copy.y = -copy.y;

	return (copy);
}



//Comparisons

bool	vect2::operator==(const vect2 &other) const {return x == other.x && y == other.y;};
bool	vect2::operator!=(const vect2 &other) const {return x != other.x || y != other.y;};


vect2	operator*(int scalar, const vect2 &other)
{
	vect2	copy(other);

	copy *= scalar;

	return (copy);
}

std::ostream &operator<<(std::ostream &out, const vect2 &other)
{
	out << "{" << other[0] << ", " << other[1] << "}";
	return (out);
}