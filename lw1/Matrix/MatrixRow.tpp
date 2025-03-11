#pragma once

#include <cmath>
#include <format>
#include <iostream>
#include <sstream>

template <std::size_t N, typename T>
std::ostream& operator<<(std::ostream& os, MatrixRow<N, T> row_)
{
	unsigned int columnWidth = MatrixRow<N, T>::NumberLength(*std::max_element(row_.begin(), row_.end(), [](const T& a, const T& b) {
		return MatrixRow<N, T>::NumberLength(a) < MatrixRow<N, T>::NumberLength(b);
	}));
	return row_.stringify(os, columnWidth);
}

template <std::size_t N, typename T>
std::function<std::ostream&(std::ostream&)> MatrixRow<N, T>::stringify(int columnWidth)
{
	return [this, columnWidth](std::ostream& os) -> std::ostream& {
		return stringify(os, columnWidth);
	};
}

template <std::size_t N, typename T>
std::ostream& MatrixRow<N, T>::stringify(std::ostream& os, int columnWidth)
{
	os << "| ";
	for (T elt : *this)
	{
		os << std::format("{:{}} ", elt, columnWidth);
	}
	os << "|";
	return os;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator*(T multiplier)
{
	MatrixRow<N, T> result = *this;
	for (T& elt : result)
	{
		elt *= multiplier;
	}
	return result;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator*=(T multiplier)
{
	for (T& elt : *this)
	{
		elt *= multiplier;
	}
	return *this;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator/(T divider)
{
	MatrixRow<N, T> result = *this;
	for (T& elt : result)
	{
		elt /= divider;
	}
	return result;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator/=(T divider)
{
	for (T& elt : *this)
	{
		elt /= divider;
	}
	return *this;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator+(const MatrixRow<N, T>& other) const
{
	MatrixRow<N, T> result;
	std::transform(
		this->begin(), this->end(),
		other.begin(), result.begin(),
		[](T a, T b) {
			return a + b;
		});
	return result;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator+=(const MatrixRow<N, T>& other)
{
	std::transform(
		this->begin(), this->end(),
		other.begin(), this->begin(),
		[](T a, T b) {
			return a + b;
		});
	return *this;
}

template <std::size_t N, typename T>
std::size_t MatrixRow<N, T>::NumberLength(T value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str().length();
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator-(const MatrixRow<N, T>& other) const
{
	MatrixRow<N, T> result;
	std::transform(
		this->begin(), this->end(),
		other.begin(),
		result.begin(),
		[](T a, T b) {
			return a - b;
		});
	return result;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator-=(const MatrixRow<N, T>& other)
{
	std::transform(
		this->begin(), this->end(),
		other.begin(),
		this->begin(),
		[](T a, T b) {
			return a - b;
		});
	return *this;
}

template <std::size_t N, typename T>
MatrixRow<N, T> MatrixRow<N, T>::operator-()
{
	MatrixRow<N, T> result;
	std::transform(
		this->begin(), this->end(),
		result.begin(),
		[](T a) {
			return -a;
		});
	return result;
}
