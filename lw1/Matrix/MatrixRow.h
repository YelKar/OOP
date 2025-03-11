#pragma once

#include <array>
#include <format>
#include <functional>
#include <ostream>

template <std::size_t N, typename T>
class MatrixRow : public std::array<T, N>
{
public:
	MatrixRow<N, T> operator*(T);
	MatrixRow<N, T> operator*=(T);
	MatrixRow<N, T> operator/(T);
	MatrixRow<N, T> operator/=(T);
	MatrixRow<N, T> operator+(const MatrixRow<N, T>&) const;
	MatrixRow<N, T> operator+=(const MatrixRow<N, T>&);
	MatrixRow<N, T> operator-(const MatrixRow<N, T>&) const;
	MatrixRow<N, T> operator-=(const MatrixRow<N, T>&);
	MatrixRow<N, T> operator-();
	template <std::size_t N_, typename T_>
	friend std::ostream& operator<<(std::ostream&, MatrixRow<N_, T_>);
	std::function<std::ostream&(std::ostream&)> stringify(int columnWidth);
	std::ostream& stringify(std::ostream& os, int columnWidth);

private:
	static std::size_t NumberLength(T value);
};

std::ostream& operator<<(std::ostream& os, const std::function<std::ostream&(std::ostream&)>& manip);

#include "MatrixRow.tpp"
