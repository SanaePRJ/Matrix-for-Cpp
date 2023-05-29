/*=============================================================
* NAME      : Matrix.h 
* AUTHOR    : SanaeProject
* VER       : 0.0.1(BETA)
* COPYRIGHGT: Copyright 2023 SanaeProject.
=============================================================*/


#ifndef _SANAE_MATRIX_H_
#define _SANAE_MATRIX_H_


/*INCLUDE*/
#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>


/*DEFINE CLASS*/
using  Ulong  = unsigned _int64;
using  Slong  = signed   _int64;
using  SizeT  = std::pair<Ulong, Ulong>;


//乗算時10^-6以下の誤差は無視する。
constexpr auto _ERROR_SANAE = 1e-6;


template<typename _T = double> class Matrix {
//Define variables(private)
private:
	std::vector<_T>   _Main;
	//First:X Second:Y
	SizeT             _Size = std::make_pair(0,0);

//Define functions(private)
private:
	//Converter
	Ulong _Convert_to_ArrayNum(Ulong _Width,SizeT _Pos) 
	{
		return (_Width*_Pos.second)+_Pos.first;
	}
	SizeT _Convert_to_Position(Ulong _Width,Ulong _Arraynum)
	{
		return std::make_pair(_Arraynum%_Width,(_Arraynum - (_Arraynum % _Width))/_Width);
	}
	void _Convert_to_Array(std::initializer_list<std::vector<_T>>& _buf,std::vector<_T>& _con)
	{
		std::vector<std::vector<_T>> _bufs = _buf;

		for (Ulong i = 0; i < _bufs.size(); i++)
			_con.insert(_con.end(),_bufs[i].begin(),_bufs[i].end());
	}

	//view
	void _View(std::vector<_T>* _DataP, SizeT _size)
	{
		for (Ulong i = 0; i < _DataP->size(); i++) {
			
			if (i != 0 && i % _size.first == 0)
				std::cout << "\n";
			
			std::cout << (*_DataP)[i] << " ";
		}
		std::cout << "\n";
		
		return;
	}

	//指定された行を入れ替えます。
	void _Swap_Line(Ulong Line1,Ulong Line2,std::vector<_T>* _data,SizeT _size)
	{
		for (Ulong i = 0; i < _size.first ;i++) {
			_T _temp = (*_data)[this->_Convert_to_ArrayNum(_size.first, { i,Line1 })];

			(*_data)[this->_Convert_to_ArrayNum(_size.first, { i,Line1 })] = (*_data)[this->_Convert_to_ArrayNum(_size.first, {i,Line2})];
			(*_data)[this->_Convert_to_ArrayNum(_size.first, { i,Line2 })] = _temp;
		}

		return;
	}
	void _Swap_Coulmn(Ulong Column1, Ulong Column2, std::vector<_T>* _data, SizeT _size)
	{
		for (Ulong i = 0; i < _size.second; i++) {
			_T _temp = this->_Access(SizeT{Column1, i}, _data,_size);

			this->_Access(SizeT{Column1, i}, _data,_size) = this->_Access(SizeT{Column2, i}, _data,_size);
			this->_Access(SizeT{Column2, i}, _data,_size) = _temp;
		}

		return;
	}

	//単位行列にします。
	void _To_Identity_Matrix(std::vector<_T>* _data, SizeT _size) {
		_data->erase(_data->begin(),_data->end());
		_data->resize(_size.first*_size.second);

		for (Ulong i = 0; i < _size.first; i++)
			for (Ulong j = 0; j < _size.second; j++)
				(*_data)[this->_Convert_to_ArrayNum(_size.first, {i,j})] = i == j ? 1 : 0;

		return;
	}

	//足し算を行います。
	void _Add(const Matrix& _Data,std::vector<_T>* _DataP,SizeT _size) 
	{
		if (_DataP->size() != _Data._Main.size() || _size.first != _Data._Size.first || _size.second != _Data._Size.second)
			throw std::invalid_argument("Must be same size.");

		for (Ulong i = 0; i < _DataP->size();i++)
			(*_DataP)[i] += _Data._Main[i];

		return;
	}
	void _Add(std::vector<_T>& _buf, std::vector<_T>* _DataP, SizeT _size)
	{
		if (_DataP->size() != _buf.size() || _size.first!=_buf[0].size()||_size.second!=_buf.size)
			throw std::invalid_argument("Must be same size.");

		for (Ulong i = 0; i < _DataP->size(); i++)
			(*_DataP)[i] += _buf[i];

		return;
	}

	//引き算を行います。
	void _Sub(const Matrix& _Data, std::vector<_T>* _DataP, SizeT _size)
	{
		if (_DataP->size() != _Data._Main.size() || _size.first != _Data._Size.first || _size.second != _Data._Size.second)
			throw std::invalid_argument("Must be same size.");

		for (Ulong i = 0; i < _DataP->size(); i++)
			(*_DataP)[i] -= _Data._Main[i];

		return;
	}
	void _Sub(std::vector<_T>& _buf, std::vector<_T>* _DataP, SizeT _size)
	{
		if (_DataP->size() != _buf.size() || _size.first != _buf[0].size() || _size.second != _buf.size)
			throw std::invalid_argument("Must be same size.");

		for (Ulong i = 0; i < _DataP->size(); i++)
			(*_DataP)[i] -= _buf[i];

		return;
	}

	//掛け算を行います。
	void _scalar_mul(std::vector<std::vector<_T>>& _buf,_T _mul_num) {
		for (Ulong i = 0; i < _buf.size();i++)
			_buf[i] *= _mul_num;

		return;
	}
	void _mul(std::vector<_T>& _buf1,SizeT _size1,std::vector<_T>& _buf2,SizeT _size2,std::vector<_T>* _DataP,SizeT* _SizeP) {
		//列と行が同じでなければならない。
		if (_size1.first != _size2.second)
			throw std::invalid_argument("Must be same line and column.");

		_DataP->erase(_DataP->begin(), _DataP->end());
		_DataP->resize(_size1.second * _size2.first);

		//計算結果格納先
		(*_SizeP) = { _size2.first,_size1.second };

		for (Ulong i = 0; i < _size1.second;i++) {
			for (Ulong j = 0; j < _size2.first; j++) {
				_T num = 0;

				for (Ulong k = 0; k < _size2.second; k++)
					num += _buf1[this->_Convert_to_ArrayNum(_size1.first, { k,i })] * _buf2[this->_Convert_to_ArrayNum(_size2.first, { j,k })];

				//誤差の修正
				(*_DataP)[this->_Convert_to_ArrayNum(_SizeP->first, { j,i })] = (num-(Slong)num)<=_ERROR_SANAE?(Slong)num:num;
			}
		}

		return;
	}

	//サラスの方式で解きます。(2次元)
	_T _det_2(std::vector<_T>* _DataP,_T _coeff,SizeT _size) 
	{
		if (_size.first != _size.second)
			throw std::invalid_argument("Must be same column and line.");
		
		if (_size.first != 2)
			throw std::invalid_argument("Must pass 2 as argument.");
		
		return _coeff * (((*_DataP)[this->_Convert_to_ArrayNum(2, {0,0})]* (*_DataP)[this->_Convert_to_ArrayNum(2, { 1,1 })])-((*_DataP)[this->_Convert_to_ArrayNum(2, { 1,0 })] * (*_DataP)[this->_Convert_to_ArrayNum(2, { 0,1 })]));
	}

	//余因子展開をして行列を一次元下げます。下げた値は_storeに格納される。係数はcoeffに格納される。
	void _confactor_expansion_1(std::vector<_T>& _Data,SizeT _size,std::vector<std::vector<_T>>* _store,std::vector<_T>* _coeff) {
		if (_size.first != _size.second)
			throw std::invalid_argument("Must be same size.");
		
		for (Ulong i = 0; i < _size.first; i++) {
			std::vector<_T> _buffer;

			for (Ulong y = 0; y < _size.first; y++)
				for (Ulong x = 0; x < _size.second;x++)
					if (x != 0 && y != i)
						_buffer.push_back(_Data[this->_Convert_to_ArrayNum(_size.first, {x,y})]);

			_coeff->push_back(_Data[this->_Convert_to_ArrayNum(_size.first, {0,i})]*std::pow(-1,1+(i+1)));
			_store->push_back(_buffer);
		}
	}
	//2次元になるまで余因子展開を行います。
	std::pair<std::vector<std::vector<_T>>, std::vector<_T>> _confactor_expansion_to_2(std::vector<_T>& _Data,SizeT _size,_T _in_coeff=1) {
		if (_size.first != _size.second)
			throw std::invalid_argument("Must be same size.");

		if (_size.first < 3)
			throw std::invalid_argument("Must pass above 3 as argument.");

		//行列格納用
		std::vector<std::vector<_T>> _buf;
		//係数格納用
		std::vector<_T>              _coeff;

		//一次元下げる
		this->_confactor_expansion_1(_Data,_size,&_buf,&_coeff);

		for (Ulong i = 0; i < _coeff.size();i++)
			_coeff[i] *= _in_coeff;

		if (_size.first == 3)
			return { _buf,_coeff };
		
		std::vector<std::pair<std::vector<std::vector<_T>>, std::vector<_T>>> _bufs;
		for (Ulong i = 0; i < _buf.size(); i++)
			_bufs.push_back(this->_confactor_expansion_to_2(_buf[i], {_size.first-1,_size.second-1},_coeff[i]));

		//行列を係数の格納
		std::pair<std::vector<std::vector<_T>>, std::vector<_T>> _retdata;

		for (Ulong i = 0; i < _bufs.size();i++) {
			for (Ulong j = 0; j < _bufs[i].first.size();j++) {
				_retdata.first.push_back(_bufs[i].first[j]);
				_retdata.second.push_back(_bufs[i].second[j]);
			}
		}

		return _retdata;
	}
	//行列式を求めます。
	_T _det(std::vector<_T>& _Data,SizeT _size) {
		if (_size.first != _size.second)
			throw std::invalid_argument("Must be same size.");

		switch (_size.first) {
		case 1:
			return _Data[0];
		case 2:
			return this->_det_2(&_Data, 1, {2,2});
		default:
			break;
		}

		_T _ret = 0;
		std::pair<std::vector<std::vector<_T>>, std::vector<_T>> _buf = this->_confactor_expansion_to_2(_Data,_size);
		
		for (Ulong i = 0; i < _buf.first.size(); i++)
			_ret += this->_det_2(&_buf.first[i], _buf.second[i], { 2,2 });
		
		return _ret;
	}

	//逆行列を求める。
	void _inverse_matrix(std::vector<_T>* _Data, std::vector<_T>* _to,SizeT _size) {
		if (this->Det() == 0)
			throw std::runtime_error("Inverse does not exist.");

		//単位行列にする。
		this->_To_Identity_Matrix(_to,_size);
		
		//_pos:基準
		for (Ulong _pos = 0; _pos < _size.first; _pos++) {
			for (Ulong y = 0; y < _size.second; y++) {
				if (_pos != y) {
					_T div = 0;

					//基準が0の場合行を入れ替える。
					if ((*_Data)[this->_Convert_to_ArrayNum(_size.first, { _pos,_pos })] == 0) {
						for (Ulong findy = 0; findy < _size.second; findy++) {
							if ((*_Data)[this->_Convert_to_ArrayNum(_size.first, { _pos,findy })] != 0) {
								this->_Swap_Line(_pos,findy,_Data,_size);
								this->_Swap_Line(_pos,findy,_to,_size);
							}
						}
					}

					//ax+b=0 x -= b/a  
					div = ((*_Data)[this->_Convert_to_ArrayNum(_size.first, { _pos,y })] / (*_Data)[this->_Convert_to_ArrayNum(_size.first, { _pos,_pos })]);

					//その他列への適用
					for (Ulong x = 0; x < _size.first; x++) {
						(*_Data)[this->_Convert_to_ArrayNum(_size.first, { x,y })] -= div * ((*_Data)[this->_Convert_to_ArrayNum(_size.first, { x,_pos })]);
						(*_to)[this->_Convert_to_ArrayNum(_size.first, { x,y })] -= div * ((*_to)[this->_Convert_to_ArrayNum(_size.first, { x,_pos })]);
					}
				}
			}
		}

		//基準が1になっていない場合その行へ適用し1にする。
		for (Ulong _pos = 0; _pos < _size.first; _pos++) {
			for (Ulong _x = 0; _x < _size.first; _x++) {
				_T _is_zero_buf = (*_Data)[this->_Convert_to_ArrayNum(_size.first, { _pos,_pos })];
				if (_is_zero_buf != 0)
					(*_to)[this->_Convert_to_ArrayNum(_size.first, { _x,_pos })] /= _is_zero_buf;
			}
		}

		return;
	}

//Define functions(public)
public:
	//Constructor
	Matrix() {}
	Matrix(std::pair<SizeT,std::vector<_T>> _In)
	{
		this->_Size = _In.first ;
		this->_Main = _In.second;

		for (Ulong i = this->_Main.size(); i < (this->_Size.first*this->_Size.second);i++)
			this->_Main[i] = 0;

		return;
	}
	Matrix(std::initializer_list<std::vector<_T>> _In)
	{
		std::vector<std::vector<_T>> _buf = _In;

		this->_Size.second = _In.size();
		this->_Size.first  = _buf[0].size();

		for (Ulong line = 0; line < this->_Size.second; line++)
			this->_Main.insert(this->_Main.end(),_buf[line].begin(),_buf[line].end());

		return;
	}
	Matrix(std::vector<_T> _Data,SizeT _size) {
		this->_Main = _Data;
		this->_Size = _size;
	}
	Matrix(const Matrix<_T>& _data)
	{
		this->_Size = _data._Size;
		this->_Main = _data._Main;
	}

	//Operator
	_T& operator [](Ulong _ArrayNum) 
	{
		if (this->_Main.size() <= _ArrayNum)
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[_ArrayNum];
	}
	_T& operator [](SizeT _Num) 
	{
		if (this->_Main.size() <= this->_Convert_to_ArrayNum(this->_Size.first,_Num))
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[this->_Convert_to_ArrayNum(this->_Size.first,_Num)];
	}

	Matrix& operator =(const Matrix<_T>& _data)
	{
		this->_Size = _data._Size;
		this->_Main = _data._Main;

		return *this;
	}
	Matrix& operator =(std::initializer_list<std::vector<_T>> _In)
	{
		this->_Main.erase(this->_Main.begin(),this->_Main.end());
		std::vector<std::vector<_T>> _buf = _In;

		this->_Size.second = _In.size();
		this->_Size.first  = _buf[0].size();

		for (Ulong line = 0; line < this->_Size.second; line++)
			this->_Main.insert(this->_Main.end(), _buf[line].begin(), _buf[line].end());

		return *this;
	}
	Matrix& operator +=(const Matrix& _Data) {
		this->_Add(_Data._Main,&this->_Main,this->_Size);

		return *this;
	}
	Matrix& operator -=(const Matrix& _data) {
		this->_Sub(_data._Main,&this->_Main,this->_Size);

		return *this;
	}
	Matrix  operator +(const Matrix& _Data) 
	{
		std::vector<_T> _ret = this->_Main;
		this->_Add(_Data,&_ret,this->_Size);

		return std::pair<SizeT, std::vector<_T>>{this->_Size,_ret};
	}
	Matrix  operator -(const Matrix& _Data)
	{
		std::vector<_T> _ret = this->_Main;
		this->_Sub(_Data, &_ret, this->_Size);

		return std::pair<SizeT, std::vector<_T>>{this->_Size, _ret};
	}
	Matrix  operator *(const Matrix& _Data)
	{
		std::vector<_T> _ret;
		SizeT           _ret_size;

		std::vector<_T> _buf      = this->_Main;
		SizeT           _buf_size = this->_Size;
		
		this->_mul(_buf, _buf_size, (std::vector<_T>&)_Data._Main, _Data._Size, &_ret, &_ret_size);

		return std::pair<SizeT, std::vector<_T>>{_ret_size,_ret};
	}

	//足し算を行います。
	Matrix& Add(const Matrix& _Data)
	{
		this->_Add(_Data._Main, &this->_Main, this->_Size);

		return *this;
	}
	//引き算を行います。
	Matrix& Sub(const Matrix& _Data)
	{
		this->_Sub(_Data._Main,&this->_Main,this->_Size);

		return *this;
	}

	//スカラー倍を行います。
	Matrix& Scalar_Mul(_T num) {
		this->_scalar_mul(&this->_Main,num);

		return *this;
	}
	//行列*行列を行います。
	Matrix& Mul(const Matrix& _Data) {
		std::vector<_T> _buf      = this->_Main;
		SizeT           _buf_size = this->_Size;

		this->_mul(_buf,_buf_size,(std::vector<_T>&)_Data._Main,_Data._Size,&this->_Main,&this->_Size);

		return *this;
	}

	//行を入れ替えます。
	Matrix& Swap_Line(Ulong Line1,Ulong Line2) 
	{
		this->_Swap_Line(Line1,Line2,&this->_Main,this->_Size);

		return *this;
	}
	//列を入れ替えます。
	Matrix& Swap_Column(Ulong Column1, Ulong Column2) 
	{
		this->_Swap_Line(Column1, Column2, &this->_Main, this->_Size);

		return *this;
	}

	//単位行列にします。
	Matrix& Ident() 
	{
		this->_To_Identity_Matrix(&this->_Main,this->_Size);

		return *this;
	}

	//行列式を求めます。
	_T Det() 
	{
		return this->_det(this->_Main,this->_Size);
	}

	//行列の転置を行います。
	Matrix Transpose() 
	{
		std::vector<_T> _Data;
		_Data.resize(this->_Size.first*this->_Size.second);

		for (Ulong x = 0; x < this->_Size.first;x++)
			for (Ulong y = 0; y < this->_Size.second;y++)
				_Data[this->_Convert_to_ArrayNum(this->_Size.second, { y,x })] = this->_Main[this->_Convert_to_ArrayNum(this->_Size.first, {x,y})];
		
		return std::pair<SizeT, std::vector<_T>>{ SizeT{this->_Size.second,this->_Size.first},_Data };
	}

	//逆行列を返します。
	Matrix Inverse() 
	{
		std::vector<_T> _to;
		std::vector<_T> _buf = this->_Main;

		this->_inverse_matrix(&_buf,&_to,this->_Size);

		return std::pair<SizeT, std::vector<_T>>{ this->_Size, _to };
	}

	//行列を表示します。
	Matrix& View()
	{
		this->_View(&this->_Main, this->_Size);

		return *this;
	}

	//シグモイド関数に通します。
	Matrix& Sigmoid() {
		for (Ulong i = 0; i < this->_Main.size(); i++)
			this->_Main[i] = 1 / (1 + std::exp(-1 * (this->_Main[i])));

		return *this;
	}
};
#endif