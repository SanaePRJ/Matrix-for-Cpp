/*=============================================================
* NAME      : MatrixBase.hpp
* AUTHOR    : SanaeProject
* VER       : 3.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
=============================================================*/




/*-----Include Guard-----*/
#ifndef SANAE_MATRIXBASE_HPP
#define SANAE_MATRIXBASE_HPP




/*-----Check Version C++14 or more.-----*/
#if (__cplusplus >= 201402L) || (defined(_MSVC_LANG)&&_MSVC_LANG >= 201402L)
	#define SANAE_CPP_14_OR_MORE
#endif




/*-----Is existing-----*/
#if __has_include(<iostream>) && __has_include(<vector>) && __has_include(<utility>) && __has_include(<stdexcept>)
	#define SANAE_MATRIXBASE_HPP_ALL_INCLUDE
#endif


#if defined(SANAE_CPP_14_OR_MORE) && defined(SANAE_MATRIXBASE_HPP_ALL_INCLUDE)


/*-----Include-----*/
#include <iostream>

#include <vector>
#include <utility>
#include <stdexcept>




/*-----DEFINE CLASS-----*/
using  Ulong = unsigned long long;         //Unsigned long long
using  Slong = signed   long long;         //Signed   long long
using  SizeT = std::pair<Ulong, Ulong>;    //Size


namespace Sanae {
	/*----------------------------------------------
	* Ignore errors less than 10^-6 during multiplication.
	* 乗算時10^-6以下の誤差を無視する。
	----------------------------------------------*/
	constexpr auto IGNORE_ERROR = 1e-6;


	template<typename T> class MatrixBase;
}




/*-------------------------------------------------------------
* Define Class
*
* Author:Sanae
-------------------------------------------------------------*/


template<typename T> class Sanae::MatrixBase 
{
	

/*-----Define Variable(protected)-----*/
protected:


	/*----------------------------------------------
	* Store a Matrix.
	* 行列を格納する可変長行列
	----------------------------------------------*/
	std::vector<T>   m_Main;

	/*----------------------------------------------
	* Store a size of Matrix. first:Column second:Row
	* 行列のサイズを格納する。first:列     second:行
	----------------------------------------------*/
	SizeT             m_Size = std::make_pair(0, 0);  //行列のサイズを格納する。 first:列 second:行


/*-----Define Functions(protected)-----*/
protected:


	/*----------------------------------------------
	* Convert {Columns , Rows} to array numbers.
	* {列,行}を配列番号に変換する。
	----------------------------------------------*/
	Ulong m_Convert_to_ArrayNum
	(
		Ulong, SizeT
	);

	/*----------------------------------------------
	* Convert array number to {Columns , Rows}.
	* 配列番号を{列,行}に変換する。
	----------------------------------------------*/
	SizeT m_Convert_to_Position
	(
		Ulong, Ulong
	);

	/*----------------------------------------------
	* Swaps the specified rows.
	* 指定された行を入れ替えます。
	----------------------------------------------*/
	void m_Swap_Line
	(
		Ulong, Ulong, std::vector<T>*, SizeT
	);

	/*----------------------------------------------
	* Swaps the specified column.
	* 指定された列を入れ替えます。
	----------------------------------------------*/
	void m_Swap_Column
	(
		Ulong, Ulong, std::vector<T>*, SizeT
	);

	/*----------------------------------------------
	* Addition.
	* 足し算を行います。
	----------------------------------------------*/
	void m_Add
	(
		const MatrixBase&, std::vector<T>*, SizeT
	);
	
	/*----------------------------------------------
	* Subtraction.
	* 引き算を行います。
	----------------------------------------------*/
	void m_Sub
	(
		const MatrixBase&, std::vector<T>*, SizeT
	);

	/*----------------------------------------------
	* Scalar Multiplication.
	* スカラー倍を行います。
	----------------------------------------------*/
	void m_Scalar_mul
	(
		std::vector<T>&, T
	);

	/*----------------------------------------------
	* Multiplication.
	* 乗算を行います。
	----------------------------------------------*/
	void m_Mul
	(
		std::vector<T>&, SizeT,
		std::vector<T>&, SizeT,
		std::vector<T>*, SizeT*
	);


/*-----Define Functions(public)-----*/
public:


	/*-----Constructor-----*/
	MatrixBase();
	MatrixBase
	(
		std::initializer_list<std::vector<T>>
	);
	MatrixBase
	(
		SizeT
	);
	MatrixBase
	(
		std::pair<SizeT, std::vector<T>> _In
	);

	/*-----Operator-----*/
	/*----------------------------------------------
	* Comparison
	* 比較
	----------------------------------------------*/
	bool operator==
	(
		const MatrixBase<T>&
	);

	/*----------------------------------------------
	* Refer by array number.
	* 配列番号で参照する。
	----------------------------------------------*/
	T& operator[]
	(
		Ulong
	);

	/*----------------------------------------------
	* Refer by {Columns,Rows}.
	* {列,行}で参照する。
	----------------------------------------------*/
	T& operator[]
	(
		SizeT
	);


};




/*-------------------------------------------------------------
* Implementation.
*
* Author:Sanae
-------------------------------------------------------------*/




/*------Function(protected)------*/
/*----------------------------------------------
* Store a Matrix.
* 行列を格納する可変長行列
----------------------------------------------*/
template<typename T> Ulong Sanae::MatrixBase<T>::m_Convert_to_ArrayNum
(
	Ulong a_Width,
	SizeT a_Pos
)
{
	return (a_Width * a_Pos.second) + a_Pos.first;
}

/*----------------------------------------------
* Convert array number to {Columns , Rows}.
* 配列番号を{列,行}に変換する。
----------------------------------------------*/
template<typename T> SizeT Sanae::MatrixBase<T>::m_Convert_to_Position
(
	Ulong a_Width,
	Ulong a_Arraynum
)
{
	return std::make_pair(a_Arraynum % a_Width, (a_Arraynum - (a_Arraynum % a_Width)) / a_Width);
}

/*----------------------------------------------
* Swaps the specified rows.
* 指定された行を入れ替えます。
----------------------------------------------*/
template<typename T> void Sanae::MatrixBase<T>::m_Swap_Line
(
	Ulong             a_Line1,
	Ulong             a_Line2,
	std::vector<T>*   a_Data,
	SizeT             a_Size
)
{
	for (Ulong i = 0; i < a_Size.first; i++) {
		T _temp = (*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { i,a_Line1 })];

		(*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { i,a_Line1 })] = (*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { i,a_Line2 })];
		(*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { i,a_Line2 })] = _temp;
	}

	return;
}

/*----------------------------------------------
* Swaps the specified column.
* 指定された列を入れ替えます。
----------------------------------------------*/
template<typename T> void Sanae::MatrixBase<T>::m_Swap_Column
(
	Ulong            a_Column1,
	Ulong            a_Column2,
	std::vector<T>*  a_Data,
	SizeT            a_Size
)
{
	for (Ulong i = 0; i < a_Size.second; i++) {
		T _temp = (*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { a_Column1,i })];

		(*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { a_Column1,i })] = (*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { a_Column2,i })];
		(*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { a_Column2,i })] = _temp;
	}

	return;
}


/*------Functions(public)------*/
/*---Constructor---*/
template<typename T> Sanae::MatrixBase<T>::MatrixBase()
{}
template<typename T> Sanae::MatrixBase<T>::MatrixBase
(
	std::initializer_list<std::vector<T>> a_Init
)
{
	std::vector<std::vector<T>> _buf = a_Init;

	this->m_Size.second = a_Init.size();
	this->m_Size.first = _buf[0].size();

	for (Ulong line = 0; line < this->m_Size.second; line++)
		this->m_Main.insert(this->m_Main.end(), _buf[line].begin(), _buf[line].end());

	return;
}
template<typename T> Sanae::MatrixBase<T>::MatrixBase
(
	SizeT a_Init_Size
)
	: m_Size(a_Init_Size)
{
	this->m_Main.resize(a_Init_Size.first * a_Init_Size.second);
}
template<typename T> Sanae::MatrixBase<T>::MatrixBase
(
	std::pair<SizeT, std::vector<T>> a_Init
)
{
	this->m_Size = a_Init.first;
	this->m_Main = a_Init.second;

	for (Ulong i = this->m_Main.size(); i < (this->m_Size.first * this->m_Size.second); i++)
		this->m_Main[i] = 0;

	return;
}

/*-----Operator-----*/
/*----------------------------------------------
* Comparison
* 比較
----------------------------------------------*/
template<typename T> bool Sanae::MatrixBase<T>::operator==
(
	const MatrixBase<T>& a_Data
)
{
	if (a_Data.m_Size.first != this->m_Size.first || a_Data.m_Size.second != this->m_Size.second)
		return false;

	for (Ulong i = 0; i < this->m_Main.size(); i++)
		if (a_Data.m_Main[i] != this->m_Main[i])
			return false;

	return true;
}

/*----------------------------------------------
* Refer by array number.
* 配列番号で参照する。
----------------------------------------------*/
template<typename T> T& Sanae::MatrixBase<T>::operator[]
(
	Ulong a_ArrayNum
)
{
	if (this->m_Main.size() <= a_ArrayNum)
		throw std::out_of_range("Tried to access out of range.");

	return this->m_Main[a_ArrayNum];
}

/*----------------------------------------------
* Refer by {Columns,Rows}.
* {列,行}で参照する。
----------------------------------------------*/
template<typename T> T& Sanae::MatrixBase<T>::operator[]
(
	SizeT a_Num
)
{
	if (this->m_Main.size() <= this->m_Convert_to_ArrayNum(this->m_Size.first, a_Num))
		throw std::out_of_range("Tried to access out of range.");

	return this->m_Main[this->m_Convert_to_ArrayNum(this->m_Size.first, a_Num)];
}

/*---Calc---*/
/*----------------------------------------------
* Addition.
* 足し算を行います。
----------------------------------------------*/
template<typename T> void Sanae::MatrixBase<T>::m_Add
(
	const MatrixBase& a_Data,
	std::vector<T>*   a_DataP,
	SizeT             a_Size
)
{
	if (a_DataP->size() != a_Data.m_Main.size() || a_Size.first != a_Data.m_Size.first || a_Size.second != a_Data.m_Size.second)
		throw std::invalid_argument("Must be same size.");

	for (Ulong i = 0; i < a_DataP->size(); i++)
		(*a_DataP)[i] += a_Data.m_Main[i];

	return;
}

/*----------------------------------------------
* Subtraction.
* 引き算を行います。
----------------------------------------------*/
template<typename T> void  Sanae::MatrixBase<T>::m_Sub
(
	const MatrixBase& a_Data,
	std::vector<T>*	  a_DataP,
	SizeT             a_Size
)
{
	if (a_DataP->size() != a_Data.m_Main.size() || a_Size.first != a_Data.m_Size.first || a_Size.second != a_Data.m_Size.second)
		throw std::invalid_argument("Must be same size.");

	for (Ulong i = 0; i < a_DataP->size(); i++)
		(*a_DataP)[i] -= a_Data.m_Main[i];

	return;
}

/*----------------------------------------------
* Scalar Multiplication.
* スカラー倍を行います。
----------------------------------------------*/
template<typename T> void Sanae::MatrixBase<T>::m_Scalar_mul
(
	std::vector<T>& a_Data,
	T               a_Mul_num
)
{
	for (Ulong i = 0; i < a_Data.size(); i++)
		a_Data[i] *= a_Mul_num;

	return;
}

/*----------------------------------------------
* Multiplication.
* 乗算を行います。
----------------------------------------------*/
template<typename T> void Sanae::MatrixBase<T>::m_Mul
(
	std::vector<T>& a_Data1,
	SizeT           a_Size1,

	std::vector<T>& a_Data2,
	SizeT           a_Size2,

	std::vector<T>* a_Storage,
	SizeT*          a_SizeP
)
{
	if (a_Size1.first != a_Size2.second) //列と行が同じでなければならない。
		throw std::invalid_argument("Must be same line and column.");

	a_Storage->erase(a_Storage->begin(), a_Storage->end());
	a_Storage->resize(a_Size1.second * a_Size2.first);

	(*a_SizeP) = { a_Size2.first,a_Size1.second }; //計算結果格納先

	//{列,行}の乗算結果を求めます。
	auto mul = [&](Ulong _i, Ulong _j) {
		T num = 0;

		for (Ulong k = 0; k < a_Size2.second; k++)
			num += a_Data1[this->m_Convert_to_ArrayNum(a_Size1.first, { k,_i })] * a_Data2[this->m_Convert_to_ArrayNum(a_Size2.first, { _j,k })];

		//誤差の修正
		(*a_Storage)[this->m_Convert_to_ArrayNum(a_SizeP->first, { _j,_i })] = abs(num - (Slong)num) <= IGNORE_ERROR ? (Slong)num : num;

		return;
	};

	//{列,行}の乗算処理を行う。
	for (Ulong i = 0; i < a_Size1.second; i++) {
		for (Ulong j = 0; j < a_Size2.first; j++) {
			mul(i, j);
		}
	}

	return;
}




#endif
#endif