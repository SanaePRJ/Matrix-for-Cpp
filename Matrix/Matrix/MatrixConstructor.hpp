/*-------------------------------------------------------------
* Name    : MatrixConstructor.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  MatrixBase型のコンストラクタの実装
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIXCONSTRUCTOR_HPP_
#define _INCLUDEGUARD_MATRIXCONSTRUCTOR_HPP_


#include <algorithm>
#include "Matrix.h"




//コンストラクタ
template<typename ty>
Sanae::Matrix<ty>::Matrix() {}


template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	std::pair<size_t,size_t> ArgSize
) 
	:matrix(ArgSize.first, std::vector<ty>(ArgSize.second, 0))
{}


/*------------------------------------------
{
	{0, 0},
	{0, 0},
	{0, 0}
}
3 * 2行列のように定義することができる。
------------------------------------------*/
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	MatrixInit_t ArgInitValue
)
	:matrix(ArgInitValue.begin(), ArgInitValue.end())
{
	//列数は等しくなければならない。
	this->m_ValidateMatrix(this->matrix);
}


//std::vector<std::vector<ty>>で入れられた時用
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	const Matrix_t& ArgInitValue
)
	:matrix(ArgInitValue.begin(), ArgInitValue.end())
{
	//列数は等しくなければならない。
	this->m_ValidateMatrix(this->matrix);
}


//コピーコンストラクタ
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	const Matrix& ArgMatrix
)
{
	//サイズ設定
	matrix.resize(ArgMatrix.matrix.size());
	//コピー
	std::copy(ArgMatrix.matrix.begin(),ArgMatrix.matrix.end(),matrix.begin());
}


//デストラクタ
template<typename ty>
inline Sanae::Matrix<ty>::~Matrix()
{
}




#endif