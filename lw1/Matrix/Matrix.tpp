#pragma once
#include <cassert>

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T>::Matrix() = default;

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T>::Matrix(const std::initializer_list<std::initializer_list<T>>& mtx)
{
	if (mtx.size() != M)
	{
		throw std::invalid_argument(std::format(
			"Invalid initialized matrix size: "
			"more rows are provided ({}) than specified in declaration ({})",
			mtx.size(),
			M));
	}
	for (
		auto [initRow, row] = std::tuple{ mtx.begin(), this->begin() };
		initRow != mtx.end();
		initRow++, row++)
	{
		if (initRow->size() != N)
		{
			throw std::invalid_argument(std::format(
				"Invalid initialized matrix size: "
				"more elements in row are provided ({}) than specified in declaration ({})",
				initRow->size(),
				N));
		}
		std::copy(initRow->begin(), initRow->end(), row->begin());
	}
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator*(T multiplier)
{
	Matrix<M, N, T> result = *this;
	for (MatrixRow<N, T>& elt : result)
	{
		elt *= multiplier;
	}
	return result;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator*=(T multiplier)
{
	for (MatrixRow<N, T>& elt : *this)
	{
		elt *= multiplier;
	}
	return *this;
}

template <std::size_t M, std::size_t N, typename T>
template <std::size_t P>
Matrix<M, P, T> Matrix<M, N, T>::operator*(const Matrix<N, P, T>& other) const
{
	Matrix<M, P, T> result;
	for (std::size_t i = 0; i < M; i++)
	{
		for (std::size_t j = 0; j < P; j++)
		{
			result[i][j] = this->GetElementMultiplicationResult(other, i, j);
		}
	}

	return result;
}

template <std::size_t M, std::size_t N, typename T>
template <std::size_t P>
T Matrix<M, N, T>::GetElementMultiplicationResult(const Matrix<N, P, T>& other, std::size_t i, std::size_t j) const
{
	if (i >= M || j >= P)
	{
		throw std::invalid_argument(std::format(
			"coordinates ({}; {}) must be less than result matrix size ({}; {})",
			i, j,
			M, P));
	}
	T result = 0;
	MatrixRow<N, T> row = this->operator[](i);
	for (auto [elt1, elt2] = std::tuple{ row.begin(), other.begin() }; elt1 != row.end(); elt1++, elt2++)
	{
		result += *elt1 * elt2->operator[](j);
	}
	return result;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator/(T divider)
{
	Matrix<M, N, T> result = *this;
	for (MatrixRow<N, T>& elt : result)
	{
		elt /= divider;
	}
	return result;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator/=(T divider)
{
	for (MatrixRow<N, T>& elt : *this)
	{
		elt /= divider;
	}
	return *this;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator+(const Matrix<M, N, T>& other)
{
	Matrix<M, N, T> result;
	std::transform(
		this->begin(), this->end(),
		other.begin(), result.begin(),
		[](const MatrixRow<N, T>& a, const MatrixRow<N, T>& b) {
			return a + b;
		});
	return result;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator+=(const Matrix<M, N, T>& other)
{
	for (std::size_t i = 0; i < M; i++)
	{
		this->operator[](i) += other[i];
	}
	return *this;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator-(const Matrix<M, N, T>& other)
{
	Matrix<M, N, T> result;
	std::transform(
		this->begin(), this->end(),
		other.begin(), result.begin(),
		[](MatrixRow<N, T>& a, MatrixRow<N, T>& b) {
			return a - b;
		});
	return result;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::operator-=(const Matrix<M, N, T>& other)
{
	for (std::size_t i = 0; i < M; i++)
	{
		this->operator[](i) -= other[i];
	}
	return *this;
}

template <std::size_t M, std::size_t N, typename T>
template <typename TT>
Matrix<M, N, T>::operator Matrix<M, N, TT>()
{
	Matrix<M, N, TT> result;
	for (std::size_t i = 0; i < M; i++)
	{
		for (std::size_t j = 0; j < N; j++)
		{
			result[i][j] = static_cast<TT>((*this)[i][j]);
		}
	}
	return result;
}

template <std::size_t M, std::size_t N, typename T>
T Matrix<M, N, T>::DeterminantByPermutation() const
{
	if (N < 1 || M < 1)
	{
		throw std::invalid_argument("Size must be positive");
	}
	if (N != M)
	{
		throw std::invalid_argument("Not square matrix has no determinant");
	}
	return CalculateDeterminantByPermutations();
}

template <std::size_t M, std::size_t N, typename T>
T Matrix<M, N, T>::CalculateDeterminantByPermutations() const
{
	const std::size_t size = M;
	auto matrix = *this;

	if (size == 1)
	{
		return matrix[0][0];
	}

	if (size == 2)
	{
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}

	std::vector<std::size_t> permutation(size);
	for (std::size_t i = 0; i < size; ++i)
	{
		permutation[i] = i;
	}

	T determinant = 0;

	do
	{
		T product = 1;
		int inversions = 0;

		for (std::size_t i = 0; i < size; ++i)
		{
			product *= matrix[i][permutation[i]];
			for (std::size_t j = 0; j < i; ++j)
			{
				if (permutation[j] > permutation[i])
				{
					inversions++;
				}
			}
		}

		if (inversions % 2 == 0)
		{
			determinant += product;
		}
		else
		{
			determinant -= product;
		}

	} while (std::next_permutation(permutation.begin(), permutation.end()));

	return determinant;
}

template <std::size_t M, std::size_t N, typename T>
T Matrix<M, N, T>::Determinant() const
{
	if (N < 1 || M < 1)
	{
		throw std::invalid_argument("Size must be positive");
	}
	if (N != M)
	{
		throw std::invalid_argument("Not square matrix has no determinant");
	}
	return CalculateDeterminantByPermutations();
}

template <std::size_t M, std::size_t N, typename T>
T Matrix<M, N, T>::CalculateDeterminant() const
{
	double determinant = 1;
	auto upperTriangularMtx = UpperTriangularForm();

	for (int i = 0; i < M; i++)
	{
		determinant *= upperTriangularMtx[i][i];
	}

	return static_cast<T>(determinant);
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, double> Matrix<M, N, T>::UpperTriangularForm() const
{
	Matrix<M, N, double> matrix;

	for (int i = 0; auto& row : matrix)
	{
		for (int j = 0; auto& elt : row)
		{
			elt = static_cast<double>((*this)[i][j]);
			j++;
		}
		i++;
	}
	int n = M;

	for (auto& row : matrix)
	{
		for (double& elt : row)
		{
			elt = std::round(elt * std::pow(10, 10)) / std::pow(10, 10);
		}
	}
	return matrix;
}

template <std::size_t M, std::size_t N, typename T>
T Matrix<M, N, T>::AlgebraicAddition(std::size_t i, std::size_t j) const
{
	if (N < 1 || M < 1)
	{
		throw std::invalid_argument("Size must be positive");
	}
	return Minor(i, j) * static_cast<T>(std::pow(-1, i + j));
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M - 1, N - 1, T> Matrix<M, N, T>::MinorMatrix(std::size_t i_, std::size_t j_) const
{
	if (N < 1 || M < 1)
	{
		throw std::invalid_argument("Size must be positive");
	}
	Matrix<M - 1, N - 1, T> result;
	for (std::size_t i = 0; auto& resultRow : result)
	{
		for (std::size_t j = 0; auto& elt : resultRow)
		{
			elt = (*this)[i + (i >= i_)][j + (j >= j_)];
			j++;
		}
		i++;
	}
	return result;
};

template <std::size_t M, std::size_t N, typename T>
T Matrix<M, N, T>::Minor(std::size_t i, std::size_t j) const
{
	return MinorMatrix(i, j).DeterminantByPermutation();
}

template <std::size_t M, std::size_t N, typename T>
Matrix<N, M, T> Matrix<M, N, T>::Transposed() const
{
	Matrix<N, M, T> result;

	for (std::size_t j = 0; auto& row : result)
	{
		for (std::size_t i = 0; auto& elt : result)
		{
			result[j][i] = (*this)[i][j];
			i++;
		}
		j++;
	}

	return result;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<N, M, T> Matrix<M, N, T>::operator^(int p) const
{
	if (N != M)
	{
		throw std::invalid_argument("Non-square matrix cannot be raised to a power");
	}
	if (p == 0)
	{
		return IdentityMatrix();
	}
	Matrix<M, N, T> matrix = p > 0 ? *this : InvertedMatrix();
	Matrix<M, N, T> multiplier = matrix;
	for (int i = 1; i < std::abs(p); i++)
	{
		matrix = matrix * multiplier;
	}
	return matrix;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<N, M, T> Matrix<M, N, T>::operator^=(int p)
{
	return Matrix<N, M, T>();
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::AdjointMatrix() const
{
	Matrix<M, N, T> result;

	for (std::size_t i = 0; auto& row : result)
	{
		for (std::size_t j = 0; auto& elt : result)
		{
			result[i][j] = AlgebraicAddition(i, j);
			j++;
		}
		i++;
	}

	return result;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<N, M, T> Matrix<M, N, T>::InvertedMatrix() const
{
	return AdjointMatrix().Transposed() / DeterminantByPermutation();
}

template <std::size_t M, std::size_t N, typename T>
std::function<std::ostream&(std::ostream&)> Matrix<M, N, T>::stringify(int columnWidth)
{
	return [this, columnWidth](std::ostream& os) -> std::ostream& {
		return stringify(os, columnWidth);
	};
	;
}

template <std::size_t M, std::size_t N, typename T>
std::ostream& Matrix<M, N, T>::stringify(std::ostream& os, int columnWidth)
{
	for (MatrixRow<N, T>& row : *this)
	{
		row.stringify(os, columnWidth) << std::endl;
	}
	return os;
}

template <std::size_t M, std::size_t N, typename T>
Matrix<M, N, T> Matrix<M, N, T>::IdentityMatrix()
{
	Matrix<M, N, T> result;
	if (N != M)
	{
		throw std::invalid_argument("The identity matrix must be square");
	}
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			result[i][j] = T(i == j);
		}
	}
	return result;
}
