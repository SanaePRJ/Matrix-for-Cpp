/*-------------------------------------------------------------
* Name    : Matrix.h
* Version : 4.0.5
* Author  : SanaePRJ
* Description:
*  MatrixBase型の定義ファイル
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIX_H_
#define _INCLUDEGUARD_MATRIX_H_




#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <thread>




namespace Sanae{


	template<typename ty> class Matrix{
	private:


		template<typename RowType    > using Row_t     = std::vector<RowType>;
		template<typename RowInitType> using RowInit_t = std::initializer_list<RowInitType>;

		using Matrix_t     = std::vector          <Row_t<ty>>;
		using MatrixInit_t = std::initializer_list<RowInit_t<ty>>;

	
		//Variables
		Matrix_t matrix;


		//MatrixUtil.hpp
		// コンストラクタや=演算子などで実行する。
		inline void   m_ValidateMatrix(const Matrix_t&) const; //すべての行に於いて列数が等しいか確認する。@列数が等しくない場合 throw します。
		
		inline bool   m_IsSquareMatrix(const Matrix_t&) const; //正方行列かどうかを取得する。
		inline bool   m_IsEmpty       (const Matrix_t&) const; //空の行列かどうかを返す。
		inline bool   m_IsSameSize    (const Matrix_t&,const Matrix_t&) const; //同じ大きさかどうかを返す。

		inline size_t m_GetRowSize   (const Matrix_t&) const; //行数を取得
		inline size_t m_GetColumnSize(const Matrix_t&) const; //列数を取得
		

		//MatrixCalc.hpp
		template<typename FuncType>
		inline void m_Calc       (Matrix_t&, const Matrix_t&,FuncType) const; //すべての要素で第一引数=FuncType(第一引数,第二引数)を実行する。

		inline void m_Add        (Matrix_t&, const Matrix_t&) const; //加算        :第一引数 += 第二引数
		inline void m_Sub        (Matrix_t&, const Matrix_t&) const; //減算        :第一引数 -= 第二引数
		inline void m_ScalarMul  (Matrix_t&, ty             ) const; //スカラー倍  :第一引数 *= 第二引数
		inline void m_HadamardMul(Matrix_t&, const Matrix_t&) const; //アダマール積:第一引数 ^= 第二引数

		inline void m_Mul        (Matrix_t&, const Matrix_t&) const; //積　        :第一引数 *= 第二引数


		//MatrixAdvCalc.hpp
		//削除予定
		inline void  m_SweepOut(Matrix_t& arg_from, Matrix_t& arg_store); //掃き出し法により逆行列を求める。
		inline ty    m_Det     (Matrix_t& arg);                           //行列式を求める。

		inline void  m_LUDecomposition(const Matrix_t&, Matrix_t&, Matrix_t&) const; //LU分解を行い第二引数L,第三引数Uに格納する。
		
		inline ty    m_DetByU (const Matrix_t&) const;                   //LU分解によって求めたU(上三角行列)によって行列式を求める。
		inline void  m_Inverse(const Matrix_t&,Matrix_t&,ty=1e-5) const; //逆行列を求める。
	

	public:


		//行列積で使用するスレッド数(初期値は最大のスレッド数の半分)
		size_t thread = std::thread::hardware_concurrency()/2;


		//MatrixConstructor.hpp
		Matrix ();
		Matrix (std::pair<size_t,size_t>); //サイズ指定
		Matrix (MatrixInit_t);             //{{0,0},{0,0}}...2*2行列
		Matrix (const Matrix_t&);          //std::vector<std::vector<ty>>
		Matrix (const Matrix&);            //コピーコンストラクタ
		~Matrix();                         //デストラクタ


		//MatrixOperator.hpp
		inline Matrix& operator =(MatrixInit_t);
		inline Matrix& operator =(const Matrix&);
		inline Matrix& operator =(Matrix&&);

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

		template<typename CastTy> operator Sanae::Matrix<CastTy>(); //キャスト

		inline Matrix& operator <<(Matrix&);         //譲渡
		inline std::vector<ty>& operator [](size_t); //要素へアクセス

		inline bool    operator ==(const Matrix&); //比較==
		inline bool    operator !=(const Matrix&); //比較!=


		//MatrixUtil.hpp
		inline size_t GetRow   (); //行数を取得
		inline size_t GetColumn(); //列数を取得

		inline Matrix& SwapRow(size_t, size_t); //第一引数行と第二引数行を入れ替える。
		inline Matrix& SwapColumn(size_t, size_t); //第一引数列と第二引数列を入れ替える。

		inline Matrix& ReSize(std::pair<size_t, size_t>); //サイズを変更する。

		inline Matrix& Setter     (std::function<ty()>);                  //引数の関数を呼び出し各要素へ返り値を代入する。
		inline Matrix& Setter     (std::function<ty(size_t,size_t,ty&)>); //引数の関数を呼び出し各要素へ返り値を代入する。(行数,列数,元の行列[行数][列数])を引数として受け取る。

		inline Matrix  Transpose  (); //転置する。

		static inline Matrix<ty> Identity(size_t); //指定された行,列数の単位行列を作成します。
		static inline Matrix<ty> Zero    (size_t); //指定された行,列数の0行列を作成します。


		//MatrixAdvCalc.hpp
		inline ty     Det    ();        //行列式を求める。
		inline Matrix Inverse(ty=1e-5); //逆行列を求める。


 	};


}




/*無効な行列を示す例外クラス*/
class InvalidMatrix : std::exception{
protected:
	std::string m_ErrorMessage = "It is an invalid matrix.";

public:
	InvalidMatrix() {}
	
	//メッセージを入力
	InvalidMatrix(const char* ArgMessage) : m_ErrorMessage(ArgMessage) {}

	//エラーメッセージを返す。
	const char* what() const noexcept override {return m_ErrorMessage.c_str();}
};




#endif