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


//�ȉ��̂悤�ɒ�`���邱�Ƃ��ł���B
//{
//	{0, 0},
//	{0, 0},
//	{0, 0}
//}
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =
(
	MatrixInit_t<ty> ArgInitValue
)
{
	//std::initializer_list<std::initialize_list<ty>>��std::vector<std::vector<ty>>��
	this->matrix = { ArgInitValue.begin(),ArgInitValue.end() };

	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	this->m_ValidateMatrix(this->matrix);

	return *this;
}

template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =
(
	const Matrix& Arg
)
{
	//�T�C�Y�ύX
	this->matrix.resize(Arg.matrix.size());
	//�R�s�[
	std::copy(Arg.matrix.begin(), Arg.matrix.end(), this->matrix.begin());

	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	this->m_ValidateMatrix(this->matrix);

	return *this;
}

//move�p
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =(Matrix<ty>&& arg) = default;

template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator +=
(
	const Matrix<ty>& Arg
) 
{

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_AddGPU(this->matrix, Arg.matrix);

		return *this;
	}

#endif

	this->m_Add(this->matrix, Arg.matrix);

	return *this;
}

template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator -=
(
	const Matrix<ty>& Arg
)
{
#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_SubGPU(this->matrix, Arg.matrix);

		return *this;
	}

#endif

	this->m_Sub(this->matrix, Arg.matrix);

	return *this;
}

template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator ^=
(
	const Matrix<ty>& Arg
)
{
#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_HadamardMulGPU(this->matrix, Arg.matrix);

		return *this;
	}

#endif

	this->m_HadamardMul(this->matrix, Arg.matrix);

	return *this;
}

template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=
(
	const Matrix<ty>& Arg
)
{
#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_MulGPU(this->matrix, Arg.matrix);

		return *this;
	}

#endif

	this->m_Mul(this->matrix, Arg.matrix);

	return *this;
}

template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=
(
	ty Arg
)
{
#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_ScalarMulGPU(this->matrix, Arg);

		return *this;
	}

#endif

	this->m_ScalarMul(this->matrix,Arg);

	return *this;
}

template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator +
(
	const Matrix<ty>& Arg
)
{
	Matrix_t<ty> Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_AddGPU(Result, Arg.matrix);

		return Result;
	}

#endif

	this->m_Add(Result, Arg.matrix);

	return Result;
}

template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator -
(
	const Matrix<ty>& Arg
)
{
	Matrix_t<ty> Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_SubGPU(Result, Arg.matrix);

		return Result;
	}

#endif

	this->m_Sub(Result, Arg.matrix);

	return Result;
}

template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator ^
(
	const Matrix<ty>& Arg
)
{
	Matrix_t<ty> Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_HadamardMulGPU(Result, Arg.matrix);

		return Result;
	}

#endif

	this->m_HadamardMul(Result, Arg.matrix);

	return Result;
}

template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *
(
	const Matrix<ty>& Arg
)
{
	Matrix_t<ty> Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_MulGPU(Result, Arg.matrix);

		return Result;
	}

#endif

	this->m_Mul(Result, Arg.matrix);

	return Result;
}

template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *
(
	ty Arg
)
{
	Matrix_t<ty> Result;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(Result));

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

	if (this->UseCUDA) {
		this->m_ScalarMulGPU(Result, Arg.matrix);

		return Result;
	}

#endif

	this->m_ScalarMul(Result, Arg);

	return Result;
}

//���n
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator<<
(
	Matrix<ty>& Arg
)
{
	//�f�[�^ move
	this->matrix = std::move(Arg.matrix);

	return *this;
}

//�s�ł̔z����擾���܂��B
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
	
	//�L���X�g���ʂ�ۑ�
	CastMatrixT ret;
	ret.reserve(this->Rows());

	for (std::vector<ty>& row : this->matrix) 
	{
		//�L���X�g���ʂ���s�ۑ�
		std::vector<CastTy> buf_row;
		buf_row.reserve(this->Cols());

		for (ty& column:row)
			buf_row.push_back(static_cast<CastTy>(column));
		
		ret.push_back(buf_row);
	}

	return Sanae::Matrix<CastTy>(ret);
}


#endif