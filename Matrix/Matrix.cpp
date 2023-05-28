#include <iostream>
#include "Matrix.h"

int main()
{
	Matrix<double> a =
	{
		{3,1,1,2},
		{5,1,3,4},
		{2,0,1,0},
		{1,3,2,1}
	};
	printf("逆数列。\n");
	a.inverse().View();
	printf("\n通常\n");
	a.View();
}