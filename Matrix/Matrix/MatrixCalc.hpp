/*-------------------------------------------------------------
* Name    : MatrixCalc.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  MatrixBase型の計算メソッドを実装
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIXCALC_HPP_
#define _INCLUDEGUARD_MATRIXCALC_HPP_




#include <algorithm>
#include <type_traits>
#include "Matrix.h"




//渡された関数をすべての要素に対して実行します。arg_data1 = Function(arg_data1,arg_data2)
template<typename ty>
template<typename FuncType>
inline void Sanae::Matrix<ty>::m_Calc
(
	Matrix_t&       ArgMatrix1,
	const Matrix_t& ArgMatrix2,

	FuncType Func
)
	const
{
	//サイズが0の場合計算できない...
	if (m_IsEmpty(ArgMatrix1) || m_IsEmpty(ArgMatrix2))
		throw InvalidMatrix("You must have some data.");

	//第一第二引数の行と列数が違う場合エラー
	if (!m_IsSameSize(ArgMatrix1,ArgMatrix2))
		throw InvalidMatrix("The number of rows and columns of data1 and data2 must be equal.");

	const size_t Row    = this->m_GetRowSize   (ArgMatrix1);  //計算する行数
	const size_t Column = this->m_GetColumnSize(ArgMatrix1);  //計算する列数

	//全要素に対して処理を行う
	for (size_t PosRow = 0; PosRow < Row; PosRow++)
	{
		for (size_t PosColumn = 0; PosColumn < Column; PosColumn++)
		{
			//arg_data1に計算結果を入れていく
			ArgMatrix1[PosRow][PosColumn] = Func(ArgMatrix1[PosRow][PosColumn], ArgMatrix2[PosRow][PosColumn]);
		}
	}

	return;
}


//加算
template<typename ty>
inline void Sanae::Matrix<ty>::m_Add
(
	Matrix_t&       ArgData1, 
	const Matrix_t& ArgData2
) 
	const
{
	this->m_Calc(ArgData1, ArgData2, std::plus<ty>());
	return;
}


//減算
template<typename ty>
inline void Sanae::Matrix<ty>::m_Sub
(
	Matrix_t&       ArgData1, 
	const Matrix_t& ArgData2
)
	const
{
	this->m_Calc(ArgData1, ArgData2, std::minus<ty>());
	return;
}


//アダマール積を行う。
template<typename ty>
inline void Sanae::Matrix<ty>::m_HadamardMul
(
	Matrix_t&       ArgData1, 
	const Matrix_t& ArgData2
)
	const
{
	this->m_Calc(ArgData1, ArgData2, std::multiplies<ty>());
	return;
}


//スカラー倍
template<typename ty>
inline void Sanae::Matrix<ty>::m_ScalarMul
(
	Matrix_t& ArgData1, 
	ty        ArgData2
)
	const
{
	this->m_Calc(ArgData1, ArgData1, [&ArgData2](ty d1, ty d2)->ty {return d1 * ArgData2; });

	return;
}




//マルチスレッドで動作するので thread を include しておく
#ifndef SANAE_MATRIX_NOTHREADS
	#include <thread>
#endif




//行列の掛け算(Threads)
template<typename ty>
inline void Sanae::Matrix<ty>::m_Mul
(
	Matrix_t&       ArgData1,
	const Matrix_t& ArgData2
)
{
	//第一引数の列数と第二引数の行数は同じでなければならない。
	if (m_GetColumnSize(ArgData1) != m_GetRowSize(ArgData2))
		throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");


	//第一引数の行数と第二引数の列数を確保,0で初期化
	Matrix_t Result(this->m_GetRowSize(ArgData1), Row_t<ty>(m_GetColumnSize(ArgData2), 0));

	//行列サイズを取得
	const size_t Row    = this->m_GetRowSize   (Result); //行数
	const size_t Column = this->m_GetColumnSize(Result); //列数
	
	//計算するタスク数を求める。
	const size_t AllTaskCount = Row * Column;

	//最低1スレッドを使う。
	if (this->thread == 0)
		this->thread = 1;

	//1スレッド当たりのタスクを求める。
	size_t OneTaskCount    = AllTaskCount / this->thread;

	//l[i][j] = Σk=0,n (m[i][k] * n[k][j])を計算させるラムダ式
	auto MulLambda = [ArgData1, ArgData2, this](size_t arg_Row, size_t arg_Column)
		{
			const size_t Size = m_GetColumnSize(ArgData1);
			ty           Sum  = 0;

			//Σk = 0, n(m[i][k] * n[k][j])
			for (size_t Pos = 0; Pos < Size; Pos++)
				Sum += ArgData1[arg_Row][Pos] * ArgData2[Pos][arg_Column];

			return Sum;
		};

//マルチスレッドで計算させる。
#ifndef SANAE_MATRIX_NOTHREADS

	//threadで分割するためのラムダ式
	auto MulThread = [&Row,&Column,&Result,&MulLambda](size_t from,size_t to) {
			for (size_t pos = from; pos < to; pos++) 
				Result[(pos / Column)][(pos % Column)] = MulLambda((pos / Column),( pos % Column));
		};
	

	//スレッド管理
	std::vector<std::thread> Threads;
	for (size_t Pos = 0; Pos < AllTaskCount;)
	{
		size_t from = Pos;
		size_t to   = Pos + OneTaskCount;
		
		//タスク数が超えた場合
		if (to > AllTaskCount)
			to = AllTaskCount;

		//タスクがもうない
		if (from == to)
			break;

		//スレッドを作成
		Threads.push_back(std::thread(MulThread,from,to));
		Pos = to;
	}

	//タスク完了まで待つ
	for (std::thread& th:Threads)
		th.join();

//単独のスレッドで計算させる。
#else

	//単独スレッドで計算させる。
	for (size_t PosRow = 0; PosRow < Row; PosRow++)
	{
		for (size_t PosColumn = 0; PosColumn < Column; PosColumn++)
		{
			Result[PosRow][PosColumn] = MulLambda(PosRow, PosColumn);
		}
	}

#endif

	ArgData1.clear();

	//bufを第一引数に譲渡
	ArgData1 = std::move(Result);

	return;
}




#endif