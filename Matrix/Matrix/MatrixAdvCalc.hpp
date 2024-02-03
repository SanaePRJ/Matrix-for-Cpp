/*-------------------------------------------------------------
* Name    : MatrixDet.hpp
* Version : 4.0.2
* * Author: SanaePRJ
* Description:
*  MatrixBase型の行列式と逆行列を求める関数の実装
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXADV_HPP
#define SANAE_MATRIXADV_HPP

#include "Matrix.h"




//掃き出し法
template<typename ty>
inline void Sanae::Matrix<ty>::m_sweepout(MatrixT* arg_from, MatrixT* arg_store)
{
	if (m_GetRowSize(arg_from) != m_GetColumnSize(arg_from))
		throw std::invalid_argument("It must be a square matrix.");

	size_t size = m_GetColumnSize(arg_from);

	//確保
	arg_store->resize(size, std::vector<ty>(size, 0));
	//単位行列へ
	this->m_to_identity(arg_store);

	//ある行の定数倍をほかの行へ加算する。
	auto Operation = [arg_from, arg_store, this, size](size_t from, size_t to, ty num)
		{
			for (size_t Column = 0; Column < m_GetColumnSize(arg_from); Column++) {
				(*arg_from )[to][Column] += (*arg_from )[from][Column] * num;
				(*arg_store)[to][Column] += (*arg_store)[from][Column] * num;
			}
		};

	for (size_t Column = 0; Column < size; Column++) {
		for (size_t Row = 0; Row < size; Row++) {
			//定数
			ty num = 0;

			if (Row == Column)
				num = (1 - 1 * (*arg_from)[Row][Column]) / (*arg_from)[Column][Column]; //対角成分を1にする。
			else
				num = -1 * (*arg_from)[Row][Column] / (*arg_from)[Column][Column];      //他の成分を0にする。

			//ある行の定数倍をほかの行へ加算する。
			Operation(Column, Row, num);
		}
	}
}


//逆行列を求める。
template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::Inverse() 
{
	//列数は等しくなければならない。
	if (this->m_CheckColumn(&this->matrix))
		throw std::invalid_argument("All the columns must be equal.");

	if (this->det() == 0)
		throw std::invalid_argument("It is not a regular matrix.");

	MatrixT buf;
	std::copy(this->matrix.begin(),this->matrix.end(),std::back_inserter(buf));

	MatrixT Inv;
	this->m_sweepout(&buf,&Inv);
	
	return Inv;
}


//行列式:ライプニッツの行列式
template<typename ty>
inline ty Sanae::Matrix<ty>::m_det(MatrixT* arg)
{
	if (this->get_row() != this->get_column())
		throw std::invalid_argument("It must be a square matrix.");

	//サラスの方式で解きます。
	auto det_2 = [](MatrixT& arg_lambda)
		{
			return arg_lambda[0][0] * arg_lambda[1][1] - arg_lambda[1][0] * arg_lambda[0][1];
		};

	//[Pos][0]-[0][Pos]を含まない行列を抜き取ります。
	auto pull = [](MatrixT& arg_lambda, MatrixT& storage, size_t Pos)
		{
			const size_t size = arg_lambda.size();

			for (size_t row = 0, storage_row = 0; row < size; row++) {
				//0の列を基準としている。
				for (size_t column = 1; column < size; column++) {
					if (row != Pos)
						storage[storage_row][column - 1] = arg_lambda[row][column];
				}
				//基準の高さ以外の場合は次へ
				if (Pos != row)
					storage_row++;
			}

			return;
		};

	//次元を落としまくる。
	auto dec_dim = [this, pull, det_2](MatrixT& arg_lambda, ty coeff, auto f)
		{
			const size_t size = arg_lambda.size(); //元のサイズ
			const size_t new_size = size - 1;          //落とした後のサイズ

			ty ret = 0; //返り値

			//2*2行列にまで落ちたらサラスの方式で解いて返す。
			if (size == 2)
				return coeff * det_2(arg_lambda);

			//[0][0]~[n][0]まで
			for (size_t Pos = 0; Pos < size; Pos++)
			{
				MatrixT buf(new_size, std::vector<ty>(new_size, 0)); //格納用
				pull(arg_lambda, buf, Pos);                          //縮小した行列を取得

				ty coeff_buf = arg_lambda[Pos][0] * (Pos % 2 == 0 ? 1 : -1); //新しい係数

				ret += f(buf, coeff_buf * coeff, f); //再帰させ結果をすべて加算
			}

			return ret;
		};

	return dec_dim(*arg, 1, dec_dim);
}


//行列式を求める。
template<typename ty>
ty Sanae::Matrix<ty>::det() 
{
	//列数は等しくなければならない。
	if (this->m_CheckColumn(&this->matrix))
		throw std::invalid_argument("All the columns must be equal.");

	return this->m_det(&this->matrix);
}




#endif