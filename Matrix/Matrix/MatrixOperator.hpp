/*-------------------------------------------------------------
* Name    : MatrixOperator.hpp
* Version : Beta
* * Author: SanaePRJ
* Description:
*  MatrixBase�^�̃I�y���[�^�̎���
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXOPERATOR_HPP
#define SANAE_MATRIXOPERATOR_HPP

#include "Matrix.h"




template<typename ty>
Sanae::Matrix<ty>&Sanae::Matrix<ty>::operator =(MatrixInitT arg_InitValue) {
	MatrixT buf = { arg_InitValue.begin(),arg_InitValue.end() };

	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	if (this->m_CheckColumn(&buf))
		throw std::invalid_argument("All the columns must be equal.");

	//matrix�ɏ��n
	matrix = { arg_InitValue.begin(), arg_InitValue.end() };

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =(Matrix& arg)
{
	//�f�[�^���N���A
	this->matrix.clear();
	//�R�s�[
	std::copy(arg.matrix.begin(), arg.matrix.end(), std::back_inserter(matrix));

	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	if (this->m_CheckColumn(&this->matrix))
		throw std::invalid_argument("All the columns must be equal.");

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator +=(Matrix<ty>& arg) {
	this->m_add(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator -=(Matrix<ty>& arg) {
	this->m_sub(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator ^=(Matrix<ty>& arg) {
	this->m_dotmul(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=(Matrix<ty>& arg) {
	this->m_mul(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=(ty arg) {
	this->m_scalarmul(&this->matrix,arg);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator +(Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_add(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator -(Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_sub(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator ^(Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_dotmul(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *(Matrix<ty>& arg)
{
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_mul(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *(ty arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_scalarmul(&buf, arg);

	return buf;
}


template<typename ty>
std::vector<ty>& Sanae::Matrix<ty>::operator [](size_t pos)
{
	return this->matrix[pos];
}




#endif