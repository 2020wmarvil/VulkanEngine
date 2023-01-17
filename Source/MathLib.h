#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <ostream>
#include <vector>

#define PI 3.14159265358979323846

class ComplexNumber {
public:
	ComplexNumber(float real, float imaginary)
		: re(real), im(imaginary) {}

	// TODO: complex conjugate

	ComplexNumber operator+=(const ComplexNumber& other)
	{
		re += other.re;
		im += other.im;
		return *this;
	}

	ComplexNumber operator-=(const ComplexNumber& other)
	{
		re -= other.re;
		im -= other.im;
		return *this;
	}

	ComplexNumber operator*=(const ComplexNumber& other)
	{
		float realTerm = re * other.re - im * other.im;
		float imaginaryTerm = re * other.im + im * other.re;
		re = realTerm;
		im = imaginaryTerm;
		return *this;
	}

	ComplexNumber operator/=(const ComplexNumber& other)
	{
		float denominator = other.re * other.re + other.im * other.im;
		float realTerm = (re * other.re + im * other.im) / denominator;
		float imaginaryTerm = (im * other.re - re * other.im) / denominator;
		re = realTerm;
		im = imaginaryTerm;
		return *this;
	}

	float Abs()
	{
		return sqrt(re * re + im * im);
	}

	float Arg()
	{
		return atan2(im, re);
	}

public:
	float re;
	float im;
};

inline ComplexNumber operator+(ComplexNumber lhs, const ComplexNumber& rhs)
{
	lhs += rhs;
	return lhs;
}

inline ComplexNumber operator-(ComplexNumber lhs, const ComplexNumber& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline ComplexNumber operator*(ComplexNumber lhs, const ComplexNumber& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline ComplexNumber operator/(ComplexNumber lhs, const ComplexNumber& rhs)
{
	lhs /= rhs;
	return lhs;
}

std::ostream& operator<<(std::ostream& os, const ComplexNumber& comp)
{
	os << comp.re << " + " << comp.im << "i";
	return os;
}

std::vector<float> DFT_Slow(std::vector<float> x)
{




	return x;
}
