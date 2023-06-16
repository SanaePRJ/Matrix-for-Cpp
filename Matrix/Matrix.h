/*=============================================================
* NAME      : Matrix.h 
* AUTHOR    : SanaeProject
* VER       : 1.0.0
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
	std::vector<_T>   _Main;                        //行列を格納する。
	SizeT             _Size = std::make_pair(0,0);  //行列のサイズを格納する。 first:列 second:行


//Define functions(private)
private:
	//Converter
	Ulong _Convert_to_ArrayNum            //{列,行}を配列番号に変換する。
	(
		Ulong _Width,
		SizeT _Pos
	) 
	{
		return (_Width*_Pos.second)+_Pos.first;
	}

	SizeT _Convert_to_Position            //行列番号を{列,行}に変換する。
	(
		Ulong _Width,
		Ulong _Arraynum
	)  
	{
		return std::make_pair(_Arraynum%_Width,(_Arraynum - (_Arraynum % _Width))/_Width);
	}

	void _Convert_to_Array                //初期値をstd::vector<_T>へ変換する。
	(
		std::initializer_list<std::vector<_T>>& _Input
		,std::vector<_T>&                       _Storage
	)                              
	{
		std::vector<std::vector<_T>> _bufs = _Input;

		for (Ulong i = 0; i < _bufs.size(); i++)
			_Storage.insert(_Storage.end(),_bufs[i].begin(),_bufs[i].end());
	}

	void _View                            //行列を表示する。
	(
		std::vector<_T>* _DataP,
		SizeT            _Size
	)          
	{
		for (Ulong i = 0; i < _DataP->size(); i++) {
			if (i != 0 && i % _Size.first == 0) std::cout << "\n";
			
			std::cout << (*_DataP)[i] << " ";
		}
		std::cout << "\n";
		
		return;
	}

	void _Swap_Line                        //指定された行を入れ替えます。
	(
		Ulong             _Line1,
		Ulong             _Line2,
		std::vector<_T>*  _Data,
		SizeT             _Size
	)              
	{
		for (Ulong i = 0; i < _Size.first ;i++) {
			_T _temp = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,_Line1 })];

			(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,_Line1 })] = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, {i,_Line2})];
			(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,_Line2 })] = _temp;
		}

		return;
	}

	void _Swap_Column                     //指定した列を入れ替えます。
	(
		Ulong            _Column1, 
		Ulong            _Column2,
		std::vector<_T>* _Data,
		SizeT            _Size
	)      
	{
		for (Ulong i = 0; i < _Size.second; i++) {
			_T _temp = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, {_Column1,i})];

			(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _Column1,i })] = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _Column2,i })];
			(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _Column2,i })] = _temp;
		}

		return;
	}

	void _To_Identity_Matrix              //単位行列にします。
	(
		std::vector<_T>* _Data,
		SizeT            _Size
	)         
	{
		_Data->erase (_Data->begin(),_Data->end()); //全データの削除
		_Data->resize(_Size.first*_Size.second);    //サイズの変更

		for (Ulong i = 0; i < _Size.first; i++)
			for (Ulong j = 0; j < _Size.second; j++)
				(*_Data)[this->_Convert_to_ArrayNum(_Size.first, {i,j})] = i == j ? 1 : 0; //行と列の番号が同じ場合1それ以外の場合0

		return;
	}

	void _Add                             //足し算を行います。
	(
		const Matrix&    _Data,
		std::vector<_T>* _DataP,
		SizeT            _Size
	) 
	{
		if (_DataP->size() != _Data._Main.size() || _Size.first != _Data._Size.first || _Size.second != _Data._Size.second)
			throw std::invalid_argument("Must be same size.");

		for (Ulong i = 0; i < _DataP->size();i++)
			(*_DataP)[i] += _Data._Main[i];

		return;
	}

	void _Sub                             //引き算を行います。
	(
		const Matrix&    _Data, 
		std::vector<_T>* _DataP, 
		SizeT            _Size
	)
	{
		if (_DataP->size() != _Data._Main.size() || _Size.first != _Data._Size.first || _Size.second != _Data._Size.second)
			throw std::invalid_argument("Must be same size.");

		for (Ulong i = 0; i < _DataP->size(); i++)
			(*_DataP)[i] -= _Data._Main[i];

		return;
	}

	void _Scalar_mul                      //スカラー倍を行います。
	(
		std::vector<_T>& _Data,
		_T               _Mul_num
	) 
	{
		for (Ulong i = 0; i < _Data.size();i++)
			_Data[i] *= _Mul_num;

		return;
	}
	
	void _Mul                             //行列の乗算を行います。
	(
		std::vector<_T>& _Data1,
		SizeT            _Size1,

		std::vector<_T>& _Data2,
		SizeT            _Size2,

		std::vector<_T>* _Storage,
		SizeT*           _SizeP
	)
	{
		if (_Size1.first != _Size2.second) //列と行が同じでなければならない。
			throw std::invalid_argument("Must be same line and column.");

		_Storage->erase(_Storage->begin(), _Storage->end());
		_Storage->resize(_Size1.second * _Size2.first);

		(*_SizeP) = { _Size2.first,_Size1.second }; //計算結果格納先

		for (Ulong i = 0; i < _Size1.second;i++) {
			for (Ulong j = 0; j < _Size2.first; j++) {
				_T num = 0;

				for (Ulong k = 0; k < _Size2.second; k++)
					num += _Data1[this->_Convert_to_ArrayNum(_Size1.first, { k,i })] * _Data2[this->_Convert_to_ArrayNum(_Size2.first, { j,k })];

				//誤差の修正
				(*_Storage)[this->_Convert_to_ArrayNum(_SizeP->first, { j,i })] = (num-(Slong)num)<=_ERROR_SANAE?(Slong)num:num;
			}
		}

		return;
	}

	_T _Det_2                             //サラスの方式で解きます。(2次元)1
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
		
		return _Coeff * (((*_DataP)[this->_Convert_to_ArrayNum(2, {0,0})]* (*_DataP)[this->_Convert_to_ArrayNum(2, { 1,1 })])-((*_DataP)[this->_Convert_to_ArrayNum(2, { 1,0 })] * (*_DataP)[this->_Convert_to_ArrayNum(2, { 0,1 })]));
	}

	void _Confactor_expansion_1           //余因子展開をして行列を一次元下げます。下げた値は_Storageに格納され係数は_Coeffに格納されます。
	(
		std::vector<_T>&              _Data,
		SizeT                         _Size,
		std::vector<std::vector<_T>>* _Storage,
		std::vector<_T>*              _Coeff
	)
	{
		if (_Size.first != _Size.second)
			throw std::invalid_argument("Must be same size.");
		
		for (Ulong i = 0; i < _Size.first; i++) {
			std::vector<_T> _buffer;

			for (Ulong y = 0; y < _Size.first; y++)
				for (Ulong x = 0; x < _Size.second;x++)
					if (x != 0 && y != i)
						_buffer.push_back(_Data[this->_Convert_to_ArrayNum(_Size.first, {x,y})]);

			_Coeff  ->push_back(_Data[this->_Convert_to_ArrayNum(_Size.first, {0,i})]*std::pow(-1,1+(i+1)));
			_Storage->push_back(_buffer);
		}
	}
	
	std::pair<std::vector<std::vector<_T>>, std::vector<_T>> 
		_Confactor_expansion_to_2            //2次元になるまで余因子展開を行います。
	(
		std::vector<_T>& _Data,
		SizeT            _Size,
		_T               _In_Coeff = 1
	) 
	{
		if (_Size.first != _Size.second)
			throw std::invalid_argument("Must be same size.");

		if (_Size.first < 3)
			throw std::invalid_argument("Must pass above 3 as argument.");

		std::vector<std::vector<_T>> _buf;   //行列格納用
		std::vector<_T>              _coeff; //係数格納用
		
		this->_Confactor_expansion_1(_Data,_Size,&_buf,&_coeff); //一次元下げる

		for (Ulong i = 0; i < _coeff.size();i++)
			_coeff[i] *= _In_Coeff;

		if (_Size.first == 3)
			return { _buf,_coeff };
		
		std::vector<std::pair<std::vector<std::vector<_T>>, std::vector<_T>>> _bufs;
		for (Ulong i = 0; i < _buf.size(); i++)
			_bufs.push_back(this->_Confactor_expansion_to_2(_buf[i], {_Size.first-1,_Size.second-1},_coeff[i])); //自分自身を呼び出す。

		//行列を係数の格納
		std::pair<std::vector<std::vector<_T>>, std::vector<_T>> _retdata;

		for (Ulong i = 0; i < _bufs.size();i++) {
			for (Ulong j = 0; j < _bufs[i].first.size();j++) {
				_retdata.first .push_back(_bufs[i].first [j]);
				_retdata.second.push_back(_bufs[i].second[j]);
			}
		}

		return _retdata;
	}
	
	_T _Det                               //行列式を求めます。
	(
		std::vector<_T>& _Data,
		SizeT            _Size
	) 
	{
		if (_Size.first != _Size.second)
			throw std::invalid_argument("Must be same size.");

		switch (_Size.first) {
		case 1:
			return _Data[0];
		case 2:
			return this->_Det_2(&_Data, 1, {2,2});
		default:
			break;
		}

		_T _ret = 0;
		std::pair<std::vector<std::vector<_T>>, std::vector<_T>> _buf = this->_Confactor_expansion_to_2(_Data,_Size);
		
		for (Ulong i = 0; i < _buf.first.size(); i++)
			_ret += this->_Det_2(&_buf.first[i], _buf.second[i], { 2,2 });
		
		return _ret;
	}

	void _Inverse_matrix                  //逆行列を求める
	(
		std::vector<_T>* _Data,
		std::vector<_T>* _Storage,
		SizeT            _Size
	)
	{
		if (this->Det() == 0)
			throw std::runtime_error("Inverse does not exist.");

		//単位行列にする。
		this->_To_Identity_Matrix(_Storage,_Size);
		
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
						(*_Storage  )[this->_Convert_to_ArrayNum(_Size.first, { x,y })] -= div * ((*_Storage  )[this->_Convert_to_ArrayNum(_Size.first, { x,_pos })]);
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


//Define functions(public)
public:
	//Constructor
	Matrix() 
	{
	}

	Matrix
	(
		const std::vector<_T>& _Main, 
		const SizeT&           _Size
	)
		: _Main(_Main), _Size(_Size)
	{
	}

	Matrix
	(
		const Matrix<_T>& _Data
	)
		: _Main(_Data._Main), _Size(_Data._Size)
	{
	}

	Matrix
	(
		std::pair<SizeT,std::vector<_T>> _In
	)
	{
		this->_Size = _In.first ;
		this->_Main = _In.second;

		for (Ulong i = this->_Main.size(); i < (this->_Size.first*this->_Size.second);i++)
			this->_Main[i] = 0;

		return;
	}

	Matrix
	(
		std::initializer_list<std::vector<_T>> _In
	)
	{
		std::vector<std::vector<_T>> _buf = _In;

		this->_Size.second = _In.size();
		this->_Size.first  = _buf[0].size();

		for (Ulong line = 0; line < this->_Size.second; line++)
			this->_Main.insert(this->_Main.end(),_buf[line].begin(),_buf[line].end());

		return;
	}


	//Operator
	bool operator==                       //比較
	(
		const Matrix& _Data
	)
	{
		if (_Data._Size.first != this->_Size.first || _Data._Size.second != this->_Size.second)
			return false;
		
		for (Ulong i = 0; i < this->_Main.size();i++)
			if (_Data._Main[i] != this->_Main[i])
				return false;
		
		return true;
	}

	_T& operator[]                        //配列参照
	(
		Ulong _ArrayNum
	) 
	{
		if (this->_Main.size() <= _ArrayNum)
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[_ArrayNum];
	}

	_T& operator[]                        //{列,行}参照
	(
		SizeT _Num
	) 
	{
		if (this->_Main.size() <= this->_Convert_to_ArrayNum(this->_Size.first,_Num))
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[this->_Convert_to_ArrayNum(this->_Size.first,_Num)];
	}

	Matrix& operator=                     //代入
	(
		const Matrix<_T>& _Data
	)
	{
		this->_Size = _Data._Size;
		this->_Main = _Data._Main;

		return *this;
	}
	
	Matrix& operator=                     //代入
	(
		std::initializer_list<std::vector<_T>> _In
	)
	{
		this->_Main.erase(this->_Main.begin(),this->_Main.end());
		std::vector<std::vector<_T>> _buf = _In;

		this->_Size.second = _In.size();
		this->_Size.first  = _buf[0].size();

		for (Ulong line = 0; line < this->_Size.second; line++)
			this->_Main.insert(this->_Main.end(), _buf[line].begin(), _buf[line].end());

		return *this;
	}
	
	Matrix& operator+=
	(
		const Matrix& _Data
	)
	{
		this->_Add(_Data,&this->_Main,this->_Size);

		return *this;
	}
	
	Matrix& operator-=
	(
		const Matrix& _Data
	)
	{
		this->_Sub(_Data,&this->_Main,this->_Size);

		return *this;
	}
	
	Matrix& operator*=
	(
		const Matrix& _Data
	)
	{
		std::vector<_T> _ret;
		SizeT           _ret_size;

		std::vector<_T> _buf = this->_Main;
		SizeT           _buf_size = this->_Size;

		this->_Mul(_buf, _buf_size, (std::vector<_T>&)_Data._Main, _Data._Size, &this->_Main, &this->_Size);

		return *this;
	}
	
	Matrix& operator*=
	(
		_T _Num
	)
	{
		for (Ulong i = 0; i < this->_Main.size(); i++)
			this->_Main[i] *= _Num;

		return *this;
	}

	Matrix  operator+
	(
		const Matrix& _Data
	) 
	{
		std::vector<_T> _ret = this->_Main;
		this->_Add(_Data,&_ret,this->_Size);

		return std::pair<SizeT, std::vector<_T>>{this->_Size,_ret};
	}
	
	Matrix  operator-
	(
		const Matrix& _Data
	)
	{
		std::vector<_T> _ret = this->_Main;
		this->_Sub(_Data, &_ret, this->_Size);

		return std::pair<SizeT, std::vector<_T>>{this->_Size, _ret};
	}
	
	Matrix  operator*
	(
		const Matrix& _Data
	)
	{
		std::vector<_T> _ret;
		SizeT           _ret_size;

		std::vector<_T> _buf      = this->_Main;
		SizeT           _buf_size = this->_Size;
		
		this->_Mul(_buf, _buf_size, (std::vector<_T>&)_Data._Main, _Data._Size, &_ret, &_ret_size);

		return std::pair<SizeT, std::vector<_T>>{_ret_size,_ret};
	}

	Matrix  operator*
	(
		_T _Num
	) 
	{
		std::vector<_T> _data = this->_Main;

		for (Ulong i = 0; i < _data.size(); i++)
			_data[i] *= _Num;

		return std::pair<SizeT,std::vector<_T>>{this->_Size,_data};
	}

	Matrix& Add                           //足し算を行う。
	(
		const Matrix& _Data
	)
	{
		this->_Add(_Data, &this->_Main, this->_Size);

		return *this;
	}

	Matrix& Sub                           //引き算を行う。
	(
		const Matrix& _Data
	)
	{
		this->_Sub(_Data,&this->_Main,this->_Size);

		return *this;
	}

	Matrix& Scalar_Mul                    //スカラー倍を行う。
	(
		_T _Num
	)
	{
		this->_Scalar_mul(this->_Main,_Num);

		return *this;
	}
	
	Matrix& Mul                           //行列どうしの乗算を行う。
	(
		const Matrix& _Data
	)
	{
		std::vector<_T> _buf      = this->_Main;
		SizeT           _buf_size = this->_Size;

		this->_Mul(_buf,_buf_size,(std::vector<_T>&)_Data._Main,_Data._Size,&this->_Main,&this->_Size);

		return *this;
	}

	Matrix& Swap_Line                     //行を入れ替える。
	(
		Ulong _Line1,
		Ulong _Line2
	) 
	{
		this->_Swap_Line(_Line1,_Line2,&this->_Main,this->_Size);

		return *this;
	}
	
	Matrix& Swap_Column                   //列を入れ替える。
	(
		Ulong _Column1,
		Ulong _Column2
	) 
	{
		this->_Swap_Column(_Column1, _Column2, &this->_Main, this->_Size);
		
		return *this;
	}

	Matrix& Ident()                       //単位行列にする。
	{
		this->_To_Identity_Matrix(&this->_Main,this->_Size);

		return *this;
	}

	_T Det()                              //行列式を求めます。 
	{
		return this->_Det(this->_Main,this->_Size);
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

		this->_Inverse_matrix(&_buf,&_to,this->_Size);

		return std::pair<SizeT, std::vector<_T>>{ this->_Size, _to };
	}

	//行列を表示します。
	Matrix& View()
	{
		this->_View(&this->_Main, this->_Size);

		return *this;
	}

	//シグモイド関数に通します。
	Matrix& Sigmoid() 
	{
		for (Ulong i = 0; i < this->_Main.size(); i++)
			this->_Main[i] = 1 / (1 + std::exp(-1 * (this->_Main[i])));

		return *this;
	}
};
#endif