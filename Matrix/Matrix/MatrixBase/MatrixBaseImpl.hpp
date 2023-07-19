/*=============================================================
* NAME      : MatrixBaseImpl.hpp
* AUTHOR    : SanaeProject
* VER       : 2.0.0
* COPYRIGHGT: Copyright 2023 SanaeProject.
* 
* 実装
* 
* 注意:
* このファイルをインクルードする前にMatrixBase.hppをインクルードしてください。
=============================================================*/


#ifndef _SANAE_MATRIXBASEIMPL_HPP_
#define _SANAE_MATRIXBASEIMPL_HPP_


#if defined(_SANAE_MATRIXBASE_HPP_ALL_INCLUDE_)


namespace Sanae {
/*------Function(protected)------*/
	/*---Converter---*/
	//{列,行}を配列番号へなおす。
	template<typename _T> Ulong _MatrixBase<_T>::_Convert_to_ArrayNum
	(
		Ulong _Width,
		SizeT _Pos
	)
	{
		return (_Width * _Pos.second) + _Pos.first;
	}
	//配列番号から{列,行}へなおす。
	template<typename _T> SizeT _MatrixBase<_T>::_Convert_to_Position
	(
		Ulong _Width,
		Ulong _Arraynum
	)
	{
		return std::make_pair(_Arraynum % _Width, (_Arraynum - (_Arraynum % _Width)) / _Width);
	}

	/*---Swap---*/
	//指定した行と行を入れ替える。
	template<typename _T> void _MatrixBase<_T>::_Swap_Line
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
	//指定した列と列を入れ替える。
	template<typename _T> void _MatrixBase<_T>::_Swap_Column
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
	template<typename _T> _MatrixBase<_T>::_MatrixBase()
	{}
	template<typename _T> _MatrixBase<_T>::_MatrixBase
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
	template<typename _T> _MatrixBase<_T>::_MatrixBase
	(
		SizeT _In_Size
	)
		: _Size(_In_Size)
	{
		this->_Main.resize(_In_Size.first * _In_Size.second);
	}
	template<typename _T> _MatrixBase<_T>::_MatrixBase
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

	/*---Operator---*/
	//等価判定
	template<typename _T> bool _MatrixBase<_T>::operator==
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

	//配列番号へアクセス
	template<typename _T> _T& _MatrixBase<_T>::operator[]
	(
		Ulong _ArrayNum
	)
	{
		if (this->_Main.size() <= _ArrayNum)
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[_ArrayNum];
	}
	//{列,行}へアクセス
	template<typename _T> _T& _MatrixBase<_T>::operator[]
	(
		SizeT _Num
	)
	{
		if (this->_Main.size() <= this->_Convert_to_ArrayNum(this->_Size.first, _Num))
			throw std::out_of_range("Tried to access out of range.");

		return this->_Main[this->_Convert_to_ArrayNum(this->_Size.first, _Num)];
	}


	/*---Calc---*/
	//加算する
	template<typename _T> void _MatrixBase<_T>::_Add
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
	//減算
	template<typename _T> void  _MatrixBase<_T>::_Sub
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
	//スカラー倍
	template<typename _T> void _MatrixBase<_T>::_Scalar_mul
	(
		std::vector<_T>& _Data,
		_T               _Mul_num
	)
	{
		for (Ulong i = 0; i < _Data.size(); i++)
			_Data[i] *= _Mul_num;

		return;
	}
	//乗算
	template<typename _T> void _MatrixBase<_T>::_Mul
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

		for (Ulong i = 0; i < _Size1.second; i++) {
			for (Ulong j = 0; j < _Size2.first; j++) {
				_T num = 0;
				for (Ulong k = 0; k < _Size2.second; k++)
					num += _Data1[this->_Convert_to_ArrayNum(_Size1.first, { k,i })] * _Data2[this->_Convert_to_ArrayNum(_Size2.first, { j,k })];
				//誤差の修正
				(*_Storage)[this->_Convert_to_ArrayNum(_SizeP->first, { j,i })] = abs(num - (Slong)num) <= _ERROR_SANAE ? (Slong)num : num;
			}
		}

		return;
	}
}


#endif
#endif