/*-------------------------------------------------------------
* Name    : MatrixConstructor.hpp
* Version : Beta
* * Author: SanaePRJ
* Description:
*  MatrixBase型のコンストラクタの実装
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXCONSTRUCTOR_HPP
#define SANAE_MATRIXCONSTRUCTOR_HPP

#include "Matrix.h"




//コンストラクタ
template<typename ty>
Sanae::Matrix<ty>::Matrix() {}


/*------------------------------------------
{
	{0, 0},
	{0, 0},
	{0, 0}
}
3 * 2行列のように定義することができる。
------------------------------------------*/
template<typename ty>
Sanae::Matrix<ty>::Matrix(MatrixInitT arg_InitValue)
	:matrix(arg_InitValue.begin(), arg_InitValue.end())
{
	//列数は等しくなければならない。
	if (this->m_CheckColumn(&this->matrix))
		throw std::invalid_argument("All the columns must be equal.");
}


//std::vector<std::vector<ty>>で入れられた時用
template<typename ty>
Sanae::Matrix<ty>::Matrix(MatrixT& arg_InitValue)
	:matrix(arg_InitValue.begin(), arg_InitValue.end())
{
	//列数は等しくなければならない。
	if (this->m_CheckColumn(&this->matrix))
		throw std::invalid_argument("All the columns must be equal.");
}


//コピーコンストラクタ
template<typename ty>
Sanae::Matrix<ty>::Matrix(const Matrix& arg)
{
	std::move(arg.matrix.begin(), arg.matrix.end(), std::back_inserter(matrix));
}




#endif