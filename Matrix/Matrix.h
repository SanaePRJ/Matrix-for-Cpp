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


//��Z��10^-6�ȉ��̌덷�͖�������B
constexpr auto _ERROR_SANAE = 1e-6;


template<typename _T = double> class Matrix {
//Define variables(private)
private:
	std::vector<_T>   _Main;                        //�s����i�[����B
	SizeT             _Size = std::make_pair(0,0);  //�s��̃T�C�Y���i�[����B first:�� second:�s


//Define functions(private)
private:
	//Converter
	//{��,�s}��z��ԍ��ɕϊ�����B
	Ulong _Convert_to_ArrayNum
	(
		Ulong _Width,
		SizeT _Pos
	) 
	{
		return (_Width*_Pos.second)+_Pos.first;
	}

	//�s��ԍ���{��,�s}�ɕϊ�����B
	SizeT _Convert_to_Position
	(
		Ulong _Width,
		Ulong _Arraynum
	)  
	{
		return std::make_pair(_Arraynum%_Width,(_Arraynum - (_Arraynum % _Width))/_Width);
	}

	//�����l��std::vector<_T>�֕ϊ�����B
	void _Convert_to_Array
	(
		std::initializer_list<std::vector<_T>>& _Input,
		std::vector<_T>&                       _Storage
	)                              
	{
		std::vector<std::vector<_T>> _bufs = _Input;

		for (Ulong i = 0; i < _bufs.size(); i++)
			_Storage.insert(_Storage.end(),_bufs[i].begin(),_bufs[i].end());
	}

	//�s���\������B
	void _View
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

	//�w�肳�ꂽ�s�����ւ��܂��B
	void _Swap_Line
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

	//�w�肵��������ւ��܂��B
	void _Swap_Column
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

	//�P�ʍs��ɂ��܂��B
	void _To_Identity_Matrix
	(
		std::vector<_T>* _Data,
		SizeT            _Size
	)         
	{
		_Data->erase (_Data->begin(),_Data->end()); //�S�f�[�^�̍폜
		_Data->resize(_Size.first*_Size.second);    //�T�C�Y�̕ύX

		for (Ulong i = 0; i < _Size.first; i++)
			for (Ulong j = 0; j < _Size.second; j++)
				(*_Data)[this->_Convert_to_ArrayNum(_Size.first, {i,j})] = i == j ? 1 : 0; //�s�Ɨ�̔ԍ��������ꍇ1����ȊO�̏ꍇ0

		return;
	}

	//�����Z���s���܂��B
	void _Add
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

	//�����Z���s���܂��B
	void _Sub
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

	//�X�J���[�{���s���܂��B
	void _Scalar_mul
	(
		std::vector<_T>& _Data,
		_T               _Mul_num
	) 
	{
		for (Ulong i = 0; i < _Data.size();i++)
			_Data[i] *= _Mul_num;

		return;
	}
	
	//�s��̏�Z���s���܂��B
	void _Mul
	(
		std::vector<_T>& _Data1,
		SizeT            _Size1,

		std::vector<_T>& _Data2,
		SizeT            _Size2,

		std::vector<_T>* _Storage,
		SizeT*           _SizeP
	)
	{
		if (_Size1.first != _Size2.second) //��ƍs�������łȂ���΂Ȃ�Ȃ��B
			throw std::invalid_argument("Must be same line and column.");

		_Storage->erase(_Storage->begin(), _Storage->end());
		_Storage->resize(_Size1.second * _Size2.first);

		(*_SizeP) = { _Size2.first,_Size1.second }; //�v�Z���ʊi�[��

		for (Ulong i = 0; i < _Size1.second;i++) {
			for (Ulong j = 0; j < _Size2.first; j++) {
				_T num = 0;

				for (Ulong k = 0; k < _Size2.second; k++)
					num += _Data1[this->_Convert_to_ArrayNum(_Size1.first, { k,i })] * _Data2[this->_Convert_to_ArrayNum(_Size2.first, { j,k })];

				//�덷�̏C��
				(*_Storage)[this->_Convert_to_ArrayNum(_SizeP->first, { j,i })] = (num-(Slong)num)<=_ERROR_SANAE?(Slong)num:num;
			}
		}

		return;
	}

	//�T���X�̕����ŉ����܂��B(2����)
	_T _Det_2
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

	//�]���q�W�J�����čs����ꎟ�������܂��B�������l��_Storage�Ɋi�[����W����_Coeff�Ɋi�[����܂��B
	void _Confactor_expansion_1
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
	
	//2�����ɂȂ�܂ŗ]���q�W�J���s���܂��B
	std::pair<std::vector<std::vector<_T>>, std::vector<_T>> 
		_Confactor_expansion_to_2
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

		std::vector<std::vector<_T>> _buf;   //�s��i�[�p
		std::vector<_T>              _coeff; //�W���i�[�p
		
		this->_Confactor_expansion_1(_Data,_Size,&_buf,&_coeff); //�ꎟ��������

		for (Ulong i = 0; i < _coeff.size();i++)
			_coeff[i] *= _In_Coeff;

		if (_Size.first == 3)
			return { _buf,_coeff };
		
		std::vector<std::pair<std::vector<std::vector<_T>>, std::vector<_T>>> _bufs;
		for (Ulong i = 0; i < _buf.size(); i++)
			_bufs.push_back(this->_Confactor_expansion_to_2(_buf[i], {_Size.first-1,_Size.second-1},_coeff[i])); //�������g���Ăяo���B

		//�s����W���̊i�[
		std::pair<std::vector<std::vector<_T>>, std::vector<_T>> _retdata;

		for (Ulong i = 0; i < _bufs.size();i++) {
			for (Ulong j = 0; j < _bufs[i].first.size();j++) {
				_retdata.first .push_back(_bufs[i].first [j]);
				_retdata.second.push_back(_bufs[i].second[j]);
			}
		}

		return _retdata;
	}
	
	//�s�񎮂����߂܂��B
	_T _Det
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

	//�t�s������߂�
	void _Inverse_matrix
	(
		std::vector<_T>* _Data,
		std::vector<_T>* _Storage,
		SizeT            _Size
	)
	{
		if (this->Det() == 0)
			throw std::runtime_error("Inverse does not exist.");

		//�P�ʍs��ɂ���B
		this->_To_Identity_Matrix(_Storage,_Size);
		
		//_pos:�
		for (Ulong _pos = 0; _pos < _Size.first; _pos++) {
			for (Ulong y = 0; y < _Size.second; y++) {
				if (_pos != y) {
					_T div = 0;

					//���0�̏ꍇ�s�����ւ���B
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

					//���̑���ւ̓K�p
					for (Ulong x = 0; x < _Size.first; x++) {
						(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { x,y })] -= div * ((*_Data)[this->_Convert_to_ArrayNum(_Size.first, { x,_pos })]);
						(*_Storage  )[this->_Convert_to_ArrayNum(_Size.first, { x,y })] -= div * ((*_Storage  )[this->_Convert_to_ArrayNum(_Size.first, { x,_pos })]);
					}
				}
			}
		}

		//���1�ɂȂ��Ă��Ȃ��ꍇ���̍s�֓K�p��1�ɂ���B
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
		const std::vector<_T>& _In_Main, 
		const SizeT&           _In_Size
	)
		: _Main(_In_Main), _Size(_In_Size)
	{
	}

	Matrix
	(
		const Matrix<_T>& _In_Data
	)
		: _Main(_In_Data._Main), _Size(_In_Data._Size)
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

	Matrix
	(
		SizeT _In_Size
	) 
		: _Size(_In_Size)
	{
		this->_Main.resize(_In_Size.first*_In_Size.second);
	}

	//Operator
	//��r
	bool operator==
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

	//�z��Q��
	_T& operator[]
	(
		Ulong _ArrayNum
	) 
	{
		if (this->_Main.size() <= _ArrayNum)
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[_ArrayNum];
	}

	//{��,�s}�Q��
	_T& operator[]
	(
		SizeT _Num
	) 
	{
		if (this->_Main.size() <= this->_Convert_to_ArrayNum(this->_Size.first,_Num))
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[this->_Convert_to_ArrayNum(this->_Size.first,_Num)];
	}

	//���
	Matrix& operator=
	(
		const Matrix<_T>& _Data
	)
	{
		this->_Size = _Data._Size;
		this->_Main = _Data._Main;

		return *this;
	}
	
	//���
	Matrix& operator=
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

		std::vector<_T> _buf      = this->_Main;
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

	//�T�C�Y�ύX(�f�[�^�͂��ׂč폜����܂��B)
	Matrix& SetSize
	(
		SizeT _Data
	)
	{
		this->_Main.erase (this->_Main.begin(),this->_Main.end());
		this->_Main.resize(_Data.first*_Data.second);
	}

	//�����Z���s���B
	Matrix& Add
	(
		const Matrix& _Data
	)
	{
		this->_Add(_Data, &this->_Main, this->_Size);

		return *this;
	}

	//�����Z���s���B
	Matrix& Sub
	(
		const Matrix& _Data
	)
	{
		this->_Sub(_Data,&this->_Main,this->_Size);

		return *this;
	}

	//�X�J���[�{���s���B
	Matrix& Scalar_Mul
	(
		_T _Num
	)
	{
		this->_Scalar_mul(this->_Main,_Num);

		return *this;
	}
	
	//�s��ǂ����̏�Z���s���B
	Matrix& Mul
	(
		const Matrix& _Data
	)
	{
		std::vector<_T> _buf      = this->_Main;
		SizeT           _buf_size = this->_Size;

		this->_Mul(_buf,_buf_size,(std::vector<_T>&)_Data._Main,_Data._Size,&this->_Main,&this->_Size);

		return *this;
	}

	//�s�����ւ���B
	Matrix& Swap_Line
	(
		Ulong _Line1,
		Ulong _Line2
	) 
	{
		this->_Swap_Line(_Line1,_Line2,&this->_Main,this->_Size);

		return *this;
	}
	
	//������ւ���B
	Matrix& Swap_Column
	(
		Ulong _Column1,
		Ulong _Column2
	) 
	{
		this->_Swap_Column(_Column1, _Column2, &this->_Main, this->_Size);
		
		return *this;
	}

	//�P�ʍs��ɂ���B
	Matrix& Ident()
	{
		this->_To_Identity_Matrix(&this->_Main,this->_Size);

		return *this;
	}

	//�s�񎮂����߂܂��B 
	_T Det()
	{
		return this->_Det(this->_Main,this->_Size);
	}

	//�s��̓]�u���s���܂��B
	Matrix Transpose() 
	{
		std::vector<_T> _Data;
		_Data.resize(this->_Size.first*this->_Size.second);

		for (Ulong x = 0; x < this->_Size.first;x++)
			for (Ulong y = 0; y < this->_Size.second;y++)
				_Data[this->_Convert_to_ArrayNum(this->_Size.second, { y,x })] = this->_Main[this->_Convert_to_ArrayNum(this->_Size.first, {x,y})];
		
		return std::pair<SizeT, std::vector<_T>>{ SizeT{this->_Size.second,this->_Size.first},_Data };
	}

	//�t�s���Ԃ��܂��B
	Matrix Inverse() 
	{
		std::vector<_T> _to;
		std::vector<_T> _buf = this->_Main;

		this->_Inverse_matrix(&_buf,&_to,this->_Size);

		return std::pair<SizeT, std::vector<_T>>{ this->_Size, _to };
	}

	//�s���\�����܂��B
	Matrix& View()
	{
		this->_View(&this->_Main, this->_Size);

		return *this;
	}

	//�V�O���C�h�֐��ɒʂ��܂��B
	Matrix& Sigmoid() 
	{
		for (Ulong i = 0; i < this->_Main.size(); i++)
			this->_Main[i] = 1 / (1 + std::exp(-1 * (this->_Main[i])));

		return *this;
	}
};
#endif