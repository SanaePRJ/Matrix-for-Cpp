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

#include "Matrix/Matrix"




//行列積高速化テストプログラム
/*
* 
* 環境:Core i5 8250U,DDR4(2400Mhz)
* 実験結果:
* 
計算回数  :高速化率が最も高いスレッド数
1000000   :3,4,5,6スレッド
125000000 :8スレッド
343000000 :7スレッド
1000000000:4スレッド
3375000000:4スレッド
8000000000:4スレッド
*/
int mul()
{
	using namespace std::chrono;

	std::default_random_engine       engine;      //エンジン
	std::normal_distribution<double> dist(0, 1);  //平均0,標準偏差1

	size_t n = 2500;

	//n*n行列を作成
	Sanae::Matrix<double> buf0 = std::pair<size_t, size_t>{ n,n };
	Sanae::Matrix<double> buf1 = std::pair<size_t, size_t>{ n,n };

	buf0.Setter([&engine, &dist]()->double {return dist(engine); });
	buf1.Setter([&engine, &dist]()->double {return dist(engine); });

	auto calc = [&buf0,&buf1,&engine,&dist]() {
		auto buf_time0 = system_clock::now();

		buf0 * buf1;

		auto buf_time1 = system_clock::now();

		return duration_cast<milliseconds>(buf_time1 - buf_time0).count();
	};

	std::cout << n * n * n << "回計算されます。" << std::endl;

	//1スレッドでの計算
	buf0.thread = 1;
	std::cout << buf0.thread << "スレッドでの計算:";
	double time1 = static_cast<double>(calc());
	std::cout << time1 << "ミリ秒かかりました。" << std::endl;

	//複数スレッドでの計算
	for (size_t i = 2; i <= std::thread::hardware_concurrency();i++) {
		buf0.thread = i;

		std::cout << buf0.thread << "スレッドでの計算:";
		double time2 = static_cast<double>(calc());
		std::cout << time2 << "ミリ秒かかりました。->";

		std::cout << (1 - time2 / time1) * 100 << "%高速化できました。" << std::endl;
	}
	system("pause");


	return 0;
}

int main()
{
	//高速化テスト
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

	else if (b != c)
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
	//*(b[{0, 1}][1]); //5 : 列から取得 b[{0,1}] = std::vector<ty*>{2,5,8}  非推奨4.0.5~

/*-----Function-----*/
	Sanae::Matrix<double> func_var0 =
	{
		{7,8,2},
		{6,0,2},
		{4,2,1}
	};
	
//破壊的
	//列と列を入れ替える。
	func_var0.SwapColumn(0,1);
	/*----------
		8 7 2
		0 6 2
		2 4 1
	----------*/

	//行と行を入れ替える。
	func_var0.SwapRow(1,2);
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
	func_var0.Det();
	//12

	//逆行列を求める。
	std::cout<<func_var0.Inverse();
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

	//3*3単位行列
	test2 = Sanae::Matrix<double>::Identity(3);

	//3*3のゼロ行列
	test2 = Sanae::Matrix<double>::Zero(3);

	return 0;
}