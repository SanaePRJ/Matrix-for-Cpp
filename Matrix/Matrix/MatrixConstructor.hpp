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


#ifndef _INCLUDEGUARD_MATRIXCONSTRUCTOR_HPP_
#define _INCLUDEGUARD_MATRIXCONSTRUCTOR_HPP_


#include <algorithm>
#include "Matrix.h"


//コンストラクタ
template<typename ty>
Sanae::Matrix<ty>::Matrix() {}

//サイズを指定して0行列を生成する。
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	std::pair<size_t,size_t> ArgSize
) 
	:matrix(ArgSize.first, std::vector<ty>(ArgSize.second, 0))
{}

//以下のように定義することができる。
//{
//	{0, 0},
//	{0, 0},
//	{0, 0}
//}
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	MatrixInit_t<ty> ArgInitValue
)
	:matrix(ArgInitValue.begin(), ArgInitValue.end())
{
	//列数は等しくなければならない。
	this->m_ValidateMatrix(this->matrix);
}

//std::vector<std::vector<ty>>
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	const Matrix_t<ty>& ArgInitValue
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