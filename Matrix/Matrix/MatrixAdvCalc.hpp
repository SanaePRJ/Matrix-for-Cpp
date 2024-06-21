/*-------------------------------------------------------------
* Name    : MatrixDet.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  MatrixBase�^�̍s�񎮂Ƌt�s������߂�֐��̎���
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIXADV_HPP_
#define _INCLUDEGUARD_MATRIXADV_HPP_




#include "Matrix.h"




/*------------------------------------------------------
* �v���C�x�[�g���\�b�h
------------------------------------------------------*/




// �|���o���@�ɂ�苁�߂����ʂ� arg_store �Ɋi�[���܂��B
// * ArgStore ���m�ۂ��Ă���K�v�͂���܂���B
template<typename ty>
inline void Sanae::Matrix<ty>::m_SweepOut
(
	Matrix_t& ArgFrom, 
	Matrix_t& ArgStore
)
{
	//���͂��ꂽ�s�񂪐����s�񂩂ǂ������ׂ�B
	m_ValidateSquareMatrix(ArgFrom);

	//arg_from���R�s�[����B
	Matrix_t from_copy(m_GetRowSize(ArgFrom),std::vector<ty>(m_GetColumnSize(ArgFrom),0));
	//�R�s�[
	std::copy(ArgFrom.begin(), ArgFrom.end(),from_copy.begin());

	//�T�C�Y���i�[
	const size_t size = m_GetRowSize(from_copy);

	//�m��
	ArgStore.resize(size, std::vector<ty>(size, 0));
	//�P�ʍs���
	this->m_ToIdentity(ArgStore);

	//����s�̒萔�{���ق��̍s�։��Z����B
	auto Operation = [&from_copy, &ArgStore, this, size](size_t from, size_t to, ty num)
		{
			for (size_t Column = 0; Column < m_GetColumnSize(from_copy); Column++) {
				from_copy[to][Column] += from_copy[from][Column] * num;
				ArgStore [to][Column] += ArgStore [from][Column] * num;
			}
		};

	for (size_t Column = 0; Column < size; Column++) {
		for (size_t Row = 0; Row < size; Row++) {
			//�萔
			ty num = 0;

			if (Row == Column)
				num = (1 - 1 * ( from_copy)[Row][Column]) / ( from_copy)[Column][Column]; //�Ίp������1�ɂ���B
			else
				num = -1 * ( from_copy)[Row][Column] / ( from_copy)[Column][Column];      //���̐�����0�ɂ���B

			//����s�̒萔�{���ق��̍s�։��Z����B
			Operation(Column, Row, num);
		}
	}
}


// �s��:���C�v�j�b�c�̍s��
template<typename ty>
inline ty Sanae::Matrix<ty>::m_Det
(
	Matrix_t& Arg
)
{
	//�����s�񂩂ǂ������ׂ�B
	this->m_ValidateSquareMatrix(Arg);

	//�T���X�̕����ŉ����܂��B
	const auto DetBy2D = [](const Matrix_t& Matrix2D)
		{
			return Matrix2D[0][0] * Matrix2D[1][1] - Matrix2D[1][0] * Matrix2D[0][1];
		};

	//[Pos][0]-[0][Pos]���܂܂Ȃ��s��𔲂����܂��B
	const auto Extraction = [](const Matrix_t& From, Matrix_t& Storage, size_t Pos)
		{
			//�����s��i�m�͊m�肵�Ă���̂Ŏ��������߂�B
			const size_t Dim = From.size();

			for (size_t Row = 0, StorageRow = 0; Row < Dim; Row++) {
				//0�̗����Ƃ��Ă���B
				for (size_t Column = 1; Column < Dim; Column++) {
					if (Row != Pos)
						Storage[StorageRow][Column - 1] = From[Row][Column];
				}
				//��̍����ȊO�̏ꍇ�͎���
				if (Pos != Row)
					StorageRow++;
			}

			return;
		};

	//�����𗎂Ƃ��܂���B
	const auto DecDim = [this, Extraction, DetBy2D](const Matrix_t& From, ty Coeff, auto Func)
		{
			const size_t Size    = From.size(); //���̃T�C�Y
			const size_t NewSize = Size - 1;    //���Ƃ�����̃T�C�Y

			ty Result = 0; //�Ԃ�l

			//2*2�s��ɂ܂ŗ�������T���X�̕����ŉ����ĕԂ��B
			if (Size == 2)
				return Coeff * DetBy2D(From);

			//[0][0]~[n][0]�܂�
			for (size_t Position = 0; Position < Size; Position++)
			{
				Matrix_t Buffer(NewSize, std::vector<ty>(NewSize, 0)); //�i�[�p
				Extraction(From, Buffer, Position);                    //�k�������s����擾

				ty CoeffBuffer = From[Position][0] * (Position % 2 == 0 ? 1 : -1); //�V�����W��

				Result += Func(Buffer, CoeffBuffer * Coeff, Func); //�ċA�������ʂ����ׂĉ��Z
			}

			return Result;
		};
	
	//���̌W��
	const ty FromCoeff = 1;

	return DecDim(Arg, FromCoeff, DecDim);
}




/*------------------------------------------------------
* �p�u���b�N���\�b�h
------------------------------------------------------*/




// �s�񎮂����߂�B
template<typename ty>
inline ty Sanae::Matrix<ty>::Det()
{
	return this->m_Det(this->matrix);
}


// �t�s������߂�B
template<typename ty>
inline Sanae::Matrix<ty> Sanae::Matrix<ty>::Inverse()
{
	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	this->m_ValidateMatrix(this->matrix);

	//�s�񎮂�0�̏ꍇ���Ȃ�
	if (this->Det() == 0)
		throw InvalidMatrix("It is not a regular matrix.");

	Matrix_t CopyMatrix;
	std::copy(this->matrix.begin(), this->matrix.end(), std::back_inserter(CopyMatrix));

	Matrix_t Inv;
	this->m_SweepOut(CopyMatrix, Inv);

	return Inv;
}




#endif