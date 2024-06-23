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




/*
* 現在このメソッドは使用せずLU分解により逆行列を求めるようにしています。
*/
// 掃き出し法により求めた結果を arg_store に格納します。
// * ArgStore を確保している必要はありません。
template<typename ty>
inline void Sanae::Matrix<ty>::m_SweepOut
(
	Matrix_t& ArgFrom, 
	Matrix_t& ArgStore
)
{
	//入力された行列が正方行列でなければならない。
	if (!m_IsSquareMatrix(ArgFrom))
		throw InvalidMatrix("Must be a square matrix.");

	//arg_fromをコピーする。
	Matrix_t from_copy(m_GetRowSize(ArgFrom),std::vector<ty>(m_GetColumnSize(ArgFrom),0));
	//コピー
	std::copy(ArgFrom.begin(), ArgFrom.end(),from_copy.begin());

	//サイズを格納
	const size_t size = m_GetRowSize(from_copy);

	//確保
	ArgStore = Identity(size);

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


/*
* 現在このメソッドは使用せずLU分解により逆行列を求めるようにしています。
*/
// 行列式:ライプニッツの行列式
template<typename ty>
inline ty Sanae::Matrix<ty>::m_Det
(
	Matrix_t& Arg
)
{
	//入力された行列が正方行列でなければならない。
	if (!m_IsSquareMatrix(Arg))
		throw InvalidMatrix("Must be a square matrix.");

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


// ArgMatrix を MatrixL と MatrixU へ分解します。
template<typename ty>
inline void Sanae::Matrix<ty>::m_LUDecomposition(const Matrix_t& ArgMatrix, Matrix_t& MatrixL, Matrix_t&MatrixU)
{
	//入力された行列が正方行列でなければならない。
	if (!m_IsSquareMatrix(ArgMatrix))
		throw InvalidMatrix("Must be a square matrix.");

	//すべての列に対して LRowFrom 行を LNum 倍したものを LRowTo 行へ加算する。
	auto Operation = [this](Matrix_t& LambdaMatrix,size_t LambdaRowFrom,size_t LambdaRowTo,ty LambdaNum)
		{
			//すべての列で実行
			for (size_t Column = 0; Column < m_GetColumnSize(LambdaMatrix); Column++) {
				// LRowFrom 行を LNum 倍したものを LRowTo 行へ加算する。
				LambdaMatrix[LambdaRowTo][Column] += LambdaNum * LambdaMatrix[LambdaRowFrom][Column];
			}
		};

	const size_t Size = m_GetRowSize(ArgMatrix);

	//単位行列取得
	MatrixL = Identity(Size).matrix;
	MatrixU = ArgMatrix;

	for (size_t Column = 0; Column < Size;Column++) {

		//BaseRow行を他の行へ加算し0にする。
		const size_t BaseRow = Column;

		for (size_t Row = Column + 1; Row < Size; Row++) {
			// A[BaseRow][Column]に何を掛けたらA[Row][Column]が0になるかを求める。
			// 
			// 0 = A[Row][Column] + A[BaseRow][Column]*MulNum
			// MulNum = -A[Row][Column] / A[BaseRow][Column]

			ty MulNum = (-1 * MatrixU[Row][Column]) / MatrixU[BaseRow][Column];

			// MatrixUの更新
			Operation(MatrixU, BaseRow, Row, MulNum);

			// MatrixLの更新
			MatrixL[Row][Column] = -MulNum;
		}
	}
}


template<typename ty>
inline ty Sanae::Matrix<ty>::m_DetByU(const Matrix_t& MatrixU)
{
	//入力された行列が正方行列でなければならない。
	if (!m_IsSquareMatrix(MatrixU))
		throw InvalidMatrix("Must be a square matrix.");

	ty Det = 1;
	
	//LU分解したUの体格要素の積で求めることが出来る。
	for (size_t Pos = 0; Pos < m_GetRowSize(MatrixU); ++Pos)
		Det *= MatrixU[Pos][Pos];
	
	return Det;
}


//逆行列を求めます。
template<typename ty>
inline void Sanae::Matrix<ty>::m_Inverse(const Matrix_t& ArgMatrix, Matrix_t& Store, ty Epsilon)
{
	//入力された行列が正方行列でなければならない。
	if (!m_IsSquareMatrix(ArgMatrix))
		throw InvalidMatrix("Must be a square matrix.");

	// 行列のサイズを取得
	const size_t Size = m_GetRowSize(ArgMatrix);

	// LU分解を行う
	Matrix_t MatrixL, MatrixU;
	m_LUDecomposition(ArgMatrix, MatrixL, MatrixU);
	
	/* 行列式が0の場合0に限りなく近くすることで近似させるためいらない
	if (m_DetByU(MatrixU) == 0)
		throw InvalidMatrix("It is not a regular matrix.");
	*/

	//行列式が0の場合0に限りなく近い数を入れることで近似する。
	for (size_t Pos = 0; Pos < Size;Pos++)
		MatrixU[Pos][Pos] += MatrixU[Pos][Pos] == 0 ? Epsilon : 0;

	// 単位行列を作成
	Matrix_t IdentityMatrix = Identity(Size).matrix;

	// 逆行列を初期化
	Store.resize(Size, std::vector<ty>(Size, 0));

	// 各列ごとに逆行列を求める
	for (size_t Column = 0; Column < Size; Column++) {
		// 単位行列の列ベクトルを取り出す
		std::vector<ty> e(Size, 0);
		e[Column] = 1;

		// 前進代入を行う
		std::vector<ty> MatrixY(Size, 0);
		for (size_t i = 0; i < Size; i++) {
			MatrixY[i] = e[i];
			
			for (size_t j = 0; j < i; j++)
				MatrixY[i] -= MatrixL[i][j] * MatrixY[j];

			MatrixY[i] /= MatrixL[i][i];
		}

		// 後退代入を行う
		std::vector<ty> MatrixX(Size, 0);
		for (long long i = Size - 1; i >= 0; i--) {
			MatrixX[i] = MatrixY[i];

			for (size_t j = i + 1; j < Size; j++) 
				MatrixX[i] -= MatrixU[i][j] * MatrixX[j];

			MatrixX[i] /= MatrixU[i][i];
		}

		// 結果をStore行列の列にコピーする
		for (size_t i = 0; i < Size; i++)
			Store[i][Column] = MatrixX[i];
	}
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
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Inverse(ty Epsilon)
{
	Matrix_t Inv;
	m_Inverse(this->matrix,Inv,Epsilon);

	return Inv;
}




#endif