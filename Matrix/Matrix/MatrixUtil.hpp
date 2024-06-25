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
* プライベートメソッド
------------------------------------------------------*/


//すべての行に於いて列数が等しくない場合 InvalidMatrix を throw します。
template<typename ty>
inline void Sanae::Matrix<ty>::m_ValidateMatrix
(
	const Matrix_t& Arg
)
	const
{
	//列数を取得
	size_t Column = this->m_GetColumnSize(Arg);

	//すべての行に於いて列が等しくない場合 throw する。
	if (std::any_of(Arg.begin(), Arg.end(), [&Column](const std::vector<ty>& x) {return x.size() != Column; }))
		throw InvalidMatrix("All rows must have the same number of columns.");
}


//正方行列かどうか
template<typename ty>
inline bool Sanae::Matrix<ty>::m_IsSquareMatrix
(
	const Matrix_t& Arg
)
	const
{
	//列数と行数が等しい場合正方行列
	return m_GetColumnSize(Arg) == m_GetRowSize(Arg);
}


//行列が空かどうかを返します。
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


//第一引数,第二引数のサイズが等しいかどうか調べます。
template<typename ty>
inline bool Sanae::Matrix<ty>::m_IsSameSize
(
	const Matrix_t& Arg1,
	const Matrix_t& Arg2
)
	const
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
inline size_t Sanae::Matrix<ty>::m_GetColumnSize
(
	const Matrix_t& Arg
)
	const
{
	//行数が0のとき列数もなし
	if (Arg.size() == 0)
		return 0;

	//行番号0の列数が基準
	return Arg[0].size();
}


/*------------------------------------------------------
* パブリックメソッド
------------------------------------------------------*/


//行数を返します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::Rows()
{
	return m_GetRowSize(matrix);
}


//列数を返します。
template<typename ty>
inline size_t Sanae::Matrix<ty>::Cols()
{
	return m_GetColumnSize(matrix);
}


//列を入れ替えます。
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


//行を入れ替えます。
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


//サイズを変更します。
template<typename ty>
inline Sanae::Matrix<ty>& Sanae::Matrix<ty>::Resize
(
	std::pair<size_t, size_t> ArgSize
)
{
	this->matrix.resize(ArgSize.first, std::vector<ty>(ArgSize.second, 0));

	return *this;
}


//すべての要素に対して ArgFunc() を実行します。
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


//すべての要素に対してArgFunc(行数,列数,要素)を実行します。
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


//転置を行います。
template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Transpose()
{
	Matrix_t Result;

	// this->matrix の行数と列数を取得
	size_t Row    = this->m_GetRowSize(this->matrix);
	size_t Column = this->m_GetColumnSize(this->matrix);

	// Result のサイズを this->matrix の列数と行数に設定
	Result.resize(Column, std::vector<ty>(Row));

	// this->matrix の各要素を Result に転置してコピー
	for (size_t i = 0; i < Row; i++) {
		for (size_t j = 0; j < Column; j++) {
			Result[j][i] = this->matrix[i][j];
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
	// arg_size * arg_size 行列の0行列を生成
	Matrix_t Result(ArgSize, std::vector<ty>(ArgSize, 0));

	//単位行列へ
	for (size_t Position = 0; Position < ArgSize; Position++)
		Result[Position][Position] = 1;

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