/*-------------------------------------------------------------
* Name    : MatrixOperator.hpp
* Version : 4.0.3
* * Author: SanaePRJ
* Description:
*  MatrixBase型のオペレータの実装
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXOPERATOR_HPP
#define SANAE_MATRIXOPERATOR_HPP

#include "Matrix.h"




template<typename ty>
Sanae::Matrix<ty>&Sanae::Matrix<ty>::operator =(MatrixInitT arg_InitValue) {
	MatrixT buf = { arg_InitValue.begin(),arg_InitValue.end() };

	//列数は等しくなければならない。
	if (this->m_CheckColumn(&buf))
		throw std::invalid_argument("All the columns must be equal.");

	//matrixに譲渡
	matrix = { arg_InitValue.begin(), arg_InitValue.end() };

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator =(const Matrix& arg)
{
	//データをクリア
	this->matrix.clear();
	//コピー
	std::copy(arg.matrix.begin(), arg.matrix.end(), std::back_inserter(matrix));

	//列数は等しくなければならない。
	if (this->m_CheckColumn(&this->matrix))
		throw std::invalid_argument("All the columns must be equal.");

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator +=(const Matrix<ty>& arg) {
	this->m_add(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator -=(const Matrix<ty>& arg) {
	this->m_sub(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator ^=(const Matrix<ty>& arg) {
	this->m_dotmul(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=(const Matrix<ty>& arg) {
	this->m_mul(&this->matrix, &arg.matrix);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator *=(ty arg) {
	this->m_scalarmul(&this->matrix,arg);

	return *this;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator +(const Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_add(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator -(const Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_sub(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator ^(const Matrix<ty>& arg) {
	MatrixT buf;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(buf));

	this->m_dotmul(&buf, &arg.matrix);

	return buf;
}


template<typename ty>
Sanae::Matrix<ty> Sanae::Matrix<ty>::operator *(const Matrix<ty>& arg)
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


//譲渡
template<typename ty>
Sanae::Matrix<ty>& Sanae::Matrix<ty>::operator<<(Matrix<ty>& arg)
{
	//データ削除
	this->matrix.erase(this->matrix.begin(),this->matrix.end());
	//データmove
	std::move(arg.matrix.begin(), arg.matrix.end(), std::back_inserter(this->matrix));

	return *this;
}


//行での配列を取得します。
template<typename ty>
std::vector<ty>& Sanae::Matrix<ty>::operator [](size_t pos)
{
	return this->matrix[pos];
}


//列での配列を取得します。
// *ポインタ型がvectorには格納されています。
// *arg.first:開始行 arg.second:列
template<typename ty>
std::vector<ty*> Sanae::Matrix<ty>::operator [](std::pair<size_t,size_t> arg) 
{
	std::vector<ty*> ret;

	for (size_t row = arg.first; row < this->get_row();row++)
		ret.push_back(&this->matrix[row][arg.second]);
	
	return ret;
}


template<typename ty>
bool Sanae::Matrix<ty>::operator==(const Matrix<ty>& arg) 
{
	return (this->matrix == arg.matrix);
}


template<typename ty>
bool Sanae::Matrix<ty>::operator!=(const Matrix<ty>& arg)
{
	return (this->matrix != arg.matrix);
}




#endif