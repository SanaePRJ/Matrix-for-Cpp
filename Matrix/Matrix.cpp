﻿/*-------------------------------------------------------------
* Name    : Matrix.cpp
* Version : 4.0.1
* Author  : SanaePRJ
* Description:
*  テストプログラム
-------------------------------------------------------------*/




#include <iostream>
#include <random>
#include <time.h>
#include "Matrix/Matrix.hpp"


int main()
{
	/*-----Constructor-----*/
	Sanae::Matrix<double> a;

	Sanae::Matrix<double> b =
	{
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};
	/*----------
		1 2 3
		4 5 6
		7 8 9
	----------*/

	Sanae::Matrix<double> c = b;
	/*----------
		1 2 3
		4 5 6
		7 8 9
	----------*/

	/*-----Operator-----*/
	//破壊的
	a = b;
	/*----------
		1 2 3
		4 5 6
		7 8 9
	----------*/

	a = {
		{4,3,8},
		{8,5,0},
		{3,5,1}
	};
	/*----------
		4 3 8
		8 5 0
		3 5 1
	----------*/

	a += b;
	/*----------
	4 3 8   1 2 3    5  5 11
	8 5 0 + 4 5 6 = 12 10  6
	3 5 1   7 8 9   10 13 10
	----------*/

	a -= b;
	/*----------
	 5  5 11   1 2 3   4 3 6
	12 10  6 - 4 5 6 = 8 5 0
	10 13 10   7 8 9   3 5 1
	----------*/

	a *= b;
	/*----------
	4 3 6   1 2 3   72 87 102
	8 5 0 * 4 5 6 = 28 41  54
	3 5 1   7 8 9   30 39  48
	----------*/

	//元に戻す
	a = b;
	/*----------
	1 2 3
	4 5 6
	7 8 9
	----------*/

	a *= 3;
	/*----------
	 3  6  9
	12 15 18
	21 24 27
	----------*/

	a ^= b; //dot mul


	//非破壊的
	(b + c);
	(b - c);
	(b * c);
	(b * 5);
	(b ^ c); //dot mul

	//参照
		/*----------
		* b:
		1 2 3
		4 5 6
		7 8 9
		----------*/
	b[1][1];         //5 : 行から取得 b[1]     = std::vector<ty >{4,5,6}
	*(b[{0, 1}][1]); //5 : 列から取得 b[{0,1}] = std::vector<ty*>{2,5,8}

/*-----Function-----*/
	Sanae::Matrix<double> func_var0 =
	{
		{7,8,2},
		{6,0,2},
		{4,2,1}
	};
	
//破壊的
	//列と列を入れ替える。
	func_var0.Swap_Column(0,1);
	/*----------
		8 7 2
		0 6 2
		2 4 1
	----------*/

	//行と行を入れ替える。
	func_var0.Swap_Row(1,2);
	/*----------
		8 7 2
		2 4 1
		0 6 2
	----------*/

//非破壊的
	//転置を行う。
	std::cout << func_var0.Transpose();
	/*
		8 2 0
		7 4 6
		2 1 2
	*/

	//行列式を求める。
	func_var0.det();
	//12

	//逆行列を求める。
	func_var0.Inverse();
	/*----------
	 0.166667 -0.166667 -0.0833333
	-0.333333  1.33333  -0.333333
	 1        -4         1.5
	----------*/

	std::cout << func_var0;

	Sanae::Matrix<double> from =
	{
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};

	Sanae::Matrix<double> moveto;
	moveto << from;

	return 0;
}
