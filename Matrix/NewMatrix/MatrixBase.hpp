/*=============================================================
* NAME      : MatrixBase.hpp
* AUTHOR    : SanaeProject
* VER       : 3.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
=============================================================*/




/*-----Include Guard-----*/
#ifndef _SANAE_MATRIXBASE_HPP_
#define _SANAE_MATRIXBASE_HPP_




/*-----Check Version C++14 or more.-----*/
#if (_cplusplus >= 201402L) || (defined(_MSVC_LANG)&&_MSVC_LANG >= 201402L)
	#define _CPP_14_OR_MORE_
#endif




/*-----Is existing-----*/
#if __has_include(<iostream>) && __has_include(<vector>) && __has_include(<utility>) && __has_include(<stdexcept>)
	#define _SANAE_MATRIXBASE_HPP_ALL_INCLUDE_
#endif




#if defined(_CPP_14_OR_MORE_) && defined(_SANAE_MATRIXBASE_HPP_ALL_INCLUDE_)

/*-----Include-----*/
#include <iostream>

#include <vector>
#include <utility>
#include <stdexcept>




/*-----DEFINE CLASS-----*/
using  Ulong = unsigned _int64;         //Unsigned long long
using  Slong = signed   _int64;         //Signed   long long
using  SizeT = std::pair<Ulong, Ulong>; //Size


namespace Sanae {
	/*----------------------------------------------
	* Ignore errors less than 10^-6 during multiplication.
	* 乗算時10^-6以下の誤差を無視する。
	----------------------------------------------*/
	constexpr auto _IGNORE_ERROR_ = 1e-6;


	template<typename _T> class _MatrixBase;
}




/*-------------------------------------------------------------
* Define Class
*
* Author:Sanae
-------------------------------------------------------------*/


template<typename _T> class Sanae::_MatrixBase 
{
	

/*-----Define Variable(protected)-----*/
protected:


	/*----------------------------------------------
	* Store a Matrix.
	* 行列を格納する可変長行列
	----------------------------------------------*/
	std::vector<_T>   _Main;

	/*----------------------------------------------
	* Store a size of Matrix. first:Column second:Row
	* 行列のサイズを格納する。first:列     second:行
	----------------------------------------------*/
	SizeT             _Size = std::make_pair(0, 0);  //行列のサイズを格納する。 first:列 second:行


/*-----Define Functions(protected)-----*/
protected:


	/*----------------------------------------------
	* Convert {Columns , Rows} to array numbers.
	* {列,行}を配列番号に変換する。
	----------------------------------------------*/
	Ulong _Convert_to_ArrayNum
	(
		Ulong, SizeT
	);

	/*----------------------------------------------
	* Convert array number to {Columns , Rows}.
	* 配列番号を{列,行}に変換する。
	----------------------------------------------*/
	SizeT _Convert_to_Position
	(
		Ulong, Ulong
	);

	/*----------------------------------------------
	* Swaps the specified rows.
	* 指定された行を入れ替えます。
	----------------------------------------------*/
	void _Swap_Line
	(
		Ulong, Ulong, std::vector<_T>*, SizeT
	);

	/*----------------------------------------------
	* Swaps the specified column.
	* 指定された列を入れ替えます。
	----------------------------------------------*/
	void _Swap_Column
	(
		Ulong, Ulong, std::vector<_T>*, SizeT
	);

	/*----------------------------------------------
	* Addition.
	* 足し算を行います。
	----------------------------------------------*/
	void _Add
	(
		const _MatrixBase&, std::vector<_T>*, SizeT
	);
	
	/*----------------------------------------------
	* Subtraction.
	* 引き算を行います。
	----------------------------------------------*/
	void _Sub
	(
		const _MatrixBase&, std::vector<_T>*, SizeT
	);

	/*----------------------------------------------
	* Scalar Multiplication.
	* スカラー倍を行います。
	----------------------------------------------*/
	void _Scalar_mul
	(
		std::vector<_T>&, _T
	);

	/*----------------------------------------------
	* Multiplication.
	* 乗算を行います。
	----------------------------------------------*/
	void _Mul
	(
		std::vector<_T>&, SizeT,
		std::vector<_T>&, SizeT,
		std::vector<_T>*, SizeT*
	);


/*-----Define Functions(public)-----*/
public:


	/*-----Constructor-----*/
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

	/*-----Operator-----*/
	/*----------------------------------------------
	* Comparison
	* 比較
	----------------------------------------------*/
	bool operator==
	(
		const _MatrixBase<_T>&
	);

	/*----------------------------------------------
	* Refer by array number.
	* 配列番号で参照する。
	----------------------------------------------*/
	_T& operator[]
	(
		Ulong
	);

	/*----------------------------------------------
	* Refer by {Columns,Rows}.
	* {列,行}で参照する。
	----------------------------------------------*/
	_T& operator[]
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
template<typename _T> Ulong Sanae::_MatrixBase<_T>::_Convert_to_ArrayNum
(
	Ulong _Width,
	SizeT _Pos
)
{
	return (_Width * _Pos.second) + _Pos.first;
}

/*----------------------------------------------
* Convert array number to {Columns , Rows}.
* 配列番号を{列,行}に変換する。
----------------------------------------------*/
template<typename _T> SizeT Sanae::_MatrixBase<_T>::_Convert_to_Position
(
	Ulong _Width,
	Ulong _Arraynum
)
{
	return std::make_pair(_Arraynum % _Width, (_Arraynum - (_Arraynum % _Width)) / _Width);
}

/*----------------------------------------------
* Swaps the specified rows.
* 指定された行を入れ替えます。
----------------------------------------------*/
template<typename _T> void Sanae::_MatrixBase<_T>::_Swap_Line
(
	Ulong             _Line1,
	Ulong             _Line2,
	std::vector<_T>* _Data,
	SizeT             _Size
)
{
	for (Ulong i = 0; i < _Size.first; i++) {
		_T _temp = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,_Line1 })];

		(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,_Line1 })] = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,_Line2 })];
		(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,_Line2 })] = _temp;
	}

	return;
}

/*----------------------------------------------
* Swaps the specified column.
* 指定された列を入れ替えます。
----------------------------------------------*/
template<typename _T> void Sanae::_MatrixBase<_T>::_Swap_Column
(
	Ulong            _Column1,
	Ulong            _Column2,
	std::vector<_T>* _Data,
	SizeT            _Size
)
{
	for (Ulong i = 0; i < _Size.second; i++) {
		_T _temp = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _Column1,i })];

		(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _Column1,i })] = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _Column2,i })];
		(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _Column2,i })] = _temp;
	}

	return;
}


/*------Functions(public)------*/
/*---Constructor---*/
template<typename _T> Sanae::_MatrixBase<_T>::_MatrixBase()
{}
template<typename _T> Sanae::_MatrixBase<_T>::_MatrixBase
(
	std::initializer_list<std::vector<_T>> _In
)
{
	std::vector<std::vector<_T>> _buf = _In;

	this->_Size.second = _In.size();
	this->_Size.first = _buf[0].size();

	for (Ulong line = 0; line < this->_Size.second; line++)
		this->_Main.insert(this->_Main.end(), _buf[line].begin(), _buf[line].end());

	return;
}
template<typename _T> Sanae::_MatrixBase<_T>::_MatrixBase
(
	SizeT _In_Size
)
	: _Size(_In_Size)
{
	this->_Main.resize(_In_Size.first * _In_Size.second);
}
template<typename _T> Sanae::_MatrixBase<_T>::_MatrixBase
(
	std::pair<SizeT, std::vector<_T>> _In
)
{
	this->_Size = _In.first;
	this->_Main = _In.second;

	for (Ulong i = this->_Main.size(); i < (this->_Size.first * this->_Size.second); i++)
		this->_Main[i] = 0;

	return;
}

/*-----Operator-----*/
/*----------------------------------------------
* Comparison
* 比較
----------------------------------------------*/
template<typename _T> bool Sanae::_MatrixBase<_T>::operator==
(
	const _MatrixBase<_T>& _Data
)
{
	if (_Data._Size.first != this->_Size.first || _Data._Size.second != this->_Size.second)
		return false;

	for (Ulong i = 0; i < this->_Main.size(); i++)
		if (_Data._Main[i] != this->_Main[i])
			return false;

	return true;
}

/*----------------------------------------------
* Refer by array number.
* 配列番号で参照する。
----------------------------------------------*/
template<typename _T> _T& Sanae::_MatrixBase<_T>::operator[]
(
	Ulong _ArrayNum
)
{
	if (this->_Main.size() <= _ArrayNum)
		throw std::out_of_range("Tried to access out of range.");

	return this->_Main[_ArrayNum];
}

/*----------------------------------------------
* Refer by {Columns,Rows}.
* {列,行}で参照する。
----------------------------------------------*/
template<typename _T> _T& Sanae::_MatrixBase<_T>::operator[]
(
	SizeT _Num
)
{
	if (this->_Main.size() <= this->_Convert_to_ArrayNum(this->_Size.first, _Num))
		throw std::out_of_range("Tried to access out of range.");

	return this->_Main[this->_Convert_to_ArrayNum(this->_Size.first, _Num)];
}

/*---Calc---*/
/*----------------------------------------------
* Addition.
* 足し算を行います。
----------------------------------------------*/
template<typename _T> void Sanae::_MatrixBase<_T>::_Add
(
	const _MatrixBase& _Data,
	std::vector<_T>* _DataP,
	SizeT            _Size
)
{
	if (_DataP->size() != _Data._Main.size() || _Size.first != _Data._Size.first || _Size.second != _Data._Size.second)
		throw std::invalid_argument("Must be same size.");

	for (Ulong i = 0; i < _DataP->size(); i++)
		(*_DataP)[i] += _Data._Main[i];

	return;
}

/*----------------------------------------------
* Subtraction.
* 引き算を行います。
----------------------------------------------*/
template<typename _T> void  Sanae::_MatrixBase<_T>::_Sub
(
	const _MatrixBase& _Data,
	std::vector<_T>* _DataP,
	SizeT              _Size
)
{
	if (_DataP->size() != _Data._Main.size() || _Size.first != _Data._Size.first || _Size.second != _Data._Size.second)
		throw std::invalid_argument("Must be same size.");

	for (Ulong i = 0; i < _DataP->size(); i++)
		(*_DataP)[i] -= _Data._Main[i];

	return;
}

/*----------------------------------------------
* Scalar Multiplication.
* スカラー倍を行います。
----------------------------------------------*/
template<typename _T> void Sanae::_MatrixBase<_T>::_Scalar_mul
(
	std::vector<_T>& _Data,
	_T               _Mul_num
)
{
	for (Ulong i = 0; i < _Data.size(); i++)
		_Data[i] *= _Mul_num;

	return;
}

/*----------------------------------------------
* Multiplication.
* 乗算を行います。
----------------------------------------------*/
template<typename _T> void Sanae::_MatrixBase<_T>::_Mul
(
	std::vector<_T>& _Data1,
	SizeT            _Size1,

	std::vector<_T>& _Data2,
	SizeT            _Size2,

	std::vector<_T>* _Storage,
	SizeT* _SizeP
)
{
	if (_Size1.first != _Size2.second) //列と行が同じでなければならない。
		throw std::invalid_argument("Must be same line and column.");

	_Storage->erase(_Storage->begin(), _Storage->end());
	_Storage->resize(_Size1.second * _Size2.first);

	(*_SizeP) = { _Size2.first,_Size1.second }; //計算結果格納先

	//{列,行}の乗算結果を求めます。
	auto mul = [&](Ulong _i, Ulong _j) {
		_T num = 0;

		for (Ulong k = 0; k < _Size2.second; k++)
			num += _Data1[this->_Convert_to_ArrayNum(_Size1.first, { k,_i })] * _Data2[this->_Convert_to_ArrayNum(_Size2.first, { _j,k })];

		//誤差の修正
		(*_Storage)[this->_Convert_to_ArrayNum(_SizeP->first, { _j,_i })] = abs(num - (Slong)num) <= _IGNORE_ERROR_ ? (Slong)num : num;

		return;
	};

	//{列,行}の乗算処理を行う。
	for (Ulong i = 0; i < _Size1.second; i++) {
		for (Ulong j = 0; j < _Size2.first; j++) {
			mul(i, j);
		}
	}

	return;
}




#endif
#endif