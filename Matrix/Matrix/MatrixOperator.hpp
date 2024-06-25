/* ---------------------------------------------------------------------------------------------
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 --------------------------------------------------------------------------------------------- */




#ifndef _INCLUDEGUARD_MATRIXOPERATOR_HPP_
#define _INCLUDEGUARD_MATRIXOPERATOR_HPP_




#include <algorithm>
#include "Matrix.h"




//以下のように定義することができる。
//{
//	{0, 0},
//	{0, 0},
//	{0, 0}
//}
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =
(
	MatrixInit_t ArgInitValue
)
{
	//std::initializer_list<std::initialize_list<ty>>をstd::vector<std::vector<ty>>へ
	this->matrix = { ArgInitValue.begin(),ArgInitValue.end() };

	//列数は等しくなければならない。
	this->m_ValidateMatrix(this->matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =
(
	const Matrix& Arg
)
{
	//サイズ変更
	this->matrix.resize(Arg.matrix.size());
	//コピー
	std::copy(Arg.matrix.begin(), Arg.matrix.end(), this->matrix.begin());

	//列数は等しくなければならない。
	this->m_ValidateMatrix(this->matrix);

	return *this;
}


//move用
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =(Matrix<ty>&& arg) = default;


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator +=
(
	const Matrix<ty>& Arg
) 
{
	this->m_Add(this->matrix, Arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator -=
(
	const Matrix<ty>& Arg
)
{
	this->m_Sub(this->matrix, Arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator ^=
(
	const Matrix<ty>& Arg
)
{
	this->m_HadamardMul(this->matrix, Arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=
(
	const Matrix<ty>& Arg
)
{
	this->m_Mul(this->matrix, Arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=
(
	ty Arg
)
{
	this->m_ScalarMul(this->matrix,Arg);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator +
(
	const Matrix<ty>& Arg
)
{
	Matrix_t buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_Add(buf, Arg.matrix);

	return buf;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator -
(
	const Matrix<ty>& Arg
)
{
	Matrix_t Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

	this->m_Sub(Result, Arg.matrix);

	return Result;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator ^
(
	const Matrix<ty>& Arg
)
{
	Matrix_t Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

	this->m_HadamardMul(Result, Arg.matrix);

	return Result;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *
(
	const Matrix<ty>& Arg
)
{
	Matrix_t Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

	this->m_Mul(Result, Arg.matrix);

	return Result;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *
(
	ty Arg
)
{
	Matrix_t Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

	this->m_ScalarMul(Result, Arg);

	return Result;
}


//譲渡
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator<<
(
	Matrix<ty>& Arg
)
{
	//データ move
	this->matrix = std::move(Arg.matrix);

	return *this;
}


//行での配列を取得します。
template<typename ty>
inline std::vector<ty>& Sanae::Matrix<ty>::operator []
(
	size_t ArgPos
)
{
	return this->matrix[ArgPos];
}


template<typename ty>
inline bool Sanae::Matrix<ty>::operator==
(
	const Matrix<ty>& ArgMatrix
)
{
	return (this->matrix == ArgMatrix.matrix);
}


template<typename ty>
inline bool Sanae::Matrix<ty>::operator!=
(
	const Matrix<ty>& ArgMatrix
)
{
	return (this->matrix != ArgMatrix.matrix);
}


template<typename ty>
template<typename CastTy>
Sanae::Matrix<ty>::operator Sanae::Matrix<CastTy>()
{
	using CastMatrixT = std::vector<std::vector<CastTy>>;
	
	//キャスト結果を保存
	CastMatrixT ret;
	ret.reserve(this->Rows());

	for (std::vector<ty>& row : this->matrix) 
	{
		//キャスト結果を一行保存
		std::vector<CastTy> buf_row;
		buf_row.reserve(this->Cols());

		for (ty& column:row)
			buf_row.push_back(static_cast<CastTy>(column));
		
		ret.push_back(buf_row);
	}

	return Sanae::Matrix<CastTy>(ret);
}




#endif