/*=============================================================
* NAME      : Matrix.hpp
* AUTHOR    : SanaeProject
* VER       : 3.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
=============================================================*/




/*-----Include Guard-----*/
#ifndef SANAE_MATRIX_HPP
#define SANAE_MATRIX_HPP




/*-----Include-----*/
#include "MatrixBase.hpp"
#include <math.h>


/*-----Check Version C++14 or more and Is existing-----*/
#if defined(SANAE_CPP_14_OR_MORE) && defined(SANAE_MATRIXBASE_HPP_ALL_INCLUDE)




/*-----Define Class-----*/
namespace Sanae {
	 template<typename T> class MatrixT;

	 using Matrix      = MatrixT<double>;
	 using Matrix_Lite = MatrixT<float >;
}




/*-------------------------------------------------------------
* Define Class
*
* Author:Sanae
-------------------------------------------------------------*/


template<typename T> class Sanae::MatrixT :public Sanae::MatrixBase<T>
{


	/*-----Define Variable(protected)-----*/
protected:


	/*----------------------------------------------
	* Convert the initial value to std::vector<_T>.
	* 初期値をstd::vector<_T>へ変換します。
	----------------------------------------------*/
	void m_Convert_to_Array
	(
		std::initializer_list<std::vector<T>>&,
		std::vector<T>&
	);

	/*----------------------------------------------
	* Display the matrix.
	* 行列を表示します。
	----------------------------------------------*/
	void m_View
	(
		std::vector<T>*,
		SizeT
	);

	/*----------------------------------------------
	* Identity matrix.
	* 単位行列にします。
	----------------------------------------------*/
	void m_To_Identity_Matrix
	(
		std::vector<T>*,
		SizeT
	);


	/*-----Determinant-----*/
	/*----------------------------------------------
	* Find the determinant using Salas' method. (2D)
	* サラスの方式で行列式を求めます。(2次元)
	----------------------------------------------*/
	T m_Det_2
	(
		std::vector<T>*,
		T,
		SizeT
	);

	/*----------------------------------------------
	* Expand the cofactors to lower the matrix by one dimension.
	The lowered value is stored in _Storage and the coefficient is stored in _Coeff.
	* 余因子展開をして行列を一次元下げます。
	下げた値は_Storageに格納され係数は_Coeffに格納されます。
	----------------------------------------------*/
	void m_Confactor_expansion_1
	(
		std::vector<T>&,
		SizeT,
		std::vector<std::vector<T>>*,
		std::vector<T>*
	);

	/*----------------------------------------------
	* Perform cofactor expansion until it becomes two-dimensional.
	* 二次元になるまで余因子展開を行います。
	----------------------------------------------*/
	std::pair<std::vector<std::vector<T>>, std::vector<T>>
		m_Confactor_expansion_to_2
		(
			std::vector<T>&,
			SizeT,
			T = 1
		);

	/*----------------------------------------------
	* Find the determinant.
	* 行列式を求めます。
	----------------------------------------------*/
	T m_Det
	(
		std::vector<T>&,
		SizeT
	);

	/*----------------------------------------------
	* Find the inverse of the matrix.
	* 逆行列を求めます。
	----------------------------------------------*/
	void m_Inverse_matrix
	(
		std::vector<T>*,
		std::vector<T>*,
		SizeT
	);

	/*----------------------------------------------
	* Find the inner product.
	* 内積を求めます。
	----------------------------------------------*/
	void m_Inner_Product
	(
		std::vector<T>*,
		SizeT*,

		std::vector<T>*,
		SizeT*,

		std::vector<T>*,

		bool
	);


	/*-----Define Functions(public)-----*/
public:


	/*-----Constructor-----*/
	using MatrixBase<T>::MatrixBase;


	/*----------------------------------------------
	* Assignment.
	* 代入
	----------------------------------------------*/
	MatrixT& operator=
	(
		const MatrixT&
	);
	MatrixT& operator=
	(
		const std::initializer_list<std::vector<T>>
	);

	/*----------------------------------------------
	* Destructive processing
	* 破壊的処理
	----------------------------------------------*/
	/*-ADD-*/
	MatrixT& operator+=
	(
		const MatrixT&
	);
	/*-SUB-*/
	MatrixT& operator-=
	(
		const MatrixT&
	);
	/*-MUL-*/
	MatrixT& operator*=
	(
		const MatrixT&
	);
	/*-Scalar MUL-*/
	MatrixT& operator*=
	(
		T
	);
	/*-INNER PRODUCT-*/
	MatrixT& operator^=
	(
		const MatrixT&
	);

	/*----------------------------------------------
	*  Non-destructive processing.
	* 非破壊的処理
	----------------------------------------------*/
	/*-ADD-*/
	MatrixT  operator+
	(
		const MatrixT&
	);
	/*-SUB-*/
	MatrixT  operator-
	(
		const MatrixT&
	);
	/*-MUL-*/
	MatrixT  operator*
	(
		const MatrixT&
	);
	/*-Scalar MUL-*/
	MatrixT  operator*
	(
		T
	);
	/*-INNER PRODUCT-*/
	MatrixT operator^
	(
		const MatrixT&
	);

	/*----------------------------------------------
	* (Destructive processing) Resize.
	* (破壊的処理)サイズ変更
	----------------------------------------------*/
	MatrixT& SetSize
	(
		SizeT,
		bool = true
	);

	/*----------------------------------------------
	* (Destructive processing) Swap lines.
	* (破壊的処理)行と行を入れ替える。
	----------------------------------------------*/
	MatrixT& Swap_Line
	(
		Ulong,
		Ulong
	);

	/*----------------------------------------------
	* (Destructive processing) Swap Columns.
	* (破壊的処理)列を入れ替える。
	----------------------------------------------*/
	MatrixT& Swap_Column
	(
		Ulong,
		Ulong
	);

	/*----------------------------------------------
	* (Destructive processing) 
	Make it an identity matrix.
	* (破壊的処理)単位行列にする。
	----------------------------------------------*/
	MatrixT& Ident();

	/*----------------------------------------------
	* (Non-destructive processing) Find the determinant.
	* (非破壊的処理)行列式を求めます。
	----------------------------------------------*/
	T Det();

	/*----------------------------------------------
	* (Non-destructive processing) Transposes a matrix.
	* (非破壊的処理)行列の転置を行います。
	----------------------------------------------*/
	MatrixT Transpose();

	/*----------------------------------------------
	* (Non-destructive processing) Returns the inverse of the matrix.
	* (非破壊的処理)逆行列を返します。
	----------------------------------------------*/
	MatrixT Inverse();

	/*----------------------------------------------
	* Display the matrix.
	* 行列を表示します。
	----------------------------------------------*/
	MatrixT& View();

	/*----------------------------------------------
	* Returns the size.
	* 大きさを返します。
	----------------------------------------------*/
	Ulong GetSize();
	
	/*----------------------------------------------
	* Returns the size.  first:column second: row
	* 大きさを返します。 first:列     second: 行
	----------------------------------------------*/
	SizeT GetSizeWH();

	/*----------------------------------------------
	* Transfer an array.
	* 配列を譲渡します。
	----------------------------------------------*/
	MatrixT& Move
	(
		std::vector<T>*,
		SizeT
	);

	/*----------------------------------------------
	* Returns a pointer to m_Main.
	(Can be used in functions such as std::max_element.)
	* m_Mainのポインタを返します。
	(std::max_elementなどの関数で使用することができます。)
	----------------------------------------------*/
	const std::vector<T>* GetVectorP();

	/*----------------------------------------------
	* Returns a pointer to _Main.
	(Can be used in functions such as std::max_element.)
	* 同じ大きさで形を変更します。
	----------------------------------------------*/
	MatrixT& Deformation
	(
		SizeT
	);
};




/*-------------------------------------------------------------
* Implementation.
*
* Author:Sanae
-------------------------------------------------------------*/




/*------Function protected------*/
/*----------------------------------------------
* Convert the initial value to std::vector<_T>.
* 初期値をstd::vector<_T>へ変換します。
----------------------------------------------*/
template<typename T>
void Sanae::MatrixT<T>::m_Convert_to_Array
(
	std::initializer_list<std::vector<T>>& a_Input,
	std::vector<T>&                        a_Storage
)
{
	std::vector<std::vector<T>> _bufs = a_Input;

	for (Ulong i = 0; i < _bufs.size(); i++)
		a_Storage.insert(a_Storage.end(), _bufs[i].begin(), _bufs[i].end());
}

/*----------------------------------------------
* Display the matrix.
* 行列を表示します。
----------------------------------------------*/
template<typename T>
void Sanae::MatrixT<T>::m_View
(
	std::vector<T>* a_DataP,
	SizeT           a_Size
)
{
	for (Ulong i = 0; i < a_DataP->size(); i++) {
		if (i != 0 && i % a_Size.first == 0) std::cout << "\n";

		std::cout << (*a_DataP)[i] << " ";
	}
	std::cout << "\n";

	return;
}

/*----------------------------------------------
* Identity matrix.
* 単位行列にします。
----------------------------------------------*/
template<typename T>
void Sanae::MatrixT<T>::m_To_Identity_Matrix
(
	std::vector<T>*     a_Data,
	SizeT               a_Size
)
{
	a_Data->erase(a_Data->begin(), a_Data->end());  //全データの削除
	a_Data->resize(a_Size.first  * a_Size.second);    //サイズの変更

	for (Ulong i = 0; i < a_Size.first; i++)
		for (Ulong j = 0; j < a_Size.second; j++)
			(*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { i,j })] = i == j ? (T)1 : (T)0; //行と列の番号が同じ場合1それ以外の場合0

	return;
}

/*-----Determinant-----*/
/*----------------------------------------------
* Find the determinant using Salas' method. (2D)
* サラスの方式で行列式を求めます。(2次元)
----------------------------------------------*/
template<typename T>
T Sanae::MatrixT<T>::m_Det_2
(
	std::vector<T>* a_DataP,
	T               a_Coeff,
	SizeT           a_Size
)
{
	if (a_Size.first != a_Size.second)
		throw std::invalid_argument("Must be same column and line.");

	if (a_Size.first != 2)
		throw std::invalid_argument("Must pass 2 as argument.");

	return a_Coeff * (((*a_DataP)[this->m_Convert_to_ArrayNum(2, { 0,0 })] * (*a_DataP)[this->m_Convert_to_ArrayNum(2, { 1,1 })]) - ((*a_DataP)[this->m_Convert_to_ArrayNum(2, { 1,0 })] * (*a_DataP)[this->m_Convert_to_ArrayNum(2, { 0,1 })]));
}

/*----------------------------------------------
* Expand the cofactors to lower the matrix by one dimension.
The lowered value is stored in _Storage and the coefficient is stored in _Coeff.
* 余因子展開をして行列を一次元下げます。
下げた値は_Storageに格納され係数は_Coeffに格納されます。
----------------------------------------------*/
template<typename T>
void Sanae::MatrixT<T>::m_Confactor_expansion_1
(
	std::vector<T>&              a_Data,
	SizeT                        a_Size,
	std::vector<std::vector<T>>* a_Storage,
	std::vector<T>*              a_Coeff
)
{
	if (a_Size.first != a_Size.second)
		throw std::invalid_argument("Must be same size.");

	for (Ulong i = 0; i < a_Size.first; i++) {
		std::vector<T> _buffer;

		for (Ulong y = 0; y < a_Size.first; y++)
			for (Ulong x = 0; x < a_Size.second; x++)
				if (x != 0 && y != i)
					_buffer.push_back(a_Data[this->m_Convert_to_ArrayNum(a_Size.first, { x,y })]);

		a_Coeff->push_back(a_Data[this->m_Convert_to_ArrayNum(a_Size.first, { 0,i })] * (T)std::pow(-1, 1 + (i + 1)));
		a_Storage->push_back(_buffer);
	}
}

/*----------------------------------------------
* Perform cofactor expansion until it becomes two-dimensional.
* 二次元になるまで余因子展開を行います。
----------------------------------------------*/
template<typename T>
std::pair<std::vector<std::vector<T>>, std::vector<T>>
Sanae::MatrixT<T>::m_Confactor_expansion_to_2
(
	std::vector<T>&     a_Data,
	SizeT               a_Size,
	T              a_In_Coeff
)
{
	if (a_Size.first != a_Size.second)
		throw std::invalid_argument("Must be same size.");

	if (a_Size.first < 3)
		throw std::invalid_argument("Must pass above 3 as argument.");

	std::vector<std::vector<T>> _buf;   //行列格納用
	std::vector<T>              _coeff; //係数格納用

	this->m_Confactor_expansion_1(a_Data, a_Size, &_buf, &_coeff); //一次元下げる

	for (Ulong i = 0; i < _coeff.size(); i++)
		_coeff[i] *= a_In_Coeff;

	if (a_Size.first == 3)
		return { _buf,_coeff };

	std::vector<std::pair<std::vector<std::vector<T>>, std::vector<T>>> _bufs;
	for (Ulong i = 0; i < _buf.size(); i++)
		_bufs.push_back(this->m_Confactor_expansion_to_2(_buf[i], { a_Size.first - 1,a_Size.second - 1 }, _coeff[i])); //自分自身を呼び出す。

	//行列を係数の格納
	std::pair<std::vector<std::vector<T>>, std::vector<T>> _retdata;

	for (Ulong i = 0; i < _bufs.size(); i++) {
		for (Ulong j = 0; j < _bufs[i].first.size(); j++) {
			_retdata.first.push_back(_bufs[i].first[j]);
			_retdata.second.push_back(_bufs[i].second[j]);
		}
	}

	return _retdata;
}

/*----------------------------------------------
* Find the determinant.
* 行列式を求めます。
----------------------------------------------*/
template<typename T>
T Sanae::MatrixT<T>::m_Det
(
	std::vector<T>&     a_Data,
	SizeT               a_Size
)
{
	if (a_Size.first != a_Size.second)
		throw std::invalid_argument("Must be same size.");

	switch (a_Size.first) {
	case 1:
		return a_Data[0];
	case 2:
		return this->m_Det_2(&a_Data, 1, { 2,2 });
	default:
		break;
	}

	T _ret = 0;
	std::pair<std::vector<std::vector<T>>, std::vector<T>> _buf = this->m_Confactor_expansion_to_2(a_Data, a_Size);

	for (Ulong i = 0; i < _buf.first.size(); i++)
		_ret += this->m_Det_2(&_buf.first[i], _buf.second[i], { 2,2 });

	return _ret;
}

/*----------------------------------------------
* Find the inverse of the matrix.
* 逆行列を求めます。
----------------------------------------------*/
template<typename T>
void Sanae::MatrixT<T>::m_Inverse_matrix
(
	std::vector<T>* a_Data,
	std::vector<T>* a_Storage,
	SizeT           a_Size
)
{
	if (this->m_Det(this->m_Main, this->m_Size) == 0)
		throw std::runtime_error("Inverse does not exist.");

	//単位行列にする。
	this->m_To_Identity_Matrix(a_Storage, a_Size);

	//_pos:基準
	for (Ulong _pos = 0; _pos < a_Size.first; _pos++) {
		for (Ulong y = 0; y < a_Size.second; y++) {
			if (_pos != y) {
				T div = 0;

				//基準が0の場合行を入れ替える。
				if ((*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { _pos,_pos })] == 0) {
					for (Ulong findy = 0; findy < a_Size.second; findy++) {
						if ((*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { _pos,findy })] != 0) {
							this->m_Swap_Line(_pos, findy, a_Data, a_Size);
							this->m_Swap_Line(_pos, findy, a_Storage, a_Size);
						}
					}
				}

				//ax+b=0 x -= b/a  
				div = ((*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { _pos,y })] / (*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { _pos,_pos })]);

				//その他列への適用
				for (Ulong x = 0; x < a_Size.first; x++) {
					(*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { x,y })] -= div * ((*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { x,_pos })]);
					(*a_Storage)[this->m_Convert_to_ArrayNum(a_Size.first, { x,y })] -= div * ((*a_Storage)[this->m_Convert_to_ArrayNum(a_Size.first, { x,_pos })]);
				}
			}
		}
	}

	//基準が1になっていない場合その行へ適用し1にする。
	for (Ulong _pos = 0; _pos < a_Size.first; _pos++) {
		for (Ulong _x = 0; _x < a_Size.first; _x++) {
			T _is_zero_buf = (*a_Data)[this->m_Convert_to_ArrayNum(a_Size.first, { _pos,_pos })];
			if (_is_zero_buf != 0)
				(*a_Storage)[this->m_Convert_to_ArrayNum(a_Size.first, { _x,_pos })] /= _is_zero_buf;
		}
	}

	return;
}

/*----------------------------------------------
* Find the inner product.
* 内積を求めます。
----------------------------------------------*/
template<typename T>
void Sanae::MatrixT<T>::m_Inner_Product
(
	std::vector<T>* a_Data1,
	SizeT*          a_Size1,

	std::vector<T>* a_Data2,
	SizeT*          a_Size2,

	std::vector<T>* a_Store,

	bool            a_Clear
)
{
	if (a_Size1 == a_Size2)
		throw std::invalid_argument("Must be same size.");

	Ulong Size = a_Size1->first * a_Size1->second;

	if (a_Clear) {
		a_Store->erase(a_Store->begin(), a_Store->end());
		a_Store->resize(Size);
	}

	for (Ulong i = 0; i < Size; i++)
		(*a_Store)[i] = (*a_Data1)[i] * (*a_Data2)[i];
}


/*------Function public------*/


/*----------------------------------------------
* Assignment.
* 代入
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::operator=
(
	const MatrixT& a_Data
)
{
	this->m_Size = a_Data.m_Size;
	this->m_Main = a_Data.m_Main;

	return *this;
}
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::operator=
(
	const std::initializer_list<std::vector<T>> a_In
)
{
	this->m_Main.erase(this->m_Main.begin(), this->m_Main.end());
	std::vector<std::vector<T>> _buf = a_In;

	this->m_Size.second = a_In.size();
	this->m_Size.first  = _buf[0].size();

	for (Ulong line = 0; line < this->m_Size.second; line++)
		this->m_Main.insert(this->m_Main.end(), _buf[line].begin(), _buf[line].end());

	return *this;
}

/*----------------------------------------------
* Destructive processing
* 破壊的処理
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::operator+=
(
	const MatrixT& a_Data
)
{
	this->m_Add(a_Data, &this->m_Main, this->m_Size);

	return *this;
}
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::operator-=
(
	const MatrixT& a_Data
)
{
	this->m_Sub(a_Data, &this->m_Main, this->m_Size);

	return *this;
}
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::operator*=
(
	const MatrixT& a_Data
)
{
	std::vector<T> _ret;
	SizeT          _ret_size;

	std::vector<T>  _buf      = this->m_Main;
	SizeT           _buf_size = this->m_Size;

	this->m_Mul(_buf, _buf_size, (std::vector<T>&)a_Data.m_Main, a_Data.m_Size, &this->m_Main, &this->m_Size);

	return *this;
}
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::operator*=
(
	T a_Num
)
{
	for (Ulong i = 0; i < this->m_Main.size(); i++)
		this->m_Main[i] *= a_Num;

	return *this;
}
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::operator^=
(
	const MatrixT<T>& a_Data
)
{
	this->m_Inner_Product(&this->m_Main, &this->m_Size, &a_Data.m_Main, &a_Data.m_Size, &this->m_Main, false);
	return *this;
}

/*----------------------------------------------
* Non-destructive processing.
* 非破壊的処理
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T> Sanae::MatrixT<T>::operator+
(
	const MatrixT& a_Data
)
{
	std::vector<T> _ret = this->m_Main;
	this->m_Add(a_Data, &_ret, this->m_Size);

	return std::pair<SizeT, std::vector<T>>{this->m_Size, _ret};
}
template<typename T>
Sanae::MatrixT<T> Sanae::MatrixT<T>::operator-
(
	const MatrixT& a_Data
)
{
	std::vector<T> _ret = this->m_Main;
	this->m_Sub(a_Data, &_ret, this->m_Size);

	return std::pair<SizeT, std::vector<T>>{this->m_Size, _ret};
}
template<typename T>
Sanae::MatrixT<T> Sanae::MatrixT<T>::operator*
(
	const MatrixT& a_Data
)
{
	std::vector<T> _ret;
	SizeT          _ret_size;

	std::vector<T> _buf      = this->m_Main;
	SizeT          _buf_size = this->m_Size;

	this->m_Mul(_buf, _buf_size, (std::vector<T>&)a_Data.m_Main, a_Data.m_Size, &_ret, &_ret_size);

	return std::pair<SizeT, std::vector<T>>{_ret_size, _ret};
}
template<typename T>
Sanae::MatrixT<T> Sanae::MatrixT<T>::operator*
(
	T a_Num
)
{
	std::vector<T> _data = this->m_Main;

	for (Ulong i = 0; i < _data.size(); i++)
		_data[i] *= a_Num;

	return std::pair<SizeT, std::vector<T>>{this->m_Size, _data};
}
template<typename T>
Sanae::MatrixT<T> Sanae::MatrixT<T>::operator^
(
	const MatrixT<T>& a_Data
) 
{
	MatrixT Ret;
	this->m_Inner_Product(&this->m_Main, &this->m_Size, &a_Data.m_Main, &a_Data.m_Size, &Ret.m_Main, true);
	Ret.m_Size = this->m_Size;

	return Ret;
}

/*----------------------------------------------
* (Destructive processing) Resize.
* (破壊的処理)サイズ変更
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::SetSize
(
	SizeT a_Data,
	bool  a_Clear
)
{
	if (a_Clear)
	{
		this->m_Main.erase(this->m_Main.begin(), this->m_Main.end());
	}
	else
	{
		//サイズ増大
		if (a_Data.first > this->m_Size.first) {
			for (Ulong i = 1; i < a_Data.second; i++)
				this->m_Main.insert(this->m_Main.begin() + (this->m_Size.first * i) + (i - 1), 0);
		}

		//サイズ減少
		if (a_Data.first < this->m_Size.first) {
			for (Ulong i = 1; i < a_Data.second; i++)
				this->m_Main.erase(this->m_Main.begin() + a_Data.first, this->m_Main.begin() + a_Data.first + (this->m_Size.first - a_Data.first));
		}
	}

	this->m_Main.resize(a_Data.first * a_Data.second);
	this->m_Size = a_Data;

	return *this;
}

/*----------------------------------------------
* (Destructive processing) Swap lines.
* (破壊的処理)行を入れ替える。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::Swap_Line
(
	Ulong a_Line1,
	Ulong a_Line2
)
{
	this->m_Swap_Line(a_Line1, a_Line2, &this->m_Main, this->m_Size);

	return *this;
}

/*----------------------------------------------
* (Destructive processing) Swap Columns.
* (破壊的処理)列を入れ替える。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::Swap_Column
(
	Ulong a_Column1,
	Ulong a_Column2
)
{
	this->m_Swap_Column(a_Column1, a_Column2, &this->m_Main, this->m_Size);

	return *this;
}

/*----------------------------------------------
* (Destructive processing)
Make it an identity matrix.
* (破壊的処理)単位行列にする。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::Ident()
{
	this->m_To_Identity_Matrix(&this->m_Main, this->m_Size);

	return *this;
}

/*----------------------------------------------
* (Non-destructive processing) Find the determinant.
* (非破壊的処理)行列式を求めます。
----------------------------------------------*/
template<typename T>
T Sanae::MatrixT<T>::Det()
{
	return this->m_Det(this->m_Main, this->m_Size);
}

/*----------------------------------------------
* (Non-destructive processing) Transposes a matrix.
* (非破壊的処理)行列の転置を行います。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T> Sanae::MatrixT<T>::Transpose()
{
	std::vector<T> _Data;
	_Data.resize(this->m_Size.first * this->m_Size.second);

	for (Ulong x = 0; x < this->m_Size.first; x++)
		for (Ulong y = 0; y < this->m_Size.second; y++)
			_Data[this->m_Convert_to_ArrayNum(this->m_Size.second, { y,x })] = this->m_Main[this->m_Convert_to_ArrayNum(this->m_Size.first, { x,y })];

	return std::pair<SizeT, std::vector<T>>{ SizeT{this->m_Size.second, this->m_Size.first}, _Data };
}

/*----------------------------------------------
* (Non-destructive processing) Returns the inverse of the matrix.
* (非破壊的処理)逆行列を返します。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T> Sanae::MatrixT<T>::Inverse()
{
	std::vector<T> _to;
	std::vector<T> _buf = this->m_Main;

	this->m_Inverse_matrix(&_buf, &_to, this->m_Size);

	return std::pair<SizeT, std::vector<T>>{ this->m_Size, _to };
}

/*----------------------------------------------
* Display the matrix.
* 行列を表示します。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::View()
{
	this->m_View(&this->m_Main, this->m_Size);

	return *this;
}

/*----------------------------------------------
* Returns the size.
* 大きさを返します。
----------------------------------------------*/
template<typename T>
Ulong Sanae::MatrixT<T>::GetSize() {
	return this->m_Main.size();
}

/*----------------------------------------------
* Returns the size.  first:column second: row
* 大きさを返します。 first:列     second: 行
----------------------------------------------*/
template<typename T>
SizeT Sanae::MatrixT<T>::GetSizeWH() {
	return this->m_Size;
}

/*----------------------------------------------
* Transfer an array.
* 配列を譲渡します。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::Move
(
	std::vector<T>* a_Data,
	SizeT           a_Size
)
{
	this->m_Main = std::move(*a_Data);
	this->m_Size = a_Size;

	return *this;
}

/*----------------------------------------------
* Returns a pointer to _Main.
(Can be used in functions such as std::max_element.)
* _Mainのポインタを返します。
(std::max_elementなどの関数で使用することができます。)
----------------------------------------------*/
template<typename T>
const std::vector<T>* Sanae::MatrixT<T>::GetVectorP()
{
	return &this->m_Main;
}

/*----------------------------------------------
* Returns a pointer to _Main.
(Can be used in functions such as std::max_element.)
* 同じ大きさで形を変更します。
----------------------------------------------*/
template<typename T>
Sanae::MatrixT<T>& Sanae::MatrixT<T>::Deformation
(
	SizeT a_To
)
{
	if (this->GetSize() != (a_To.first * a_To.second))
		throw std::invalid_argument("Array count must not change.");
	else
		this->m_Size = a_To;

	return *this;
}




#endif
#endif