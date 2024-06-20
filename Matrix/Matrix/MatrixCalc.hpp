/*-------------------------------------------------------------
* Name    : MatrixCalc.hpp
* Version : 4.0.5
* * Author: SanaePRJ
* Description:
*  MatrixBase�^�̌v�Z���\�b�h������
-------------------------------------------------------------*/




#ifndef _INCLUDEGUARD_MATRIXCALC_HPP_
#define _INCLUDEGUARD_MATRIXCALC_HPP_




#include <algorithm>
#include <type_traits>
#include "Matrix.h"




//�n���ꂽ�֐������ׂĂ̗v�f�ɑ΂��Ď��s���܂��Barg_data1 = Function(arg_data1,arg_data2)
template<typename ty>
template<typename FuncType>
inline void Sanae::Matrix<ty>::m_Calc
(
	Matrix_t&       ArgMatrix1,
	const Matrix_t& ArgMatrix2,

	FuncType Func
)
	const
{
	//�T�C�Y��0�̏ꍇ�v�Z�ł��Ȃ�...
	if (m_IsEmpty(ArgMatrix1) || m_IsEmpty(ArgMatrix2))
		throw InvalidMatrix("You must have some data.");

	//���������̍s�Ɨ񐔂��Ⴄ�ꍇ�G���[
	if (!m_IsSameSize(ArgMatrix1,ArgMatrix2))
		throw InvalidMatrix("The number of rows and columns of data1 and data2 must be equal.");

	const size_t Row    = this->m_GetRowSize   (ArgMatrix1);  //�v�Z����s��
	const size_t Column = this->m_GetColumnSize(ArgMatrix1);  //�v�Z�����

	//�S�v�f�ɑ΂��ď������s��
	for (size_t PosRow = 0; PosRow < Row; PosRow++)
	{
		for (size_t PosColumn = 0; PosColumn < Column; PosColumn++)
		{
			//arg_data1�Ɍv�Z���ʂ����Ă���
			ArgMatrix1[PosRow][PosColumn] = Func(ArgMatrix1[PosRow][PosColumn], ArgMatrix2[PosRow][PosColumn]);
		}
	}

	return;
}


//���Z
template<typename ty>
inline void Sanae::Matrix<ty>::m_Add
(
	Matrix_t&       ArgData1, 
	const Matrix_t& ArgData2
) 
	const
{
	this->m_Calc(ArgData1, ArgData2, std::plus<ty>());
	return;
}


//���Z
template<typename ty>
inline void Sanae::Matrix<ty>::m_Sub
(
	Matrix_t&       ArgData1, 
	const Matrix_t& ArgData2
)
	const
{
	this->m_Calc(ArgData1, ArgData2, std::minus<ty>());
	return;
}


//�A�_�}�[���ς��s���B
template<typename ty>
inline void Sanae::Matrix<ty>::m_HadamardMul
(
	Matrix_t&       ArgData1, 
	const Matrix_t& ArgData2
)
	const
{
	this->m_Calc(ArgData1, ArgData2, std::multiplies<ty>());
	return;
}


//�X�J���[�{
template<typename ty>
inline void Sanae::Matrix<ty>::m_ScalarMul
(
	Matrix_t& ArgData1, 
	ty        ArgData2
)
	const
{
	this->m_Calc(ArgData1, ArgData1, [&ArgData2](ty d1, ty d2)->ty {return d1 * ArgData2; });

	return;
}




//�}���`�X���b�h�œ��삷��̂� thread �� include ���Ă���
#ifndef SANAE_MATRIX_NOTHREADS
	#include <thread>
#endif




//�s��̊|���Z(Threads)
template<typename ty>
inline void Sanae::Matrix<ty>::m_Mul
(
	Matrix_t&       ArgData1,
	const Matrix_t& ArgData2
)
{
	//�������̗񐔂Ƒ������̍s���͓����łȂ���΂Ȃ�Ȃ��B
	if (m_GetColumnSize(ArgData1) != m_GetRowSize(ArgData2))
		throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");


	//�������̍s���Ƒ������̗񐔂��m��,0�ŏ�����
	Matrix_t Result(this->m_GetRowSize(ArgData1), Row_t<ty>(m_GetColumnSize(ArgData2), 0));

	//�s��T�C�Y���擾
	const size_t Row    = this->m_GetRowSize   (Result); //�s��
	const size_t Column = this->m_GetColumnSize(Result); //��
	
	//�v�Z����^�X�N�������߂�B
	const size_t AllTaskCount = Row * Column;

	//�Œ�1�X���b�h���g���B
	if (this->thread == 0)
		this->thread = 1;

	//1�X���b�h������̃^�X�N�����߂�B
	size_t OneTaskCount    = AllTaskCount / this->thread;

	//l[i][j] = ��k=0,n (m[i][k] * n[k][j])���v�Z�����郉���_��
	auto MulLambda = [ArgData1, ArgData2, this](size_t arg_Row, size_t arg_Column)
		{
			const size_t Size = m_GetColumnSize(ArgData1);
			ty           Sum  = 0;

			//��k = 0, n(m[i][k] * n[k][j])
			for (size_t Pos = 0; Pos < Size; Pos++)
				Sum += ArgData1[arg_Row][Pos] * ArgData2[Pos][arg_Column];

			return Sum;
		};

//�}���`�X���b�h�Ōv�Z������B
#ifndef SANAE_MATRIX_NOTHREADS

	//thread�ŕ������邽�߂̃����_��
	auto MulThread = [&Row,&Column,&Result,&MulLambda](size_t from,size_t to) {
			for (size_t pos = from; pos < to; pos++) 
				Result[(pos / Column)][(pos % Column)] = MulLambda((pos / Column),( pos % Column));
		};
	

	//�X���b�h�Ǘ�
	std::vector<std::thread> Threads;
	for (size_t Pos = 0; Pos < AllTaskCount;)
	{
		size_t from = Pos;
		size_t to   = Pos + OneTaskCount;
		
		//�^�X�N�����������ꍇ
		if (to > AllTaskCount)
			to = AllTaskCount;

		//�^�X�N�������Ȃ�
		if (from == to)
			break;

		//�X���b�h���쐬
		Threads.push_back(std::thread(MulThread,from,to));
		Pos = to;
	}

	//�^�X�N�����܂ő҂�
	for (std::thread& th:Threads)
		th.join();

//�P�Ƃ̃X���b�h�Ōv�Z������B
#else

	//�P�ƃX���b�h�Ōv�Z������B
	for (size_t PosRow = 0; PosRow < Row; PosRow++)
	{
		for (size_t PosColumn = 0; PosColumn < Column; PosColumn++)
		{
			Result[PosRow][PosColumn] = MulLambda(PosRow, PosColumn);
		}
	}

#endif

	ArgData1.clear();

	//buf��������ɏ��n
	ArgData1 = std::move(Result);

	return;
}




#endif