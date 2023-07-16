/*=============================================================
* NAME      : MatrixDet.hpp
* AUTHOR    : SanaeProject
* VER       : 2.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
* 
* 行列式実装用
* 
* このファイルはMatrixImpl.hppによってインクルードされます。
=============================================================*/


#ifndef _SANAE_MATRIXDET_HPP_
#define _SANAE_MATRIXDET_HPP_


#if defined(_SANAE_MATRIXBASE_HPP_ALL_INCLUDE_)


namespace Sanae {


	//2*2の行列式を求めます。
	double Matrix::_Det_2
	(
		std::vector<double>* _DataP,
		double               _Coeff,
		SizeT                _Size
	)
	{
		if (_Size.first != _Size.second)
			throw std::invalid_argument("Must be same column and line.");

		if (_Size.first != 2)
			throw std::invalid_argument("Must pass 2 as argument.");

		return _Coeff * (((*_DataP)[this->_Convert_to_ArrayNum(2, { 0,0 })] * (*_DataP)[this->_Convert_to_ArrayNum(2, { 1,1 })]) - ((*_DataP)[this->_Convert_to_ArrayNum(2, { 1,0 })] * (*_DataP)[this->_Convert_to_ArrayNum(2, { 0,1 })]));
	}

	//余因子展開をして行列を一次元下げます。
	void Matrix::_Confactor_expansion_1
	(
		std::vector<double>& _Data,
		SizeT                             _Size,
		std::vector<std::vector<double>>* _Storage,
		std::vector<double>* _Coeff
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
	//2*2行列になるまで余因子展開をします。
	std::pair<std::vector<std::vector<double>>, std::vector<double>>
		Matrix::_Confactor_expansion_to_2
		(
			std::vector<double>& _Data,
			SizeT                _Size,
			double               _In_Coeff = 1
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

	//行列式を求めます。
	double Matrix::_Det
	(
		std::vector<double>& _Data,
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
}


#endif
#endif