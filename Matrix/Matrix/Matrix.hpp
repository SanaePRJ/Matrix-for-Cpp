/*=============================================================
* NAME      : Matrix.hpp
* AUTHOR    : SanaeProject
* VER       : 2.0.2
* COPYRIGHGT: Copyright 2023 SanaeProject.
* 
* プロトタイプ宣言
* 
* 注意:
* MatrixImpl.hppをインクルードしてください。
=============================================================*/


#ifndef _SANAE_MATRIX_HPP_
#define _SANAE_MATRIX_HPP_


#include "MatrixBase/MatrixBase.hpp"
#include "MatrixBase/MatrixBaseImpl.hpp"


#if defined(_SANAE_MATRIXBASE_HPP_ALL_INCLUDE_)


namespace Sanae {
	class Matrix :public _MatrixBase<double>
	{
		//Define Functions(private)
	private:
		//初期値をstd::vector<_T>へ変換する。
		void _Convert_to_Array
		(
			std::initializer_list<std::vector<double>>&,
			std::vector<double>&
		);

		//行列を表示する。
		void _View
		(
			std::vector<double>*,
			SizeT
		);
		//単位行列にします。
		void _To_Identity_Matrix
		(
			std::vector<double>*,
			SizeT
		);

		//行列式関係
		//サラスの方式で解きます。(2次元)
		double _Det_2
		(
			std::vector<double>*,
			double,
			SizeT
		);
		//余因子展開をして行列を一次元下げます。下げた値は_Storageに格納され係数は_Coeffに格納されます。
		void _Confactor_expansion_1
		(
			std::vector<double>&,
			SizeT,
			std::vector<std::vector<double>>*,
			std::vector<double>*
		);
		//2次元になるまで余因子展開を行います。
		std::pair<std::vector<std::vector<double>>, std::vector<double>>
			_Confactor_expansion_to_2
			(
				std::vector<double>&,
				SizeT,
				double
			);
		//行列式を求めます。
		double _Det
		(
			std::vector<double>&,
			SizeT
		);

		//逆行列を求めます。
		void _Inverse_matrix
		(
			std::vector<double>*,
			std::vector<double>*,
			SizeT
		);

		//内積を求めます。
		void _Inner_Product
		(
			std::vector<double>*,
			SizeT*,

			std::vector<double>*,
			SizeT*,

			std::vector<double>*,

			bool
		);

		//Define Functions(public)
	public:
		//Constructor
		using _MatrixBase::_MatrixBase;

		//代入
		Matrix& operator=
		(
			const Matrix&
		);
		Matrix& operator=
		(
			std::initializer_list<std::vector<double>>
		);

		//破壊的処理
		Matrix& operator+=
		(
			const Matrix&
		);
		Matrix& operator-=
		(
			const Matrix&
		);
		Matrix& operator*=
		(
			const Matrix&
		);
		Matrix& operator*=
		(
			double
		);

		//非破壊的処理
		Matrix  operator+
		(
			const Matrix&
		);
		Matrix  operator-
		(
			const Matrix&
		);
		Matrix  operator*
		(
			const Matrix&
		);
		Matrix  operator*
		(
			double
		);

		//(破壊的処理)サイズ変更(データはすべて削除されます。)
		Matrix& SetSize
		(
			SizeT,
			bool
		);

		//(破壊的処理)行を入れ替える。
		Matrix& Swap_Line
		(
			Ulong,
			Ulong
		);
		//(破壊的処理)列を入れ替える。
		Matrix& Swap_Column
		(
			Ulong,
			Ulong
		);

		//(破壊的処理)単位行列にする。
		Matrix& Ident();
		//(非破壊的処理)行列式を求めます。 
		double Det();
		//(非破壊的処理)行列の転置を行います。
		Matrix Transpose();
		//(非破壊的処理)逆行列を返します。
		Matrix Inverse();
		
		//行列を表示します。
		Matrix& View();

		//大きさを返します。
		Ulong GetSize();
		//大きさを返します。 first:列 second:行
		SizeT GetSizeWH();

		//(非破壊的処理)内積を求めます。
		Matrix& Inner_Product
		(
			Matrix&
		);
		//(破壊的処理)内積を求めます。
		Matrix Inner_Product_Destructive
		(
			Matrix&
		);

		Matrix& Move
		(
			std::vector<double>*,
			SizeT
		);

		//_Mainのポインタを返します。(std::max_elementなどの関数で使用する用)
		const std::vector<double>* GetVectorP();
	};
}

#endif
#endif