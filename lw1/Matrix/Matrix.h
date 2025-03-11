#pragma once

#include "MatrixRow.h"
#include <array>
template <std::size_t M, std::size_t N, typename T>
class Matrix : public std::array<MatrixRow<N, T>, M>
{
public:
	Matrix();
	Matrix(const std::initializer_list<std::initializer_list<T>>& mtx);
	Matrix<M, N, T> operator*(T);
	Matrix<M, N, T> operator*=(T);
	template <std::size_t P>
	Matrix<M, P, T> operator*(const Matrix<N, P, T>&) const;
	Matrix<M, N, T> operator/(T);
	Matrix<M, N, T> operator/=(T);
	Matrix<M, N, T> operator+(const Matrix<M, N, T>&);
	Matrix<M, N, T> operator+=(const Matrix<M, N, T>&);
	Matrix<M, N, T> operator-(const Matrix<M, N, T>&);
	Matrix<M, N, T> operator-=(const Matrix<M, N, T>&);
	Matrix<N, M, T> operator^(int p) const;
	Matrix<N, M, T> operator^=(int p);
	Matrix<M - 1, N - 1, T> MinorMatrix(std::size_t i, std::size_t j) const;
	T Minor(std::size_t i, std::size_t j) const;
	T Determinant() const;
	T DeterminantByPermutation() const;
	T AlgebraicAddition(std::size_t i, std::size_t j) const;
	Matrix<N, M, T> Transposed() const;
	static Matrix<M, N, T> IdentityMatrix();
	std::function<std::ostream&(std::ostream&)> stringify(int columnWidth);
	std::ostream& stringify(std::ostream& os, int columnWidth);
	Matrix<M, N, double> UpperTriangularForm() const;
	Matrix<M, N, T> AdjointMatrix() const;
	Matrix<N, M, T> InvertedMatrix() const;
	template <typename TT>
	operator Matrix<M, N, TT>(); // NOLINT

private:
	template <std::size_t P>
	T GetElementMultiplicationResult(const Matrix<N, P, T>&, std::size_t i, std::size_t j) const;
	T CalculateDeterminant() const;
	T CalculateDeterminantByPermutations() const;
};

#include "Matrix.tpp"
