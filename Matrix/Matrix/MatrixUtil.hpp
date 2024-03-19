/*-------------------------------------------------------------
* Name    : MatrixUtil.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  Matrix�^��utility�֐��̎���
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXUTIL_HPP
#define SANAE_MATRIXUTIL_HPP


#include <vector>
#include <stdexcept>
#include <functional>
#include <iomanip>

#include "Matrix.h"




//�񐔂����ׂē��������ǂ����𒲂ׂ܂��B
//* ��肪�������ꍇtrue��Ԃ��܂��B
template<typename ty>
inline bool Sanae::Matrix<ty>::m_CheckColumn(MatrixT* arg) const
{
	size_t Column = this->m_GetColumnSize(arg);

	if (Column == 0)
		return false;

	for (auto& Row : *arg)
		if (Row.size() != Column)
			return true;
	
	return false;
}


//�s�����擾���܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetRowSize(MatrixT* arg) const
{
	return arg->size();
}


//�񐔂��擾���܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetColumnSize(MatrixT* arg) const
{
	//�s����0�̂Ƃ��񐔂��Ȃ�
	if (arg->size() == 0)
		return 0;

	//�s�ԍ�0�̗񐔂��
	return arg->at(0).size();
}


//�s����Ԃ��܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::get_row()
{
	return this->m_GetRowSize(&matrix);
}


//�񐔂�Ԃ��܂��B
template<typename ty>
inline size_t Sanae::Matrix<ty>::get_column()
{
	return this->m_GetColumnSize(&matrix);
}


//�P�ʍs��ɂ���B
template<typename ty>
inline void Sanae::Matrix<ty>::m_to_identity(MatrixT* arg) const
{
	//�����s��ł��邩
	if (this->m_GetRowSize(arg) != this->m_GetColumnSize(arg))
		throw std::invalid_argument("It must be a square matrix.");

	//�T�C�Y�̎擾
	size_t size = this->m_GetRowSize(arg);
	//�[���s��ɂ���
	arg->resize(size,std::vector<ty>(size,0));

	for (size_t pos = 0; pos < size;pos++)
		(*arg)[pos][pos] = 1;
	
	return;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Swap_Column(size_t arg1,size_t arg2) 
{
	for (std::vector<ty>& array : this->matrix) {
		ty buf      = array[arg1];
		array[arg1] = array[arg2];
		array[arg2] = buf;
	}

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Swap_Row(size_t arg1,size_t arg2)
{
	std::swap(this->matrix[arg1],this->matrix[arg2]);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::resize(std::pair<size_t, size_t> arg_Size)
{
	this->matrix.resize(arg_Size.first,std::vector<ty>(arg_Size.second,0));

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Setter(std::function<ty()> arg_func)
{
	for (size_t row = 0; row < matrix.size(); row++)
		for (size_t column = 0; column < matrix[row].size(); column++)
			matrix[row][column] = arg_func();

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Setter(std::function<ty(size_t,size_t,ty&)> arg_func)
{
	for (size_t row = 0; row < matrix.size(); row++)
		for (size_t column = 0; column < matrix[row].size(); column++)
			matrix[row][column] = arg_func(row,column,matrix[row][column]);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Transpose() 
{
	MatrixT ret;
	
	// this->matrix�̍s���Ɨ񐔂��擾
	size_t Row    = this->m_GetRowSize   (&this->matrix);
	size_t Column = this->m_GetColumnSize(&this->matrix);

	// b�̃T�C�Y��a�̗񐔂ƍs���ɐݒ�
	ret.resize(Column, std::vector<ty>(Row));

	// this->matrix�̊e�v�f��ret�ɓ]�u���ăR�s�[
	for (size_t i = 0; i < Row; i++) {
		for (size_t j = 0; j < Column; j++) {
			ret[j][i] = matrix[i][j];
		}
	}

	return ret;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Identity(size_t arg_size)
{
	MatrixT buf(arg_size,std::vector<ty>(arg_size,0));

	for (size_t pos = 0; pos < arg_size;pos++)
		buf[pos][pos] = 1;
	
	return (Sanae::Matrix<ty>)buf;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Zero(size_t arg_size)
{
	return Matrix<ty>(std::pair<size_t,size_t>(arg_size,arg_size));
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
	std::streamsize shuttersize = Sanae::FontWeight * (matrix.get_column() - 1) + 1;

	//�d�؂�
	ost << std::setfill('-') << std::setw(shuttersize) << "-";

	//setfill����
	ost << std::setfill(' ') << std::endl;

	for (size_t row = 0; row < matrix.get_row();row++) {
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