/*=============================================================
* NAME      : Matrix.hpp
* AUTHOR    : SanaeProject
* VER       : 3.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
=============================================================*/




/*-----Include Guard-----*/
#ifndef _SANAE_MATRIX_HPP_
#define _SANAE_MATRIX_HPP_




/*-----Include-----*/
#include "MatrixBase.hpp"

/*-----Check Version C++14 or more and Is existing-----*/
#if defined(_CPP_14_OR_MORE_) && defined(_SANAE_MATRIXBASE_HPP_ALL_INCLUDE_)




/*-----Define Class-----*/
namespace Sanae {
	 template<typename _T> class _Matrix_;

	 using Matrix      = _Matrix_<double>;
	 using Matrix_Lite = _Matrix_<float >;
}




/*-------------------------------------------------------------
* Define Class
*
* Author:Sanae
-------------------------------------------------------------*/


template<typename _T> class Sanae::_Matrix_ :public Sanae::_MatrixBase<_T>
{


	/*-----Define Variable(protected)-----*/
protected:


	/*----------------------------------------------
	* Convert the initial value to std::vector<_T>.
	* 初期値をstd::vector<_T>へ変換します。
	----------------------------------------------*/
	void _Convert_to_Array
	(
		std::initializer_list<std::vector<_T>>&,
		std::vector<_T>&
	);

	/*----------------------------------------------
	* Display the matrix.
	* 行列を表示します。
	----------------------------------------------*/
	void _View
	(
		std::vector<_T>*,
		SizeT
	);

	/*----------------------------------------------
	* Identity matrix.
	* 単位行列にします。
	----------------------------------------------*/
	void _To_Identity_Matrix
	(
		std::vector<_T>*,
		SizeT
	);


	/*-----Determinant-----*/
	/*----------------------------------------------
	* Find the determinant using Salas' method. (2D)
	* サラスの方式で行列式を求めます。(2次元)
	----------------------------------------------*/
	_T _Det_2
	(
		std::vector<_T>*,
		_T,
		SizeT
	);

	/*----------------------------------------------
	* Expand the cofactors to lower the matrix by one dimension.
	The lowered value is stored in _Storage and the coefficient is stored in _Coeff.
	* 余因子展開をして行列を一次元下げます。
	下げた値は_Storageに格納され係数は_Coeffに格納されます。
	----------------------------------------------*/
	void _Confactor_expansion_1
	(
		std::vector<_T>&,
		SizeT,
		std::vector<std::vector<_T>>*,
		std::vector<_T>*
	);

	/*----------------------------------------------
	* Perform cofactor expansion until it becomes two-dimensional.
	* 二次元になるまで余因子展開を行います。
	----------------------------------------------*/
	std::pair<std::vector<std::vector<_T>>, std::vector<_T>>
		_Confactor_expansion_to_2
		(
			std::vector<_T>&,
			SizeT,
			double = 1
		);

	/*----------------------------------------------
	* Find the determinant.
	* 行列式を求めます。
	----------------------------------------------*/
	double _Det
	(
		std::vector<_T>&,
		SizeT
	);

	/*----------------------------------------------
	* Find the inverse of the matrix.
	* 逆行列を求めます。
	----------------------------------------------*/
	void _Inverse_matrix
	(
		std::vector<_T>*,
		std::vector<_T>*,
		SizeT
	);

	/*----------------------------------------------
	* Find the inner product.
	* 内積を求めます。
	----------------------------------------------*/
	void _Inner_Product
	(
		std::vector<_T>*,
		SizeT*,

		std::vector<_T>*,
		SizeT*,

		std::vector<_T>*,

		bool
	);


	/*-----Define Functions(public)-----*/
public:


	/*-----Constructor-----*/
	using _MatrixBase<_T>::_MatrixBase;


	/*----------------------------------------------
	* Assignment.
	* 代入
	----------------------------------------------*/
	_Matrix_& operator=
	(
		const _Matrix_&
	);
	_Matrix_& operator=
	(
		std::initializer_list<std::vector<_T>>
	);

	/*----------------------------------------------
	* Destructive processing
	* 破壊的処理
	----------------------------------------------*/
	_Matrix_& operator+=
	(
		const _Matrix_&
	);
	_Matrix_& operator-=
	(
		const _Matrix_&
	);
	_Matrix_& operator*=
	(
		const _Matrix_&
	);
	_Matrix_& operator*=
	(
		_T
	);

	/*----------------------------------------------
	*  Non-destructive processing.
	* 非破壊的処理
	----------------------------------------------*/
	_Matrix_  operator+
	(
		const _Matrix_&
	);
	_Matrix_  operator-
	(
		const _Matrix_&
	);
	_Matrix_  operator*
	(
		const _Matrix_&
	);
	_Matrix_  operator*
	(
		_T
	);

	/*----------------------------------------------
	* (Destructive processing) Resize.
	* (破壊的処理)サイズ変更
	----------------------------------------------*/
	_Matrix_& SetSize
	(
		SizeT,
		bool = true
	);

	/*----------------------------------------------
	* (Destructive processing) Swap lines.
	* (破壊的処理)行と行を入れ替える。
	----------------------------------------------*/
	_Matrix_& Swap_Line
	(
		Ulong,
		Ulong
	);

	/*----------------------------------------------
	* (Destructive processing) Swap Columns.
	* (破壊的処理)列を入れ替える。
	----------------------------------------------*/
	_Matrix_& Swap_Column
	(
		Ulong,
		Ulong
	);

	/*----------------------------------------------
	* (Destructive processing) 
	Make it an identity matrix.
	* (破壊的処理)単位行列にする。
	----------------------------------------------*/
	_Matrix_& Ident();

	/*----------------------------------------------
	* (Non-destructive processing) Find the determinant.
	* (非破壊的処理)行列式を求めます。
	----------------------------------------------*/
	_T Det();

	/*----------------------------------------------
	* (Non-destructive processing) Transposes a matrix.
	* (非破壊的処理)行列の転置を行います。
	----------------------------------------------*/
	_Matrix_ Transpose();

	/*----------------------------------------------
	* (Non-destructive processing) Returns the inverse of the matrix.
	* (非破壊的処理)逆行列を返します。
	----------------------------------------------*/
	_Matrix_ Inverse();

	/*----------------------------------------------
	* Display the matrix.
	* 行列を表示します。
	----------------------------------------------*/
	_Matrix_& View();

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
	* (Destructive processing) Find the dot product.
	* (破壊的処理)内積を求めます。
	----------------------------------------------*/
	_Matrix_ Inner_Product
	(
		_Matrix_&
	);

	/*----------------------------------------------
	* (Destructive processing) Find the dot product.
	* (破壊的処理)内積を求めます。
	----------------------------------------------*/
	_Matrix_& Inner_Product_Destructive
	(
		_Matrix_&
	);

	/*----------------------------------------------
	* Transfer an array.
	* 配列を譲渡します。
	----------------------------------------------*/
	_Matrix_& Move
	(
		std::vector<_T>*,
		SizeT
	);

	/*----------------------------------------------
	* Returns a pointer to _Main.
	(Can be used in functions such as std::max_element.)
	* _Mainのポインタを返します。
	(std::max_elementなどの関数で使用することができます。)
	----------------------------------------------*/
	const std::vector<_T>* GetVectorP();

	/*----------------------------------------------
	* Returns a pointer to _Main.
	(Can be used in functions such as std::max_element.)
	* 同じ大きさで形を変更します。
	----------------------------------------------*/
	_Matrix_& Deformation
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
template<typename _T>
void Sanae::_Matrix_<_T>::_Convert_to_Array
(
	std::initializer_list<std::vector<_T>>& _Input,
	std::vector<_T>& _Storage
)
{
	std::vector<std::vector<_T>> _bufs = _Input;

	for (Ulong i = 0; i < _bufs.size(); i++)
		_Storage.insert(_Storage.end(), _bufs[i].begin(), _bufs[i].end());
}

/*----------------------------------------------
* Display the matrix.
* 行列を表示します。
----------------------------------------------*/
template<typename _T>
void Sanae::_Matrix_<_T>::_View
(
	std::vector<_T>* _DataP,
	SizeT                _Size
)
{
	for (Ulong i = 0; i < _DataP->size(); i++) {
		if (i != 0 && i % _Size.first == 0) std::cout << "\n";

		std::cout << (*_DataP)[i] << " ";
	}
	std::cout << "\n";

	return;
}

/*----------------------------------------------
* Identity matrix.
* 単位行列にします。
----------------------------------------------*/
template<typename _T>
void Sanae::_Matrix_<_T>::_To_Identity_Matrix
(
	std::vector<_T>*     _Data,
	SizeT                _Size
)
{
	_Data->erase(_Data->begin(), _Data->end());  //全データの削除
	_Data->resize(_Size.first * _Size.second);    //サイズの変更

	for (Ulong i = 0; i < _Size.first; i++)
		for (Ulong j = 0; j < _Size.second; j++)
			(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,j })] = i == j ? 1 : 0; //行と列の番号が同じ場合1それ以外の場合0

	return;
}

/*-----Determinant-----*/
/*----------------------------------------------
* Find the determinant using Salas' method. (2D)
* サラスの方式で行列式を求めます。(2次元)
----------------------------------------------*/
template<typename _T>
_T Sanae::_Matrix_<_T>::_Det_2
(
	std::vector<_T>* _DataP,
	_T               _Coeff,
	SizeT            _Size
)
{
	if (_Size.first != _Size.second)
		throw std::invalid_argument("Must be same column and line.");

	if (_Size.first != 2)
		throw std::invalid_argument("Must pass 2 as argument.");

	return _Coeff * (((*_DataP)[this->_Convert_to_ArrayNum(2, { 0,0 })] * (*_DataP)[this->_Convert_to_ArrayNum(2, { 1,1 })]) - ((*_DataP)[this->_Convert_to_ArrayNum(2, { 1,0 })] * (*_DataP)[this->_Convert_to_ArrayNum(2, { 0,1 })]));
}

/*----------------------------------------------
* Expand the cofactors to lower the matrix by one dimension.
The lowered value is stored in _Storage and the coefficient is stored in _Coeff.
* 余因子展開をして行列を一次元下げます。
下げた値は_Storageに格納され係数は_Coeffに格納されます。
----------------------------------------------*/
template<typename _T>
void Sanae::_Matrix_<_T>::_Confactor_expansion_1
(
	std::vector<_T>& _Data,
	SizeT                             _Size,
	std::vector<std::vector<_T>>* _Storage,
	std::vector<_T>* _Coeff
)
{
	if (_Size.first != _Size.second)
		throw std::invalid_argument("Must be same size.");

	for (Ulong i = 0; i < _Size.first; i++) {
		std::vector<double> _buffer;

		for (Ulong y = 0; y < _Size.first; y++)
			for (Ulong x = 0; x < _Size.second; x++)
				if (x != 0 && y != i)
					_buffer.push_back(_Data[this->_Convert_to_ArrayNum(_Size.first, { x,y })]);

		_Coeff->push_back(_Data[this->_Convert_to_ArrayNum(_Size.first, { 0,i })] * std::pow(-1, 1 + (i + 1)));
		_Storage->push_back(_buffer);
	}
}

/*----------------------------------------------
* Perform cofactor expansion until it becomes two-dimensional.
* 二次元になるまで余因子展開を行います。
----------------------------------------------*/
template<typename _T>
std::pair<std::vector<std::vector<_T>>, std::vector<_T>>
Sanae::_Matrix_<_T>::_Confactor_expansion_to_2
(
	std::vector<_T>&     _Data,
	SizeT                _Size,
	double               _In_Coeff
)
{
	if (_Size.first != _Size.second)
		throw std::invalid_argument("Must be same size.");

	if (_Size.first < 3)
		throw std::invalid_argument("Must pass above 3 as argument.");

	std::vector<std::vector<double>> _buf;   //行列格納用
	std::vector<double>              _coeff; //係数格納用

	this->_Confactor_expansion_1(_Data, _Size, &_buf, &_coeff); //一次元下げる

	for (Ulong i = 0; i < _coeff.size(); i++)
		_coeff[i] *= _In_Coeff;

	if (_Size.first == 3)
		return { _buf,_coeff };

	std::vector<std::pair<std::vector<std::vector<double>>, std::vector<double>>> _bufs;
	for (Ulong i = 0; i < _buf.size(); i++)
		_bufs.push_back(this->_Confactor_expansion_to_2(_buf[i], { _Size.first - 1,_Size.second - 1 }, _coeff[i])); //自分自身を呼び出す。

	//行列を係数の格納
	std::pair<std::vector<std::vector<double>>, std::vector<double>> _retdata;

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
template<typename _T>
double Sanae::_Matrix_<_T>::_Det
(
	std::vector<_T>&     _Data,
	SizeT                _Size
)
{
	if (_Size.first != _Size.second)
		throw std::invalid_argument("Must be same size.");

	switch (_Size.first) {
	case 1:
		return _Data[0];
	case 2:
		return this->_Det_2(&_Data, 1, { 2,2 });
	default:
		break;
	}

	double _ret = 0;
	std::pair<std::vector<std::vector<double>>, std::vector<double>> _buf = this->_Confactor_expansion_to_2(_Data, _Size);

	for (Ulong i = 0; i < _buf.first.size(); i++)
		_ret += this->_Det_2(&_buf.first[i], _buf.second[i], { 2,2 });

	return _ret;
}

/*----------------------------------------------
* Find the inverse of the matrix.
* 逆行列を求めます。
----------------------------------------------*/
template<typename _T>
void Sanae::_Matrix_<_T>::_Inverse_matrix
(
	std::vector<_T>* _Data,
	std::vector<_T>* _Storage,
	SizeT            _Size
)
{
	if (this->_Det(this->_Main, this->_Size) == 0)
		throw std::runtime_error("Inverse does not exist.");

	//単位行列にする。
	this->_To_Identity_Matrix(_Storage, _Size);

	//_pos:基準
	for (Ulong _pos = 0; _pos < _Size.first; _pos++) {
		for (Ulong y = 0; y < _Size.second; y++) {
			if (_pos != y) {
				_T div = 0;

				//基準が0の場合行を入れ替える。
				if ((*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _pos,_pos })] == 0) {
					for (Ulong findy = 0; findy < _Size.second; findy++) {
						if ((*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _pos,findy })] != 0) {
							this->_Swap_Line(_pos, findy, _Data, _Size);
							this->_Swap_Line(_pos, findy, _Storage, _Size);
						}
					}
				}

				//ax+b=0 x -= b/a  
				div = ((*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _pos,y })] / (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _pos,_pos })]);

				//その他列への適用
				for (Ulong x = 0; x < _Size.first; x++) {
					(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { x,y })] -= div * ((*_Data)[this->_Convert_to_ArrayNum(_Size.first, { x,_pos })]);
					(*_Storage)[this->_Convert_to_ArrayNum(_Size.first, { x,y })] -= div * ((*_Storage)[this->_Convert_to_ArrayNum(_Size.first, { x,_pos })]);
				}
			}
		}
	}

	//基準が1になっていない場合その行へ適用し1にする。
	for (Ulong _pos = 0; _pos < _Size.first; _pos++) {
		for (Ulong _x = 0; _x < _Size.first; _x++) {
			_T _is_zero_buf = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _pos,_pos })];
			if (_is_zero_buf != 0)
				(*_Storage)[this->_Convert_to_ArrayNum(_Size.first, { _x,_pos })] /= _is_zero_buf;
		}
	}

	return;
}

/*----------------------------------------------
* Find the inner product.
* 内積を求めます。
----------------------------------------------*/
template<typename _T>
void Sanae::_Matrix_<_T>::_Inner_Product
(
	std::vector<_T>* _Data1,
	SizeT* _Size1,

	std::vector<_T>* _Data2,
	SizeT* _Size2,

	std::vector<_T>* _Store,

	bool                 _Clear
)
{
	if (_Size1 == _Size2)
		throw std::invalid_argument("Must be same size.");

	Ulong Size = _Size1->first * _Size1->second;

	if (_Clear) {
		_Store->erase(_Store->begin(), _Store->end());
		_Store->resize(Size);
	}

	for (Ulong i = 0; i < Size; i++)
		(*_Store)[i] = (*_Data1)[i] * (*_Data2)[i];
}


/*------Function public------*/


/*----------------------------------------------
* Assignment.
* 代入
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::operator=
(
	const _Matrix_& _Data
)
{
	this->_Size = _Data._Size;
	this->_Main = _Data._Main;

	return *this;
}
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::operator=
(
	std::initializer_list<std::vector<_T>> _In
)
{
	this->_Main.erase(this->_Main.begin(), this->_Main.end());
	std::vector<std::vector<_T>> _buf = _In;

	this->_Size.second = _In.size();
	this->_Size.first = _buf[0].size();

	for (Ulong line = 0; line < this->_Size.second; line++)
		this->_Main.insert(this->_Main.end(), _buf[line].begin(), _buf[line].end());

	return *this;
}

/*----------------------------------------------
* Destructive processing
* 破壊的処理
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::operator+=
(
	const _Matrix_& _Data
)
{
	this->_Add(_Data, &this->_Main, this->_Size);

	return *this;
}
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::operator-=
(
	const _Matrix_& _Data
)
{
	this->_Sub(_Data, &this->_Main, this->_Size);

	return *this;
}
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::operator*=
(
	const _Matrix_& _Data
)
{
	std::vector<_T> _ret;
	SizeT           _ret_size;

	std::vector<_T> _buf      = this->_Main;
	SizeT           _buf_size = this->_Size;

	this->_Mul(_buf, _buf_size, (std::vector<_T>&)_Data._Main, _Data._Size, &this->_Main, &this->_Size);

	return *this;
}
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::operator*=
(
	_T _Num
)
{
	for (Ulong i = 0; i < this->_Main.size(); i++)
		this->_Main[i] *= _Num;

	return *this;
}

/*----------------------------------------------
* Non-destructive processing.
* 非破壊的処理
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T> Sanae::_Matrix_<_T>::operator+
(
	const _Matrix_& _Data
)
{
	std::vector<double> _ret = this->_Main;
	this->_Add(_Data, &_ret, this->_Size);

	return std::pair<SizeT, std::vector<double>>{this->_Size, _ret};
}
template<typename _T>
Sanae::_Matrix_<_T> Sanae::_Matrix_<_T>::operator-
(
	const _Matrix_& _Data
)
{
	std::vector<double> _ret = this->_Main;
	this->_Sub(_Data, &_ret, this->_Size);

	return std::pair<SizeT, std::vector<double>>{this->_Size, _ret};
}
template<typename _T>
Sanae::_Matrix_<_T> Sanae::_Matrix_<_T>::operator*
(
	const _Matrix_& _Data
)
{
	std::vector<double> _ret;
	SizeT               _ret_size;

	std::vector<double> _buf = this->_Main;
	SizeT               _buf_size = this->_Size;

	this->_Mul(_buf, _buf_size, (std::vector<double>&)_Data._Main, _Data._Size, &_ret, &_ret_size);

	return std::pair<SizeT, std::vector<double>>{_ret_size, _ret};
}
template<typename _T>
Sanae::_Matrix_<_T> Sanae::_Matrix_<_T>::operator*
(
	_T _Num
)
{
	std::vector<double> _data = this->_Main;

	for (Ulong i = 0; i < _data.size(); i++)
		_data[i] *= _Num;

	return std::pair<SizeT, std::vector<double>>{this->_Size, _data};
}

/*----------------------------------------------
* (Destructive processing) Resize.
* (破壊的処理)サイズ変更
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::SetSize
(
	SizeT _Data,
	bool  _Clear
)
{
	if (_Clear)
	{
		this->_Main.erase(this->_Main.begin(), this->_Main.end());
	}
	else
	{
		//サイズ増大
		if (_Data.first > this->_Size.first) {
			for (Ulong i = 1; i < _Data.second; i++)
				this->_Main.insert(this->_Main.begin() + (this->_Size.first * i) + (i - 1), 0);
		}

		//サイズ減少
		if (_Data.first < this->_Size.first) {
			for (Ulong i = 1; i < _Data.second; i++)
				this->_Main.erase(this->_Main.begin() + _Data.first, this->_Main.begin() + _Data.first + (this->_Size.first - _Data.first));
		}
	}

	this->_Main.resize(_Data.first * _Data.second);
	this->_Size = _Data;

	return *this;
}

/*----------------------------------------------
* (Destructive processing) Swap lines.
* (破壊的処理)行を入れ替える。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::Swap_Line
(
	Ulong _Line1,
	Ulong _Line2
)
{
	this->_Swap_Line(_Line1, _Line2, &this->_Main, this->_Size);

	return *this;
}

/*----------------------------------------------
* (Destructive processing) Swap Columns.
* (破壊的処理)列を入れ替える。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::Swap_Column
(
	Ulong _Column1,
	Ulong _Column2
)
{
	this->_Swap_Column(_Column1, _Column2, &this->_Main, this->_Size);

	return *this;
}

/*----------------------------------------------
* (Destructive processing)
Make it an identity matrix.
* (破壊的処理)単位行列にする。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::Ident()
{
	this->_To_Identity_Matrix(&this->_Main, this->_Size);

	return *this;
}

/*----------------------------------------------
* (Non-destructive processing) Find the determinant.
* (非破壊的処理)行列式を求めます。
----------------------------------------------*/
template<typename _T>
_T Sanae::_Matrix_<_T>::Det()
{
	return this->_Det(this->_Main, this->_Size);
}

/*----------------------------------------------
* (Non-destructive processing) Transposes a matrix.
* (非破壊的処理)行列の転置を行います。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T> Sanae::_Matrix_<_T>::Transpose()
{
	std::vector<double> _Data;
	_Data.resize(this->_Size.first * this->_Size.second);

	for (Ulong x = 0; x < this->_Size.first; x++)
		for (Ulong y = 0; y < this->_Size.second; y++)
			_Data[this->_Convert_to_ArrayNum(this->_Size.second, { y,x })] = this->_Main[this->_Convert_to_ArrayNum(this->_Size.first, { x,y })];

	return std::pair<SizeT, std::vector<double>>{ SizeT{this->_Size.second, this->_Size.first}, _Data };
}

/*----------------------------------------------
* (Non-destructive processing) Returns the inverse of the matrix.
* (非破壊的処理)逆行列を返します。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T> Sanae::_Matrix_<_T>::Inverse()
{
	std::vector<double> _to;
	std::vector<double> _buf = this->_Main;

	this->_Inverse_matrix(&_buf, &_to, this->_Size);

	return std::pair<SizeT, std::vector<double>>{ this->_Size, _to };
}

/*----------------------------------------------
* Display the matrix.
* 行列を表示します。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::View()
{
	this->_View(&this->_Main, this->_Size);

	return *this;
}

/*----------------------------------------------
* Returns the size.
* 大きさを返します。
----------------------------------------------*/
template<typename _T>
Ulong Sanae::_Matrix_<_T>::GetSize() {
	return this->_Main.size();
}

/*----------------------------------------------
* Returns the size.  first:column second: row
* 大きさを返します。 first:列     second: 行
----------------------------------------------*/
template<typename _T>
SizeT Sanae::_Matrix_<_T>::GetSizeWH() {
	return this->_Size;
}

/*----------------------------------------------
* (Destructive processing) Find the dot product.
* (破壊的処理)内積を求めます。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T> Sanae::_Matrix_<_T>::Inner_Product
(
	_Matrix_& _Data
)
{
	_Matrix_ Ret;
	this->_Inner_Product(&this->_Main, &this->_Size, &_Data._Main, &_Data._Size, &Ret._Main, true);
	Ret._Size = this->_Size;

	return Ret;
}

/*----------------------------------------------
* (Destructive processing) Find the dot product.
* (破壊的処理)内積を求めます。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::Inner_Product_Destructive
(
	_Matrix_& _Data
)
{
	this->_Inner_Product(&this->_Main, &this->_Size, &_Data._Main, &_Data._Size, &this->_Main, false);
	return *this;
}

/*----------------------------------------------
* Transfer an array.
* 配列を譲渡します。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::Move
(
	std::vector<_T>* _Data,
	SizeT                _Size
)
{
	this->_Main = std::move(*_Data);
	this->_Size = _Size;

	return *this;
}

/*----------------------------------------------
* Returns a pointer to _Main.
(Can be used in functions such as std::max_element.)
* _Mainのポインタを返します。
(std::max_elementなどの関数で使用することができます。)
----------------------------------------------*/
template<typename _T>
const std::vector<_T>* Sanae::_Matrix_<_T>::GetVectorP()
{
	return &this->_Main;
}

/*----------------------------------------------
* Returns a pointer to _Main.
(Can be used in functions such as std::max_element.)
* 同じ大きさで形を変更します。
----------------------------------------------*/
template<typename _T>
Sanae::_Matrix_<_T>& Sanae::_Matrix_<_T>::Deformation
(
	SizeT _To
)
{
	if (this->GetSize() != (_To.first * _To.second))
		throw std::invalid_argument("Array count must not change.");
	else
		this->_Size = _To;

	return *this;
}




#endif
#endif