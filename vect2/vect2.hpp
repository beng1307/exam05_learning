#pragma once

#include <iostream>

class vect2
{
	private:

		int x;
		int y;

	public:

		vect2(int _x = 0, int _y = 0): x(_x), y(_y) {};
		vect2(const vect2 &other): x(other.x), y(other.y) {};
		vect2	&operator=(const vect2 &other);

		//Accesses
		int operator[](int index) const;
		int	&operator[](int index);

		//increments & decrements
		vect2	&operator++();
		vect2	operator++(int);
		vect2	&operator--();
		vect2	operator--(int);

		//Maths
		vect2	operator+(const vect2 &other) const;
		vect2	&operator+=(const vect2 &other);

		vect2	operator-(const vect2 &other) const;
		vect2	&operator-=(const vect2 &other);

		vect2	operator*(int scalar) const;
		vect2	&operator*=(int scalar);

		//Unary Minus
		vect2	operator-() const;

		//Comparisons
		bool	operator==(const vect2 &other) const;
		bool	operator!=(const vect2 &other) const;

};

vect2	operator*(int scalar, const vect2 &other);

std::ostream &operator<<(std::ostream &out, const vect2 &other);