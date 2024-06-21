/*-------------------------------------------------------------
* Name    : MatrixDet.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  MatrixBase型の行列式と逆行列を求める関数の実装
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIXADV_HPP_
#define _INCLUDEGUARD_MATRIXADV_HPP_




#include "Matrix.h"




/*------------------------------------------------------
* プライベートメソッド
------------------------------------------------------*/




// 掃き出し法により求めた結果を arg_store に格納します。
// * ArgStore を確保している必要はありません。
template<typename ty>
inline void Sanae::Matrix<ty>::m_SweepOut
(
	Matrix_t& ArgFrom, 
	Matrix_t& ArgStore
)
{
	//入力された行列が正方行列かどうか調べる。
	m_ValidateSquareMatrix(ArgFrom);

	//arg_fromをコピーする。
	Matrix_t from_copy(m_GetRowSize(ArgFrom),std::vector<ty>(m_GetColumnSize(ArgFrom),0));
	//コピー
	std::copy(ArgFrom.begin(), ArgFrom.end(),from_copy.begin());

	//サイズを格納
	const size_t size = m_GetRowSize(from_copy);

	//確保
	ArgStore.resize(size, std::vector<ty>(size, 0));
	//単位行列へ
	this->m_ToIdentity(ArgStore);

	//ある行の定数倍をほかの行へ加算する。
	auto Operation = [&from_copy, &ArgStore, this, size](size_t from, size_t to, ty num)
		{
			for (size_t Column = 0; Column < m_GetColumnSize(from_copy); Column++) {
				from_copy[to][Column] += from_copy[from][Column] * num;
				ArgStore [to][Column] += ArgStore [from][Column] * num;
			}
		};

	for (size_t Column = 0; Column < size; Column++) {
		for (size_t Row = 0; Row < size; Row++) {
			//定数
			ty num = 0;

			if (Row == Column)
				num = (1 - 1 * ( from_copy)[Row][Column]) / ( from_copy)[Column][Column]; //対角成分を1にする。
			else
				num = -1 * ( from_copy)[Row][Column] / ( from_copy)[Column][Column];      //他の成分を0にする。

			//ある行の定数倍をほかの行へ加算する。
			Operation(Column, Row, num);
		}
	}
}


// 行列式:ライプニッツの行列式
template<typename ty>
inline ty Sanae::Matrix<ty>::m_Det
(
	Matrix_t& Arg
)
{
	//正方行列かどうか調べる。
	this->m_ValidateSquareMatrix(Arg);

	//サラスの方式で解きます。
	const auto DetBy2D = [](const Matrix_t& Matrix2D)
		{
			return Matrix2D[0][0] * Matrix2D[1][1] - Matrix2D[1][0] * Matrix2D[0][1];
		};

	//[Pos][0]-[0][Pos]を含まない行列を抜き取ります。
	const auto Extraction = [](const Matrix_t& From, Matrix_t& Storage, size_t Pos)
		{
			//正方行列ナノは確定しているので次元を求める。
			const size_t Dim = From.size();

			for (size_t Row = 0, StorageRow = 0; Row < Dim; Row++) {
				//0の列を基準としている。
				for (size_t Column = 1; Column < Dim; Column++) {
					if (Row != Pos)
						Storage[StorageRow][Column - 1] = From[Row][Column];
				}
				//基準の高さ以外の場合は次へ
				if (Pos != Row)
					StorageRow++;
			}

			return;
		};

	//次元を落としまくる。
	const auto DecDim = [this, Extraction, DetBy2D](const Matrix_t& From, ty Coeff, auto Func)
		{
			const size_t Size    = From.size(); //元のサイズ
			const size_t NewSize = Size - 1;    //落とした後のサイズ

			ty Result = 0; //返り値

			//2*2行列にまで落ちたらサラスの方式で解いて返す。
			if (Size == 2)
				return Coeff * DetBy2D(From);

			//[0][0]~[n][0]まで
			for (size_t Position = 0; Position < Size; Position++)
			{
				Matrix_t Buffer(NewSize, std::vector<ty>(NewSize, 0)); //格納用
				Extraction(From, Buffer, Position);                    //縮小した行列を取得

				ty CoeffBuffer = From[Position][0] * (Position % 2 == 0 ? 1 : -1); //新しい係数

				Result += Func(Buffer, CoeffBuffer * Coeff, Func); //再帰させ結果をすべて加算
			}

			return Result;
		};
	
	//元の係数
	const ty FromCoeff = 1;

	return DecDim(Arg, FromCoeff, DecDim);
}




/*------------------------------------------------------
* パブリックメソッド
------------------------------------------------------*/




// 行列式を求める。
template<typename ty>
inline ty Sanae::Matrix<ty>::Det()
{
	return this->m_Det(this->matrix);
}


// 逆行列を求める。
template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Inverse()
{
	//列数は等しくなければならない。
	this->m_ValidateMatrix(this->matrix);

	//行列式が0の場合解なし
	if (this->Det() == 0)
		throw InvalidMatrix("It is not a regular matrix.");

	Matrix_t CopyMatrix;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(CopyMatrix));

	Matrix_t Inv;
	this->m_SweepOut(CopyMatrix, Inv);

	return Inv;
}




#endif