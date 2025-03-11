#include "Matrix.h"
#include <iostream>

int main()
{
	Matrix<3, 3, double> mtx{
		{ 1, 10, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};

	std::cout << ((Matrix<3, 3, int>)((mtx ^ -1) * mtx)).stringify(1) << std::endl;
}
