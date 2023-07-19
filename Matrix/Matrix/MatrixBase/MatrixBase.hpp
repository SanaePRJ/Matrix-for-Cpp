/*=============================================================
* NAME      : MatrixBase.hpp
* AUTHOR    : SanaeProject
* VER       : 2.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
* 
* プロトタイプ宣言
* 
* 注意:
* MatrixBaseImpl.hppをインクルードしてください。
=============================================================*/


#ifndef _SANAE_MATRIXBASE_HPP_
#define _SANAE_MATRIXBASE_HPP_


//Check Version C++14以上
#if (_cplusplus >= 201402L) || (defined(_MSVC_LANG)&&_MSVC_LANG >= 201402L)

//Is existing
#if __has_include(<vector>) && __has_include(<utility>) && __has_include(<stdexcept>) && __has_include(<iostream>)
#define _SANAE_MATRIXBASE_HPP_ALL_INCLUDE_


/*INCLUDE*/
#include <iostream>

#include <vector>
#include <utility>
#include <stdexcept>


/*DEFINE CLASS*/
using  Ulong = unsigned _int64;
using  Slong = signed   _int64;
using  SizeT = std::pair<Ulong, Ulong>;


//乗算時10^-6以下の誤差は無視する。
constexpr auto _ERROR_SANAE = 1e-12;


namespace Sanae {
	template<typename _T = double> class _MatrixBase {
		//Define Variables
	protected:
		std::vector<_T>   _Main;                         //行列を格納する。
		SizeT             _Size = std::make_pair(0, 0);  //行列のサイズを格納する。 first:列 second:行


		//Define Functions(protected)
	protected:
		//{列,行}を配列番号に変換する。
		Ulong _Convert_to_ArrayNum
		(
			Ulong, SizeT
		);

		//行列番号を{列,行}に変換する。
		SizeT _Convert_to_Position
		(
			Ulong, Ulong
		);

		//指定された行を入れ替えます。
		void _Swap_Line
		(
			Ulong, Ulong, std::vector<_T>*, SizeT
		);

		//指定した列を入れ替えます。
		void _Swap_Column
		(
			Ulong, Ulong, std::vector<_T>*, SizeT
		);

		//足し算を行います。
		void _Add
		(
			const _MatrixBase&, std::vector<_T>*, SizeT
		);

		//引き算を行います。
		void _Sub
		(
			const _MatrixBase&, std::vector<_T>*, SizeT
		);

		//スカラー倍を行います。
		void _Scalar_mul
		(
			std::vector<_T>&, _T
		);

		//行列の乗算を行います。
		void _Mul
		(
			std::vector<_T>&, SizeT,
			std::vector<_T>&, SizeT,
			std::vector<_T>*, SizeT*
		);


		//Define Functions(public)
	public:
		//Constructor
		_MatrixBase();
		_MatrixBase
		(
			std::initializer_list<std::vector<_T>>
		);
		_MatrixBase
		(
			SizeT
		);
		_MatrixBase
		(
			std::pair<SizeT, std::vector<_T>> _In
		);

		//Operator
		//比較
		bool operator==
		(
			const _MatrixBase<_T>&
		);

		//配列参照
		_T& operator[]
		(
			Ulong
		);
		//{列,行}参照
		_T& operator[]
		(
			SizeT
		);

	};
}


#endif //Is existing
#endif //Check Version
#endif //Include Guard