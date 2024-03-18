/*-------------------------------------------------------------
* Name    : Matrix.h
* Version : 4.0.5
* Author  : SanaePRJ
* Description:
*  MatrixBase型の定義ファイル
-------------------------------------------------------------*/




#ifndef SANAE_MATRIX_H
#define SANAE_MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>




namespace Sanae{


	template<typename ty>
	class Matrix
	{
	private:
		//Variables
		using MatrixT     = std::vector<std::vector<ty>>;
		using MatrixInitT = std::initializer_list<std::initializer_list<ty>>;

		MatrixT matrix;


		//Private Methods
		inline bool   m_CheckColumn  (MatrixT*) const;   //列数が正しいかどうか確認する。

		inline size_t m_GetRowSize   (MatrixT*) const;   //行数を取得
		inline size_t m_GetColumnSize(MatrixT*) const;   //列数を取得

		inline void m_calc     (MatrixT*, const MatrixT*,std::function<ty(ty,ty)>) const;   //std::functionで受け取った関数で演算

		inline void m_add      (MatrixT*, const MatrixT*) const;   //加算      :第一引数 += 第二引数
		inline void m_sub      (MatrixT*, const MatrixT*) const;   //減算      :第一引数 -= 第二引数
		inline void m_dotmul   (MatrixT*, const MatrixT*) const;   //内積      :第一引数 ^= 第二引数
		inline void m_scalarmul(MatrixT*, ty      ) const;         //スカラー倍:第一引数 *= 第二引数

		inline void m_mul      (MatrixT*, const MatrixT*)      ;   //積　      :第一引数 *= 第二引数

		inline void m_to_identity(MatrixT*) const;                 //単位行列にする。

		inline void m_sweepout(MatrixT* arg_from, MatrixT* arg_store);   //掃き出し法により逆行列を求める。
		inline ty   m_det     (MatrixT* arg);                            //行列式を求める。


	public:
		//行列積で使用するスレッド数
		size_t thread = std::thread::hardware_concurrency();

		//コンストラクタ
		Matrix ();
		Matrix (std::pair<size_t,size_t>);  //サイズ指定
		Matrix (MatrixInitT);               //{{0,0},{0,0}}...2*2行列
		Matrix (const MatrixT&);            //std::vector<std::vector<ty>>
		Matrix (const Matrix&);             //コピーコンストラクタ
		~Matrix();                          //デストラクタ

		inline Matrix& operator =(MatrixInitT);
		inline Matrix& operator =(const Matrix&);

		inline Matrix& operator +=(const Matrix&); //加算
		inline Matrix& operator -=(const Matrix&); //減算
		inline Matrix& operator ^=(const Matrix&); //内積
		inline Matrix& operator *=(const Matrix&); //行列積
		inline Matrix& operator *=(ty);            //スカラー倍

		inline Matrix  operator + (const Matrix&); //加算
		inline Matrix  operator - (const Matrix&); //減算
		inline Matrix  operator ^ (const Matrix&); //内積
		inline Matrix  operator * (const Matrix&); //行列積
		inline Matrix  operator * (ty);            //スカラー倍
		template<typename CastTy>
		operator Sanae::Matrix<CastTy>();   //キャスト

		inline Matrix& operator <<(Matrix&);         //譲渡
		inline std::vector<ty>& operator [](size_t); //要素へアクセス
		inline std::vector<ty*> operator [](std::pair<size_t,size_t>);

		inline bool    operator ==(const Matrix&);  //比較==
		inline bool    operator !=(const Matrix&);  //比較!=

		inline size_t get_row   ();  //行数を取得
		inline size_t get_column();  //列数を取得
		
		inline ty     det       ();  //行列式を求める。
		inline Matrix Inverse   ();  //逆行列を求める。

		inline Matrix& Swap_Column(size_t, size_t);  //第一引数列と第二引数列を入れ替える。
		inline Matrix& Swap_Row   (size_t, size_t);  //第一引数行と第二引数行を入れ替える。
		
		inline Matrix& resize     (std::pair<size_t,size_t>);  //サイズを変更する。

		inline Matrix& Setter     (std::function<ty()>);                 //引数の関数を呼び出し各要素へ返り値を代入する。
		inline Matrix& Setter     (std::function<ty(size_t,size_t,ty&)>);//引数の関数を呼び出し各要素へ返り値を代入する。(行数,列数,元の行列[行数][列数])を引数として受け取る。

		inline Matrix  Transpose  ();  //転置する。
 	};


}




#endif