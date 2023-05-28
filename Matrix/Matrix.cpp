#include <iostream>
#include "Matrix.h"

int main()
{
	Matrix<double> a =
	{
		{3,1,3},
		{4,3,6},
		{9,1,4}
	};
	(a * a.Inverse()).View();

	a.Sigmoid().View();
}