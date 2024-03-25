/*-------------------------------------------------------------
* Name    : MatrixOperator.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  MatrixBase�^�̃I�y���[�^�̎���
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXOPERATOR_HPP
#define SANAE_MATRIXOPERATOR_HPP

#include <algorithm>
#include "Matrix.h"



template<typename ty>
inline Sanae::Matrix<ty>&Sanae::Matrix<ty>::operator =(MatrixInitT arg_InitValue) {
	//std::initializer_list<std::initialize_list<ty>>��std::vector<std::vector<ty>>��
	this->matrix = { arg_InitValue.begin(),arg_InitValue.end() };

	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	if (this->m_CheckColumn(&matrix))
		throw std::invalid_argument("All the columns must be equal.");

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =(const Matrix& arg)
{
	//�T�C�Y�ύX
	this->matrix.resize(arg.matrix.size());
	//�R�s�[
	std::copy(arg.matrix.begin(), arg.matrix.end(), this->matrix.begin());

	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	if (this->m_CheckColumn(&this->matrix))
		throw std::invalid_argument("All the columns must be equal.");

	return *this;
}

template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =(Matrix<ty>&& arg) = default;


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator +=(const Matrix<ty>& arg) {
	this->m_add(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator -=(const Matrix<ty>& arg) {
	this->m_sub(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator ^=(const Matrix<ty>& arg) {
	this->m_dotmul(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=(const Matrix<ty>& arg) {
	this->m_mul(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=(ty arg) {
	this->m_scalarmul(&this->matrix,arg);

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator +(const Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_add(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator -(const Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_sub(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator ^(const Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_dotmul(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *(const Matrix<ty>& arg)
{
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_mul(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *(ty arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_scalarmul(&buf, arg);

	return buf;
}


//���n
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator<<(Matrix<ty>& arg)
{
	//�f�[�^move
	this->matrix = std::move(arg.matrix);

	return *this;
}


//�s�ł̔z����擾���܂��B
template<typename ty>
inline std::vector<ty>& Sanae::Matrix<ty>::operator [](size_t pos)
{
	return this->matrix[pos];
}


//[[�񐄏�]]
//��ł̔z����擾���܂��B
// *�|�C���^�^��vector�ɂ͊i�[����Ă��܂��B
// *arg.first:�J�n�s arg.second:��
template<typename ty>
inline std::vector<ty*> Sanae::Matrix<ty>::operator [](std::pair<size_t, size_t> arg)
{
	std::vector<ty*> ret;

	for (size_t row = arg.first; row < this->get_row();row++)
		ret.push_back(&this->matrix[row][arg.second]);
	
	return ret;
}


template<typename ty>
inline bool Sanae::Matrix<ty>::operator==(const Matrix<ty>& arg)
{
	return (this->matrix == arg.matrix);
}


template<typename ty>
inline bool Sanae::Matrix<ty>::operator!=(const Matrix<ty>& arg)
{
	return (this->matrix != arg.matrix);
}


template<typename ty>
template<typename CastTy>
Sanae::Matrix<ty>::operator Sanae::Matrix<CastTy>()
{
	using CastMatrixT = std::vector<std::vector<CastTy>>;
	
	//�L���X�g���ʂ�ۑ�
	CastMatrixT ret;
	ret.reserve(this->get_row());

	for (std::vector<ty>& row : this->matrix) 
	{
		//�L���X�g���ʂ���s�ۑ�
		std::vector<CastTy> buf_row;
		buf_row.reserve(this->get_column());

		for (ty& column:row)
			buf_row.push_back(static_cast<CastTy>(column));
		
		ret.push_back(buf_row);
	}

	return Sanae::Matrix<CastTy>(ret);
}




#endif