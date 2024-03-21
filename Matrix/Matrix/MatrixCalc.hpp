/*-------------------------------------------------------------
* Name    : MatrixCalc.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  MatrixBase型の計算メソッドを実装
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXCALC_HPP
#define SANAE_MATRIXCALC_HPP

#include <algorithm>
#include "Matrix.h"




//渡された関数をすべての要素に対して実行します。arg_data1 = Function(arg_data1,arg_data2)
template<typename ty>
inline void Sanae::Matrix<ty>::m_calc(MatrixT* arg_data1,const MatrixT* arg_data2, std::function<ty(ty,ty)> Func) const
{
	//サイズが0の場合計算できない...
	if (arg_data1->size() == 0 || arg_data2->size() == 0)
		throw std::invalid_argument("You must have some data.");

	//第一第二引数の行と列数が違う場合エラー
	if (this->m_GetRowSize(arg_data1) != this->m_GetRowSize((MatrixT*)arg_data2) || this->m_GetColumnSize(arg_data1) != this->m_GetColumnSize((MatrixT*)arg_data2))
		throw std::invalid_argument("The number of rows and columns of data1 and data2 must be equal.");

	const size_t Row    = this->m_GetRowSize   (arg_data1);  //計算する行数
	const size_t Column = this->m_GetColumnSize(arg_data1);  //計算する列数

	for (size_t pos_row = 0; pos_row < Row; pos_row++)
	{
		for (size_t pos_column = 0; pos_column < Column; pos_column++)
		{
			//arg_data1に計算結果を入れていく
			(*arg_data1)[pos_row][pos_column] = Func((*arg_data1)[pos_row][pos_column], (*arg_data2)[pos_row][pos_column]);
		}
	}

	return;
}




//加算
template<typename ty>
inline void Sanae::Matrix<ty>::m_add(MatrixT* arg_data1, const MatrixT* arg_data2) const
{
	this->m_calc(arg_data1, arg_data2, [](ty d1, ty d2)->ty {return d1 + d2; });
	return;
}


//減算
template<typename ty>
inline void Sanae::Matrix<ty>::m_sub(MatrixT* arg_data1, const MatrixT* arg_data2) const
{
	this->m_calc(arg_data1, arg_data2, [](ty d1, ty d2)->ty {return d1 - d2; });
	return;
}


//dot
template<typename ty>
inline void Sanae::Matrix<ty>::m_dotmul(MatrixT* arg_data1, const MatrixT* arg_data2) const
{
	this->m_calc(arg_data1, arg_data2, [](ty d1, ty d2)->ty {return d1 * d2; });
	return;
}


//スカラー倍
template<typename ty>
inline void Sanae::Matrix<ty>::m_scalarmul(MatrixT* arg_data1, ty arg_data2) const
{
	this->m_calc(arg_data1, (const MatrixT*)arg_data1, [&arg_data2](ty d1, ty d2)->ty {return d1 * arg_data2; });

	return;
}




#ifndef SANAE_MATRIX_NOTHREADS


#include <thread>




//行列の掛け算(Threads)
template<typename ty>
inline void Sanae::Matrix<ty>::m_mul(MatrixT* arg_data1, const MatrixT* arg_data2)
{
	//第一引数の列数と第二引数の行数は同じでなければならない。
	if (this->m_GetColumnSize(arg_data1) != this->m_GetRowSize((MatrixT*)arg_data2))
		throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");


	//第一引数の行数と第二引数の列数を確保,0で初期化
	MatrixT buf(this->m_GetRowSize(arg_data1), std::vector<ty>(this->m_GetColumnSize((MatrixT*)arg_data2), 0));

	//行列サイズを取得
	const size_t Row    = this->m_GetRowSize   (&buf); //行数
	const size_t Column = this->m_GetColumnSize(&buf); //列数
	
	//計算数
	const size_t alltaskcount = Row * Column;

	if (this->thread == 0)
		this->thread = 1;

	size_t taskcount    = alltaskcount / this->thread;

	//l[i][j] = Σk=0,n (m[i][k] * n[k][j])を計算させるラムダ式
	auto mul_lambda = [arg_data1, arg_data2, this](size_t arg_Row, size_t arg_Column)
		{
			const size_t size = this->m_GetColumnSize(arg_data1);
			ty           sum  = 0;

			//Σk = 0, n(m[i][k] * n[k][j])
			for (size_t Pos = 0; Pos < size; Pos++)
				sum += (*arg_data1)[arg_Row][Pos] * (*arg_data2)[Pos][arg_Column];

			return sum;
		};

	//threadで分割するためのラムダ式
	auto mul_thread = [&Row,&Column,&buf,&mul_lambda](size_t from,size_t to) {
			for (size_t pos = from; pos < to; pos++) 
				buf[(pos / Column)][(pos % Column)] = mul_lambda((pos / Column),( pos % Column));
		};
	

	//スレッド管理
	std::vector<std::thread> threads;
	for (size_t pos = 0; pos < alltaskcount;)
	{
		size_t from = pos;
		size_t to   = pos + taskcount;
		
		if (to > alltaskcount)
			to = alltaskcount;

		if (from == to)
			break;

		//スレッドを作成
		threads.push_back(std::thread(mul_thread,from,to));
		pos = to;
	}

	//タスク完了まで待つ
	for (std::thread& th:threads)
		th.join();

	arg_data1->clear();

	//bufを第一引数に譲渡
	*arg_data1 = std::move(buf);


	return;
}




#else




//行列の掛け算(NoThreads)
template<typename ty>
inline void Sanae::Matrix<ty>::m_mul(MatrixT* arg_data1, const MatrixT* arg_data2) const
{
	//第一引数の列数と第二引数の行数は同じでなければならない。
	if (this->m_GetColumnSize(arg_data1) != this->m_GetRowSize((MatrixT*)arg_data2))
		throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");

	//第一引数の行数と第二引数の列数を確保,0で初期化
	MatrixT buf(this->m_GetRowSize(arg_data1), std::vector<ty>(this->m_GetColumnSize((MatrixT*)arg_data2), 0));

	const size_t Row    = this->m_GetRowSize(&buf);    //行数
	const size_t Column = this->m_GetColumnSize(&buf); //列数

	//l[i][j] = Σk=0,n (m[i][k] * n[k][j])を計算させるラムダ式
	auto mul_lambda = [arg_data1, arg_data2, this](size_t arg_Row, size_t arg_Column)
		{

			const size_t size = this->m_GetColumnSize(arg_data1);
			ty           sum = 0;

			for (size_t Pos = 0; Pos < size; Pos++)
				sum += (*arg_data1)[arg_Row][Pos] * (*arg_data2)[Pos][arg_Column];

			return sum;
		};

	for (size_t pos_row = 0; pos_row < Row; pos_row++)
	{
		for (size_t pos_column = 0; pos_column < Column; pos_column++)
		{
			buf[pos_row][pos_column] = mul_lambda(pos_row, pos_column);
		}
	}

	arg_data1->clear();
	//bufを第一引数に譲渡
	std::move(buf.begin(), buf.end(), std::back_inserter(*arg_data1));

	return;
}




#endif




#endif