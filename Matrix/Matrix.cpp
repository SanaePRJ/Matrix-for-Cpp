/*-------------------------------------------------------------
* Name    : Matrix.cpp
* Version : 4.0.4
* Author  : SanaePRJ
* Description:
*  テストプログラム
-------------------------------------------------------------*/


#include <functional>
#include <iostream>
#include <random>
#include <time.h>
#include <thread>


#include "Matrix/Matrix.hpp"




//行列積高速化テストプログラム
int mul()
{
	std::default_random_engine       engine;      //エンジン
	std::normal_distribution<double> dist(0, 1);  //平均0,標準偏差1

	size_t n = 900;

	//n*n行列を作成
	Sanae::Matrix<double> buf0 = std::pair<size_t, size_t>{ n,n };
	Sanae::Matrix<double> buf1 = std::pair<size_t, size_t>{ n,n };

	//乱数をセット
	buf0.Setter([&engine, &dist]() -> double {return dist(engine); });
	buf1.Setter([&engine, &dist]() -> double {return dist(engine); });

	//行列積計算用スレッドを設定
	buf0.thread = std::thread::hardware_concurrency();

	system("pause");

	//タイム計測スタート
	time_t now = time(NULL);

	buf0 *= buf1;

	//終了
	time_t past = time(NULL) - now;

	std::cout << past << "秒かかりました。" << std::endl;

	system("pause");
	//std::cout << buf0; //計算結果

	return 0;
}




int main()
{
	mul();

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

	//比較演算子
	if (b == c)
		std::cout << "Same" << std::endl;

	else if (c != c)
		std::cout << "not Same" << std::endl;

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

	Sanae::Matrix<double> test = b * c;

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

	//行列の値を行数,列数,元の値によって変更する。
	Sanae::Matrix<double> test2 = std::pair<size_t, size_t>{3,3};
	test2.Setter([](size_t row, size_t column, double& value)->double {return row + column + value; });
	std::cout << test2;

	return 0;
}