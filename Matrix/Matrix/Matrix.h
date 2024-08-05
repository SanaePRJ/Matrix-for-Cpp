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


// Include guard
#ifndef _INCLUDEGUARD_MATRIX_H_
#define _INCLUDEGUARD_MATRIX_H_


// Include
#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <thread>
#include <iomanip>


// CUDA
#ifdef _SANAE_MATRIX_ENABLE_CUDA_


	#include <cuda_runtime.h>
	#include <device_launch_parameters.h>


#endif


namespace Sanae{


	template<typename ty> class Matrix{
	private:
		enum class CalcOpeCode;

		template<typename RowType    > using Row_t     = std::vector<RowType>;
		template<typename RowInitType> using RowInit_t = std::initializer_list<RowInitType>;

		template<typename MatrixType = ty> using Matrix_t     = std::vector          <Row_t<MatrixType>>;
		template<typename MatrixType = ty> using MatrixInit_t = std::initializer_list<RowInit_t<MatrixType>>;

	
		// Variables
		Matrix_t<ty> matrix;


		// MatrixUtil.hpp
		// コンストラクタや=演算子などで実行する。
		inline void   m_ValidateMatrix(const Matrix_t<ty>&) const; //すべての行に於いて列数が等しいか確認する。@列数が等しくない場合 throw します。
		
		inline bool   m_IsSquareMatrix(const Matrix_t<ty>&) const; //正方行列かどうかを取得する。
		inline bool   m_IsEmpty       (const Matrix_t<ty>&) const; //空の行列かどうかを返す。
		inline bool   m_IsSameSize    (const Matrix_t<ty>&,const Matrix_t<ty>&) const; //同じ大きさかどうかを返す。

		inline size_t m_GetRowSize   (const Matrix_t<ty>&) const; //行数を取得
		inline size_t m_GetColumnSize(const Matrix_t<ty>&) const; //列数を取得

		
		// MatrixCalc.hpp
		template<typename FuncType>
		inline void m_Calc       (Matrix_t<ty>&, const Matrix_t<ty>&,FuncType); // すべての要素で第一引数=FuncType(第一引数,第二引数)を実行する。

		inline void m_Add        (Matrix_t<ty>&, const Matrix_t<ty>&); // 加算        : 第一引数 += 第二引数
		inline void m_Sub        (Matrix_t<ty>&, const Matrix_t<ty>&); // 減算        : 第一引数 -= 第二引数
		inline void m_ScalarMul  (Matrix_t<ty>&, ty                 ); // スカラー倍  : 第一引数 *= 第二引数
		inline void m_HadamardMul(Matrix_t<ty>&, const Matrix_t<ty>&); // アダマール積: 第一引数 ^= 第二引数

		inline void m_Mul        (Matrix_t<ty>&, const Matrix_t<ty>&);       // 積　        : 第一引数 *= 第二引数


		// MatrixCalcCUDA.hpp
#ifdef _SANAE_MATRIX_ENABLE_CUDA_

		static std::vector<ty> Flatten  (const Matrix_t<ty>&);
		static Matrix_t<ty>    UnFlatten(const std::vector<ty>&, size_t, size_t);


		inline void m_CalcGPU(Matrix_t<ty>&, const Matrix_t<ty>&, CalcOpeCode) const;

		inline void m_AddGPU(Matrix_t<ty>&, const Matrix_t<ty>&) const;         // 加算        :第一引数 += 第二引数
		inline void m_SubGPU(Matrix_t<ty>&, const Matrix_t<ty>&) const;         // 減算        :第一引数 -= 第二引数
		inline void m_ScalarMulGPU  (Matrix_t<ty>&, ty     ) const;             // スカラー倍  :第一引数 *= 第二引数
		inline void m_HadamardMulGPU(Matrix_t<ty>&, const Matrix_t<ty>&) const; // アダマール積:第一引数 ^= 第二引数
		inline void m_MulGPU(Matrix_t<ty>&, const Matrix_t<ty>&);               // 積　        :第一引数 *= 第二引数

#endif


		// MatrixAdvCalc.hpp
		// 削除予定
		inline void  m_SweepOut(Matrix_t<ty>& arg_from, Matrix_t<ty>& arg_store); // 掃き出し法により逆行列を求める。
		inline ty    m_Det     (Matrix_t<ty>& arg);                               // 行列式を求める。

		inline void  m_LUDecomposition(const Matrix_t<ty>&, Matrix_t<ty>&, Matrix_t<ty>&) const; // LU分解を行い第二引数L,第三引数Uに格納する。
		
		inline ty    m_DetByU (const Matrix_t<ty>&) const;                       // LU分解によって求めたU(上三角行列)によって行列式を求める。
		inline void  m_Inverse(const Matrix_t<ty>&,Matrix_t<ty>&,ty=1e-5) const; // 逆行列を求める。
	

	public:

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

		enum class CalcOpeCode { Add, Sub, HadamardMul, ScalarMul };
		dim3 ThreadsPerBlock   {16,16};  // 初期値は{16,16}ですが適宜チューニングしてください。

#endif

		// 演算時にCUDAを使用する。
		bool UseCUDA = true;

		// 行列積で使用するスレッド数(初期値は最大のスレッド数)
		size_t thread = std::thread::hardware_concurrency();


		// MatrixConstructor.hpp
		Matrix ();
		Matrix (std::pair<size_t,size_t>); // サイズ指定
		Matrix (MatrixInit_t<ty>);         // {{0,0},{0,0}}...2*2行列
		Matrix (const Matrix_t<ty>&);      // std::vector<std::vector<ty>>
		Matrix (const Matrix&);            // コピーコンストラクタ
		~Matrix();                         // デストラクタ


		//MatrixOperator.hpp
		inline Matrix& operator =(MatrixInit_t<ty>);
		inline Matrix& operator =(const Matrix&);
		inline Matrix& operator =(Matrix&&);

		inline Matrix& operator +=(const Matrix&); // 加算
		inline Matrix& operator -=(const Matrix&); // 減算
		inline Matrix& operator ^=(const Matrix&); // 内積
		inline Matrix& operator *=(const Matrix&); // 行列積
		inline Matrix& operator *=(ty);            // スカラー倍

		inline Matrix  operator + (const Matrix&); // 加算
		inline Matrix  operator - (const Matrix&); // 減算
		inline Matrix  operator ^ (const Matrix&); // 内積
		inline Matrix  operator * (const Matrix&); // 行列積
		inline Matrix  operator * (ty);            // スカラー倍

		template<typename CastTy> operator Sanae::Matrix<CastTy>(); // キャスト

		inline Matrix& operator <<(Matrix&);         // 譲渡
		inline std::vector<ty>& operator [](size_t); // 要素へアクセス

		inline bool    operator ==(const Matrix&); // 比較 ==
		inline bool    operator !=(const Matrix&); // 比較 !=


		// MatrixUtil.hpp
		inline size_t Rows(); // 行数を取得
		inline size_t Cols(); // 列数を取得

		inline Matrix& SwapRow(size_t, size_t);    // 第一引数行と第二引数行を入れ替える。
		inline Matrix& SwapColumn(size_t, size_t); // 第一引数列と第二引数列を入れ替える。

		inline auto GetRowRef(size_t) -> std::vector<std::reference_wrapper<ty>>; // 1行分のデータを参照する可変長配列を返す。
		inline auto GetColRef(size_t) -> std::vector<std::reference_wrapper<ty>>; // 1列分のデータを参照する可変長配列を返す。

		inline Matrix& Resize(std::pair<size_t, size_t>); // サイズを変更する。

		inline Matrix& Setter     (std::function<ty()>);                  // 引数の関数を呼び出し各要素へ返り値を代入する。
		inline Matrix& Setter     (std::function<ty(size_t,size_t,ty&)>); // 引数の関数を呼び出し各要素へ返り値を代入する。(行数,列数,元の行列[行数][列数])を引数として受け取る。

		inline Matrix  Transpose  (); // 転置する。

		static inline Matrix<ty> Identity(size_t); // 指定された行=列数の単位行列を作成します。 
		
		static inline Matrix<ty> Zero    (size_t); // 指定された行,列数の0行列を作成します。 @ 正方行列
		static inline Matrix<ty> Zero    (size_t, size_t);


		// MatrixAdvCalc.hpp
		inline auto Det() -> typename std::enable_if<std::is_floating_point<ty>::value,double>::type;                // 行列式を求める。
		inline auto Inverse(ty=1e-5) -> typename std::enable_if<std::is_floating_point<ty>::value,Matrix<ty>>::type; // 逆行列を求める。

 	};

}


// 無効な行列を示す例外クラス
class InvalidMatrix : std::exception{
protected:

	std::string m_ErrorMessage = "It is an invalid matrix.";

public:

	InvalidMatrix() {}
	
	// メッセージを入力
	InvalidMatrix(const char* ArgMessage) : m_ErrorMessage(ArgMessage) {}

	// エラーメッセージを返す。
	const char* what() const noexcept override {return m_ErrorMessage.c_str();}

};


namespace Sanae {

	// 行列表示用
	const static std::streamsize DefaultWeight = 4;
	static       std::streamsize FontWeight = DefaultWeight;

}


// std::coutで出力
template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<
(
	std::basic_ostream<CharT, Traits>& ArgOstream,
	Sanae::Matrix<MatrixType> Matrix
)
{
	for (size_t Row = 0; Row < Matrix.Rows(); Row++) {
		for (MatrixType Column : Matrix[Row])
			ArgOstream << std::setw(Sanae::FontWeight) << Column;

		ArgOstream << std::endl;
	}

	return ArgOstream;
}


#endif