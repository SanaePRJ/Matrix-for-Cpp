/*=============================================================
* NAME      : MatrixImpl.hpp
* AUTHOR    : SanaeProject
* VER       : 2.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
* 
* 実装
* 
* 注意:
* このファイルをインクルードする前にMatrix.hppをインクルードしてください。
=============================================================*/


#ifndef _SANAE_MATRIXIMPL_HPP_
#define _SANAE_MATRIXIMPL_HPP_


#if defined(_SANAE_MATRIXBASE_HPP_ALL_INCLUDE_)


#include "MatrixDet.hpp"


namespace Sanae{
	/*------Function private------*/
	void Matrix::_Convert_to_Array
	(
		std::initializer_list<std::vector<double>>& _Input,
		std::vector<double>& _Storage
	)
	{
		std::vector<std::vector<double>> _bufs = _Input;

		for (Ulong i = 0; i < _bufs.size(); i++)
			_Storage.insert(_Storage.end(), _bufs[i].begin(), _bufs[i].end());
	}
	void Matrix::_View
	(
		std::vector<double>* _DataP,
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
	void Matrix::_To_Identity_Matrix
	(
		std::vector<double>* _Data,
		SizeT                _Size
	)
	{
		_Data->erase(_Data->begin(), _Data->end()); //全データの削除
		_Data->resize(_Size.first * _Size.second);    //サイズの変更

		for (Ulong i = 0; i < _Size.first; i++)
			for (Ulong j = 0; j < _Size.second; j++)
				(*_Data)[this->_Convert_to_ArrayNum(_Size.first, { i,j })] = i == j ? 1 : 0; //行と列の番号が同じ場合1それ以外の場合0

		return;
	}

	void Matrix::_Inverse_matrix
	(
		std::vector<double>* _Data,
		std::vector<double>* _Storage,
		SizeT                _Size
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
					double div = 0;

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
				double _is_zero_buf = (*_Data)[this->_Convert_to_ArrayNum(_Size.first, { _pos,_pos })];
				if (_is_zero_buf != 0)
					(*_Storage)[this->_Convert_to_ArrayNum(_Size.first, { _x,_pos })] /= _is_zero_buf;
			}
		}

		return;
	}


	/*------Function public------*/
		//代入
	Matrix& Matrix::operator=
	(
		const Matrix& _Data
	)
	{
		this->_Size = _Data._Size;
		this->_Main = _Data._Main;

		return *this;
	}
	//代入
	Matrix& Matrix::operator=
	(
		std::initializer_list<std::vector<double>> _In
	)
	{
		this->_Main.erase(this->_Main.begin(), this->_Main.end());
		std::vector<std::vector<double>> _buf = _In;

		this->_Size.second = _In.size();
		this->_Size.first = _buf[0].size();

		for (Ulong line = 0; line < this->_Size.second; line++)
			this->_Main.insert(this->_Main.end(), _buf[line].begin(), _buf[line].end());

		return *this;
	}
	Matrix& Matrix::operator+=
		(
			const Matrix& _Data
			)
	{
		this->_Add(_Data, &this->_Main, this->_Size);

		return *this;
	}

	Matrix& Matrix::operator-=
	(
		const Matrix& _Data
	)
	{
		this->_Sub(_Data, &this->_Main, this->_Size);

		return *this;
	}

	Matrix& Matrix::operator*=
	(
		const Matrix& _Data
	)
	{
		std::vector<double> _ret;
		SizeT           _ret_size;

		std::vector<double> _buf = this->_Main;
		SizeT           _buf_size = this->_Size;

		this->_Mul(_buf, _buf_size, (std::vector<double>&)_Data._Main, _Data._Size, &this->_Main, &this->_Size);

		return *this;
	}

	Matrix& Matrix::operator*=
	(
		double _Num
	)
	{
		for (Ulong i = 0; i < this->_Main.size(); i++)
			this->_Main[i] *= _Num;

		return *this;
	}
	Matrix  Matrix::operator+
	(
		const Matrix& _Data
	)
	{
		std::vector<double> _ret = this->_Main;
		this->_Add(_Data, &_ret, this->_Size);

		return std::pair<SizeT, std::vector<double>>{this->_Size, _ret};
	}
	Matrix  Matrix::operator-
	(
		const Matrix& _Data
	)
	{
		std::vector<double> _ret = this->_Main;
		this->_Sub(_Data, &_ret, this->_Size);

		return std::pair<SizeT, std::vector<double>>{this->_Size, _ret};
	}
	Matrix  Matrix::operator*
	(
		const Matrix& _Data
	)
	{
		std::vector<double> _ret;
		SizeT           _ret_size;

		std::vector<double> _buf = this->_Main;
		SizeT           _buf_size = this->_Size;

		this->_Mul(_buf, _buf_size, (std::vector<double>&)_Data._Main, _Data._Size, &_ret, &_ret_size);

		return std::pair<SizeT, std::vector<double>>{_ret_size, _ret};
	}

	Matrix  Matrix::operator*
	(
		double _Num
	)
	{
		std::vector<double> _data = this->_Main;

		for (Ulong i = 0; i < _data.size(); i++)
			_data[i] *= _Num;

		return std::pair<SizeT, std::vector<double>>{this->_Size, _data};
	}

	Matrix& Matrix::SetSize
	(
		SizeT _Data
	)
	{
		this->_Main.erase(this->_Main.begin(), this->_Main.end());
		this->_Main.resize(_Data.first * _Data.second);

		return *this;
	}
	//(破壊)行を入れ替える。
	Matrix& Matrix::Swap_Line
	(
		Ulong _Line1,
		Ulong _Line2
	)
	{
		this->_Swap_Line(_Line1, _Line2, &this->_Main, this->_Size);

		return *this;
	}
	//(破壊)列を入れ替える。
	Matrix& Matrix::Swap_Column
	(
		Ulong _Column1,
		Ulong _Column2
	)
	{
		this->_Swap_Column(_Column1, _Column2, &this->_Main, this->_Size);

		return *this;
	}
	//単位行列にする。
	Matrix& Matrix::Ident()
	{
		this->_To_Identity_Matrix(&this->_Main, this->_Size);

		return *this;
	}

	//(非破壊)行列式を求めます。 
	double Matrix::Det()
	{
		return this->_Det(this->_Main, this->_Size);
	}
	//(非破壊)行列の転置を行います。
	Matrix Matrix::Transpose()
	{
		std::vector<double> _Data;
		_Data.resize(this->_Size.first * this->_Size.second);

		for (Ulong x = 0; x < this->_Size.first; x++)
			for (Ulong y = 0; y < this->_Size.second; y++)
				_Data[this->_Convert_to_ArrayNum(this->_Size.second, { y,x })] = this->_Main[this->_Convert_to_ArrayNum(this->_Size.first, { x,y })];

		return std::pair<SizeT, std::vector<double>>{ SizeT{this->_Size.second, this->_Size.first}, _Data };
	}
	//(非破壊)逆行列を返します。
	Matrix Matrix::Inverse()
	{
		std::vector<double> _to;
		std::vector<double> _buf = this->_Main;

		this->_Inverse_matrix(&_buf, &_to, this->_Size);

		return std::pair<SizeT, std::vector<double>>{ this->_Size, _to };
	}
	//行列を表示します。
	Matrix& Matrix::View()
	{
		this->_View(&this->_Main, this->_Size);

		return *this;
	}
	//大きさを返します。
	Ulong Matrix::GetSize() {
		return this->_Main.size();
	}
	//大きさを返します。 first:列 second:行
	SizeT Matrix::GetSizeWH() {
		return this->_Size;
	}
}


#endif
#endif