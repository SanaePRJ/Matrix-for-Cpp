/*-------------------------------------------------------------
* Name    : MatrixUtil.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  Matrix�^��utility�֐��̎���
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIXUTIL_HPP_
#define _INCLUDEGUARD_MATRIXUTIL_HPP_




#include <vector>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <iomanip>

#include "Matrix.h"




//�񐔂����ׂē��������ǂ����𒲂ׂ܂��B
//* ��肪�������ꍇ InvalidMatrix �� throw ���܂��B
template<typename ty>
inline void Sanae::Matrix<ty>::m_ValidateMatrix
(
	const Matrix_t& Arg
)
	const
{
	//�񐔂��擾
	size_t Column = this->m_GetColumnSize(Arg);

	//���ׂĂ̍s�ɉ����ė񂪓������Ȃ��ꍇthrow����B
	if (std::any_of(Arg.begin(), Arg.end(), [&Column](const std::vector<ty>& x) {return x.size() != Column; }))
		throw InvalidMatrix("All rows must have the same number of columns.");
}


//�����s�񂾂����� true ��Ԃ��Ⴆ�� false ��Ԃ��B
template<typename ty>
inline void Sanae::Matrix<ty>::m_ValidateSquareMatrix
(
	const Matrix_t& Arg
)
	const
{
	//�񐔂ƍs�����������ꍇ�����s��
	if (m_GetColumnSize(Arg) != m_GetRowSize(Arg))
		throw InvalidMatrix("It must be a square matrix.");
}


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


template<typename ty>
inline bool Sanae::Matrix<ty>::m_IsSameSize(const Matrix_t& Arg1,const Matrix_t& Arg2) const
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
inline size_t Sanae::Matrix<ty>::m_GetColumnSize(const Matrix_t& arg) const
{
	//�s����0�̂Ƃ��񐔂��Ȃ�
	if (arg.size() == 0)
		return 0;

	//�s�ԍ�0�̗񐔂��
	return arg[0].size();
}


//�s����Ԃ��܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::GetRow()
{
	return this->m_GetRowSize(matrix);
}


//�񐔂�Ԃ��܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::GetColumn()
{
	return this->m_GetColumnSize(matrix);
}


//�P�ʍs��ɂ���B
template<typename ty>
inline void Sanae::Matrix<ty>::m_ToIdentity
(
	Matrix_t& Arg
)
	const
{
	//�����s��ł��邩
	if (this->m_GetRowSize(Arg) != this->m_GetColumnSize(Arg))
		throw std::invalid_argument("It must be a square matrix.");

	//�T�C�Y�̎擾
	size_t size = this->m_GetRowSize(Arg);
	//�[���s��ɂ���
	Arg.resize(size, std::vector<ty>(size, 0));

	for (size_t pos = 0; pos < size; pos++)
		Arg[pos][pos] = 1;

	return;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::SwapColumn
(
	size_t Arg1, 
	size_t Arg2
)
{
	for (std::vector<ty>& array : this->matrix) {
		ty buf = array[Arg1];
		array[Arg1] = array[Arg2];
		array[Arg2] = buf;
	}

	return *this;
}


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


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::ReSize
(
	std::pair<size_t, size_t> ArgSize
)
{
	this->matrix.resize(ArgSize.first, std::vector<ty>(ArgSize.second, 0));

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Setter
(
	std::function<ty()> ArgFunc
)
{
	for (size_t Row = 0; Row < matrix.size(); Row++)
		for (size_t Column = 0; Column < matrix[Row].size(); Column++)
			matrix[Row][Column] = ArgFunc();

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Setter
(
	std::function<ty(size_t, size_t, ty&)> ArgFunc
)
{
	for (size_t Row = 0; Row < matrix.size(); Row++)
		for (size_t Column = 0; Column < matrix[Row].size(); Column++)
			matrix[Row][Column] = ArgFunc(Row, Column, matrix[Row][Column]);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Transpose()
{
	Matrix_t Result;

	// this->matrix�̍s���Ɨ񐔂��擾
	size_t Row    = this->m_GetRowSize(this->matrix);
	size_t Column = this->m_GetColumnSize(this->matrix);

	// b�̃T�C�Y��a�̗񐔂ƍs���ɐݒ�
	Result.resize(Column, std::vector<ty>(Row));

	// this->matrix�̊e�v�f��ret�ɓ]�u���ăR�s�[
	for (size_t i = 0; i < Row; i++) {
		for (size_t j = 0; j < Column; j++) {
			Result[j][i] = matrix[i][j];
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
	//arg_size*arg_size�s���0�s��𐶐�
	Matrix_t Result(ArgSize, std::vector<ty>(ArgSize, 0));

	//�P�ʍs���
	for (size_t pos = 0; pos < ArgSize; pos++)
		Result[pos][pos] = 1;

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




namespace Sanae {
	std::streamsize FontWeight = 4;
}


//std::cout�o�͗p
template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<
(
	std::basic_ostream<CharT, Traits>& ost,
	Sanae::Matrix<MatrixType>          matrix
	)
{
	//left&���s���Ă���
	ost << std::left << std::endl;

	//�d�؂�̃T�C�Y
	std::streamsize shuttersize = Sanae::FontWeight * (matrix.GetColumn() - 1) + 1;

	//�d�؂�
	ost << std::setfill('-') << std::setw(shuttersize) << "-";

	//setfill����
	ost << std::setfill(' ') << std::endl;

	for (size_t row = 0; row < matrix.GetRow(); row++) {
		for (MatrixType buf : matrix[row])
			ost << std::setw(Sanae::FontWeight) << buf;

		ost << std::endl;
	}

	//�d�؂�
	ost << std::setfill('-') << std::setw(shuttersize) << "-";
	ost << std::endl;

	return ost;
}




#endif