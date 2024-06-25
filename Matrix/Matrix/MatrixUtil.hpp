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




#ifndef _INCLUDEGUARD_MATRIXUTIL_HPP_
#define _INCLUDEGUARD_MATRIXUTIL_HPP_




#include <vector>
#include <stdexcept>
#include <functional>
#include <algorithm>

#include "Matrix.h"




/*------------------------------------------------------
* �v���C�x�[�g���\�b�h
------------------------------------------------------*/


//���ׂĂ̍s�ɉ����ė񐔂��������Ȃ��ꍇ InvalidMatrix �� throw ���܂��B
template<typename ty>
inline void Sanae::Matrix<ty>::m_ValidateMatrix
(
	const Matrix_t& Arg
)
	const
{
	//�񐔂��擾
	size_t Column = this->m_GetColumnSize(Arg);

	//���ׂĂ̍s�ɉ����ė񂪓������Ȃ��ꍇ throw ����B
	if (std::any_of(Arg.begin(), Arg.end(), [&Column](const std::vector<ty>& x) {return x.size() != Column; }))
		throw InvalidMatrix("All rows must have the same number of columns.");
}


//�����s�񂩂ǂ���
template<typename ty>
inline bool Sanae::Matrix<ty>::m_IsSquareMatrix
(
	const Matrix_t& Arg
)
	const
{
	//�񐔂ƍs�����������ꍇ�����s��
	return m_GetColumnSize(Arg) == m_GetRowSize(Arg);
}


//�s�񂪋󂩂ǂ�����Ԃ��܂��B
template<typename ty>
inline bool Sanae::Matrix<ty>::m_IsEmpty
(
	const Matrix_t& Arg
)
	const
{
	if (m_GetRowSize(Arg) == 0)
		return true;

	return false;
}


//������,�������̃T�C�Y�����������ǂ������ׂ܂��B
template<typename ty>
inline bool Sanae::Matrix<ty>::m_IsSameSize
(
	const Matrix_t& Arg1,
	const Matrix_t& Arg2
)
	const
{
	//�s����������
	if (m_GetRowSize(Arg1) != m_GetRowSize(Arg2))
		return false;

	//�񐔂�������
	if (m_GetColumnSize(Arg1) != m_GetColumnSize(Arg2))
		return false;

	return true;
}


//�s�����擾���܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetRowSize
(
	const Matrix_t& Arg
)
	const
{
	return Arg.size();
}


//�񐔂��擾���܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetColumnSize
(
	const Matrix_t& Arg
)
	const
{
	//�s����0�̂Ƃ��񐔂��Ȃ�
	if (Arg.size() == 0)
		return 0;

	//�s�ԍ�0�̗񐔂��
	return Arg[0].size();
}


/*------------------------------------------------------
* �p�u���b�N���\�b�h
------------------------------------------------------*/


//�s����Ԃ��܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::Rows()
{
	return m_GetRowSize(matrix);
}


//�񐔂�Ԃ��܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::Cols()
{
	return m_GetColumnSize(matrix);
}


//������ւ��܂��B
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::SwapColumn
(
	size_t Arg1, 
	size_t Arg2
)
{
	for (std::vector<ty>& Rows : this->matrix) {
		ty Buffer = Rows[Arg1];

		Rows[Arg1] = Rows[Arg2];
		Rows[Arg2] = Buffer;
	}

	return *this;
}


template<typename ty>
inline auto Sanae::Matrix<ty>::GetRowRef
(
	size_t ArgRowPos
)
	-> std::vector<std::reference_wrapper<ty>>
{
	std::vector<std::reference_wrapper<ty>> Result;

	if (ArgRowPos < Rows()) {
		for (auto& elem : matrix[ArgRowPos])
			Result.push_back(elem);
	}

	return Result;
}


template<typename ty>
inline auto Sanae::Matrix<ty>::GetColRef
(
	size_t ArgColumnPos
)
	-> std::vector<std::reference_wrapper<ty>>
{
	std::vector<std::reference_wrapper<ty>> Result;

	for (auto& Row : matrix) {
		if (ArgColumnPos < Cols())
			Result.push_back(Row[ArgColumnPos]);
	}

	return Result;
}


//�s�����ւ��܂��B
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::SwapRow
(
	size_t Arg1,
	size_t Arg2
)
{
	std::swap(this->matrix[Arg1], this->matrix[Arg2]);

	return *this;
}


//�T�C�Y��ύX���܂��B
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Resize
(
	std::pair<size_t, size_t> ArgSize
)
{
	this->matrix.resize(ArgSize.first, std::vector<ty>(ArgSize.second, 0));

	return *this;
}


//���ׂĂ̗v�f�ɑ΂��� ArgFunc() �����s���܂��B
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Setter
(
	std::function<ty()> ArgFunc
)
{
	for (size_t Row = 0; Row < this->matrix.size(); Row++)
		for (size_t Column = 0; Column < this->matrix[Row].size(); Column++)
			this->matrix[Row][Column] = ArgFunc();

	return *this;
}


//���ׂĂ̗v�f�ɑ΂���ArgFunc(�s��,��,�v�f)�����s���܂��B
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Setter
(
	std::function<ty(size_t, size_t, ty&)> ArgFunc
)
{
	for (size_t Row = 0; Row < this->matrix.size(); Row++)
		for (size_t Column = 0; Column < this->matrix[Row].size(); Column++)
			this->matrix[Row][Column] = ArgFunc(Row, Column, this->matrix[Row][Column]);

	return *this;
}


//�]�u���s���܂��B
template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Transpose()
{
	Matrix_t Result;

	// this->matrix �̍s���Ɨ񐔂��擾
	size_t Row    = this->m_GetRowSize(this->matrix);
	size_t Column = this->m_GetColumnSize(this->matrix);

	// Result �̃T�C�Y�� this->matrix �̗񐔂ƍs���ɐݒ�
	Result.resize(Column, std::vector<ty>(Row));

	// this->matrix �̊e�v�f�� Result �ɓ]�u���ăR�s�[
	for (size_t i = 0; i < Row; i++) {
		for (size_t j = 0; j < Column; j++) {
			Result[j][i] = this->matrix[i][j];
		}
	}

	return Result;
}


//�P�ʍs��𐶐����܂��B
template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Identity
(
	size_t ArgSize
)
{
	// arg_size * arg_size �s���0�s��𐶐�
	Matrix_t Result(ArgSize, std::vector<ty>(ArgSize, 0));

	//�P�ʍs���
	for (size_t Position = 0; Position < ArgSize; Position++)
		Result[Position][Position] = 1;

	return (Sanae::Matrix<ty>)Result;
}


//�w�肳�ꂽ�T�C�Y�̃[���s��𐶐����܂��B
template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Zero
(
	size_t ArgSize
)
{
	return Matrix<ty>(std::pair<size_t, size_t>(ArgSize, ArgSize));
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Zero
(
	size_t ArgSizeRow, 
	size_t ArgSizeColumn
)
{
	return Matrix<ty>(std::pair<size_t, size_t>(ArgSizeRow, ArgSizeColumn));
}




#endif