/*-------------------------------------------------------------
* Name    : MatrixUtil.hpp
* Version : 4.0.2
* * Author: SanaePRJ
* Description:
*  Matrix型のutility関数の実装
-------------------------------------------------------------*/




#ifndef SANAE_MATRIXUTIL_HPP
#define SANAE_MATRIXUTIL_HPP


#include <vector>
#include <stdexcept>
#include <functional>

#include "Matrix.h"




//列数がすべて等しいかどうかを調べます。
//* 問題があった場合trueを返します。
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


//行数を取得します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetRowSize(MatrixT* arg) const
{
	return arg->size();
}


//列数を取得します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetColumnSize(MatrixT* arg) const
{
	//行数が0のとき列数もなし
	if (arg->size() == 0)
		return 0;

	//行番号0の列数が基準
	return arg->at(0).size();
}


//行数を返します。
template<typename ty>
size_t Sanae::Matrix<ty>::get_row()
{
	return this->m_GetRowSize(&matrix);
}


//列数を返します。
template<typename ty>
size_t Sanae::Matrix<ty>::get_column() 
{
	return this->m_GetColumnSize(&matrix);
}


//単位行列にする。
template<typename ty>
inline void Sanae::Matrix<ty>::m_to_identity(MatrixT* arg) const
{
	if (this->m_GetRowSize(arg) != this->m_GetColumnSize(arg))
		throw std::invalid_argument("It must be a square matrix.");

	for (size_t Row = 0; Row < this->m_GetRowSize(arg);Row++) 
	{
		for (size_t Column = 0; Column < this->m_GetColumnSize(arg);Column++) 
		{
			if (Row == Column)
				(*arg)[Row][Column] = 1;
			else
				(*arg)[Row][Column] = 0;
		}
	}
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
	for (std::vector<ty>& Rows : matrix)
		for (ty& Columns : Rows)
			Columns = arg_func();

	return *this;
}


template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Transpose() 
{
	MatrixT ret;
	
	// this->matrixの行数と列数を取得
	size_t Row    = this->m_GetRowSize   (&this->matrix);
	size_t Column = this->m_GetColumnSize(&this->matrix);

	// bのサイズをaの列数と行数に設定
	ret.resize(Column, std::vector<ty>(Row));

	// this->matrixの各要素をretに転置してコピー
	for (size_t i = 0; i < Row; i++) {
		for (size_t j = 0; j < Column; j++) {
			ret[j][i] = matrix[i][j];
		}
	}

	return ret;
}




//std::cout出力用
template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<
(
	std::basic_ostream<CharT, Traits>& ost,
	Sanae::Matrix <MatrixType>  matrix
)
{
	for (size_t row = 0; row < matrix.get_row();row++) {
		for (MatrixType buf : matrix[row])
			ost << buf << " ";

		ost << std::endl;
	}

	return ost;
}




#endif