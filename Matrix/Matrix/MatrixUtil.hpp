/*-------------------------------------------------------------
* Name    : MatrixUtil.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  Matrix型のutility関数の実装
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIXUTIL_HPP_
#define _INCLUDEGUARD_MATRIXUTIL_HPP_




#include <vector>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <iomanip>

#include "Matrix.h"




//列数がすべて等しいかどうかを調べます。
//* 問題があった場合 InvalidMatrix を throw します。
template<typename ty>
inline void Sanae::Matrix<ty>::m_ValidateMatrix
(
	const Matrix_t& Arg
)
	const
{
	//列数を取得
	size_t Column = this->m_GetColumnSize(Arg);

	//すべての行に於いて列が等しくない場合throwする。
	if (std::any_of(Arg.begin(), Arg.end(), [&Column](const std::vector<ty>& x) {return x.size() != Column; }))
		throw InvalidMatrix("All rows must have the same number of columns.");
}


//正方行列だったら true を返し違えば false を返す。
template<typename ty>
inline void Sanae::Matrix<ty>::m_ValidateSquareMatrix
(
	const Matrix_t& Arg
)
	const
{
	//列数と行数が等しい場合正方行列
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
	//行数が等しい
	if (m_GetRowSize(Arg1) != m_GetRowSize(Arg2))
		return false;

	//列数が等しい
	if (m_GetColumnSize(Arg1) != m_GetColumnSize(Arg2))
		return false;

	return true;
}


//行数を取得します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetRowSize
(
	const Matrix_t& Arg
)
	const
{
	return Arg.size();
}


//列数を取得します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::m_GetColumnSize(const Matrix_t& arg) const
{
	//行数が0のとき列数もなし
	if (arg.size() == 0)
		return 0;

	//行番号0の列数が基準
	return arg[0].size();
}


//行数を返します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::GetRow()
{
	return this->m_GetRowSize(matrix);
}


//列数を返します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::GetColumn()
{
	return this->m_GetColumnSize(matrix);
}


//単位行列にする。
template<typename ty>
inline void Sanae::Matrix<ty>::m_ToIdentity
(
	Matrix_t& Arg
)
	const
{
	//正方行列であるか
	if (this->m_GetRowSize(Arg) != this->m_GetColumnSize(Arg))
		throw std::invalid_argument("It must be a square matrix.");

	//サイズの取得
	size_t size = this->m_GetRowSize(Arg);
	//ゼロ行列にする
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

	// this->matrixの行数と列数を取得
	size_t Row    = this->m_GetRowSize(this->matrix);
	size_t Column = this->m_GetColumnSize(this->matrix);

	// bのサイズをaの列数と行数に設定
	Result.resize(Column, std::vector<ty>(Row));

	// this->matrixの各要素をretに転置してコピー
	for (size_t i = 0; i < Row; i++) {
		for (size_t j = 0; j < Column; j++) {
			Result[j][i] = matrix[i][j];
		}
	}

	return Result;
}


//単位行列を生成します。
template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Identity
(
	size_t ArgSize
)
{
	//arg_size*arg_size行列の0行列を生成
	Matrix_t Result(ArgSize, std::vector<ty>(ArgSize, 0));

	//単位行列へ
	for (size_t pos = 0; pos < ArgSize; pos++)
		Result[pos][pos] = 1;

	return (Sanae::Matrix<ty>)Result;
}


//指定されたサイズのゼロ行列を生成します。
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


//std::cout出力用
template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<
(
	std::basic_ostream<CharT, Traits>& ost,
	Sanae::Matrix<MatrixType>          matrix
	)
{
	//left&改行しておく
	ost << std::left << std::endl;

	//仕切りのサイズ
	std::streamsize shuttersize = Sanae::FontWeight * (matrix.GetColumn() - 1) + 1;

	//仕切り
	ost << std::setfill('-') << std::setw(shuttersize) << "-";

	//setfill解除
	ost << std::setfill(' ') << std::endl;

	for (size_t row = 0; row < matrix.GetRow(); row++) {
		for (MatrixType buf : matrix[row])
			ost << std::setw(Sanae::FontWeight) << buf;

		ost << std::endl;
	}

	//仕切り
	ost << std::setfill('-') << std::setw(shuttersize) << "-";
	ost << std::endl;

	return ost;
}




#endif