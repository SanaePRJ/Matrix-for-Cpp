/*-------------------------------------------------------------
* Name    : Matrix.cpp
* Version : 4.0.5
* Author  : SanaePRJ
* Description:
*  テストプログラム
-------------------------------------------------------------*/




//#define SANAE_MATRIX_NOTHREADS

#include <functional>
#include <iostream>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>

#include "Test.hpp"
#include "Matrix/Matrix"




int main()
{
	using namespace Sanae;

	// 行列の初期化
	Matrix<double> Mat1
	{
		{ -1, 2 , 3 },
		{ 4 , -5, 6 },
		{ 7 , 8 , -9}
	};

	Matrix<double> Mat2
	{
		{ -1, 2 , 3 },
		{ 4 , -5, 6 },
		{ 7 , 8 , -9}
	};

	std::cout << "Matrix 1:\n" << Mat1 << "\n";
	std::cout << "Matrix 2:\n" << Mat2 << "\n";

	// 加算
	Matrix<double> Sum = Mat1 + Mat2; // += でも計算できます: Mat1 += Mat2;
	std::cout << "Sum (Matrix 1 + Matrix 2):\n" << Sum << "\n";

	// 減算
	Matrix<double> Diff = Mat1 - Mat2; // -= でも計算できます: Mat1 -= Mat2;
	std::cout << "Difference (Matrix 1 - Matrix 2):\n" << Diff << "\n";

	// 行列積
	Matrix<double> Prod = Mat1 * Mat2; // *= でも計算できます: Mat1 *= Mat2;
	std::cout << "Product (Matrix 1 * Matrix 2):\n" << Prod << "\n";

	// スカラー倍
	Matrix<double> ScalarMul = Mat1 * 2.0; // *= でも計算できます: Mat1 *= 2.0;
	std::cout << "Scalar Multiplication (Matrix 1 * 2):\n" << ScalarMul << "\n";

	// 転置
	Matrix<double> Transpose = Mat1.Transpose();
	std::cout << "Transpose of Matrix 1:\n" << Transpose << "\n";

	// 単位行列
	Matrix<double> Identity = Matrix<double>::Identity(3);
	std::cout << "Identity Matrix (3x3):\n" << Identity << "\n";


	// 行列式
	double Det = Mat1.Det();
	std::cout << "Determinant of Matrix 1: " << Det << '\n';

	FontWeight = 15;

	// 逆行列
	Matrix<double> Inv = Mat1.Inverse();
	std::cout << "Inverse of Matrix 1:\n" << Inv << "\n";
	
	FontWeight = DefaultWeight;

	MulTest();


	return 0;
}