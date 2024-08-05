/* ---------------------------------------------------------------------------------------------
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 --------------------------------------------------------------------------------------------- */


// Include guard
#ifndef _INCLUDEGUARD_MATRIX_H_
#define _INCLUDEGUARD_MATRIX_H_


// Include
#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <thread>
#include <iomanip>


// CUDA
#ifdef _SANAE_MATRIX_ENABLE_CUDA_


	#include <cuda_runtime.h>
	#include <device_launch_parameters.h>


#endif


namespace Sanae{


	template<typename ty> class Matrix{
	private:
		enum class CalcOpeCode;

		template<typename RowType    > using Row_t     = std::vector<RowType>;
		template<typename RowInitType> using RowInit_t = std::initializer_list<RowInitType>;

		template<typename MatrixType = ty> using Matrix_t     = std::vector          <Row_t<MatrixType>>;
		template<typename MatrixType = ty> using MatrixInit_t = std::initializer_list<RowInit_t<MatrixType>>;

	
		// Variables
		Matrix_t<ty> matrix;


		// MatrixUtil.hpp
		// �R���X�g���N�^��=���Z�q�ȂǂŎ��s����B
		inline void   m_ValidateMatrix(const Matrix_t<ty>&) const; //���ׂĂ̍s�ɉ����ė񐔂����������m�F����B@�񐔂��������Ȃ��ꍇ throw ���܂��B
		
		inline bool   m_IsSquareMatrix(const Matrix_t<ty>&) const; //�����s�񂩂ǂ������擾����B
		inline bool   m_IsEmpty       (const Matrix_t<ty>&) const; //��̍s�񂩂ǂ�����Ԃ��B
		inline bool   m_IsSameSize    (const Matrix_t<ty>&,const Matrix_t<ty>&) const; //�����傫�����ǂ�����Ԃ��B

		inline size_t m_GetRowSize   (const Matrix_t<ty>&) const; //�s�����擾
		inline size_t m_GetColumnSize(const Matrix_t<ty>&) const; //�񐔂��擾

		
		// MatrixCalc.hpp
		template<typename FuncType>
		inline void m_Calc       (Matrix_t<ty>&, const Matrix_t<ty>&,FuncType); // ���ׂĂ̗v�f�ő�����=FuncType(������,������)�����s����B

		inline void m_Add        (Matrix_t<ty>&, const Matrix_t<ty>&); // ���Z        : ������ += ������
		inline void m_Sub        (Matrix_t<ty>&, const Matrix_t<ty>&); // ���Z        : ������ -= ������
		inline void m_ScalarMul  (Matrix_t<ty>&, ty                 ); // �X�J���[�{  : ������ *= ������
		inline void m_HadamardMul(Matrix_t<ty>&, const Matrix_t<ty>&); // �A�_�}�[����: ������ ^= ������

		inline void m_Mul        (Matrix_t<ty>&, const Matrix_t<ty>&);       // �ρ@        : ������ *= ������


		// MatrixCalcCUDA.hpp
#ifdef _SANAE_MATRIX_ENABLE_CUDA_

		static std::vector<ty> Flatten  (const Matrix_t<ty>&);
		static Matrix_t<ty>    UnFlatten(const std::vector<ty>&, size_t, size_t);


		inline void m_CalcGPU(Matrix_t<ty>&, const Matrix_t<ty>&, CalcOpeCode) const;

		inline void m_AddGPU(Matrix_t<ty>&, const Matrix_t<ty>&) const;         // ���Z        :������ += ������
		inline void m_SubGPU(Matrix_t<ty>&, const Matrix_t<ty>&) const;         // ���Z        :������ -= ������
		inline void m_ScalarMulGPU  (Matrix_t<ty>&, ty     ) const;             // �X�J���[�{  :������ *= ������
		inline void m_HadamardMulGPU(Matrix_t<ty>&, const Matrix_t<ty>&) const; // �A�_�}�[����:������ ^= ������
		inline void m_MulGPU(Matrix_t<ty>&, const Matrix_t<ty>&);               // �ρ@        :������ *= ������

#endif


		// MatrixAdvCalc.hpp
		// �폜�\��
		inline void  m_SweepOut(Matrix_t<ty>& arg_from, Matrix_t<ty>& arg_store); // �|���o���@�ɂ��t�s������߂�B
		inline ty    m_Det     (Matrix_t<ty>& arg);                               // �s�񎮂����߂�B

		inline void  m_LUDecomposition(const Matrix_t<ty>&, Matrix_t<ty>&, Matrix_t<ty>&) const; // LU�������s��������L,��O����U�Ɋi�[����B
		
		inline ty    m_DetByU (const Matrix_t<ty>&) const;                       // LU�����ɂ���ċ��߂�U(��O�p�s��)�ɂ���čs�񎮂����߂�B
		inline void  m_Inverse(const Matrix_t<ty>&,Matrix_t<ty>&,ty=1e-5) const; // �t�s������߂�B
	

	public:

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

		enum class CalcOpeCode { Add, Sub, HadamardMul, ScalarMul };
		dim3 ThreadsPerBlock   {16,16};  // �����l��{16,16}�ł����K�X�`���[�j���O���Ă��������B

#endif

		// ���Z����CUDA���g�p����B
		bool UseCUDA = true;

		// �s��ςŎg�p����X���b�h��(�����l�͍ő�̃X���b�h��)
		size_t thread = std::thread::hardware_concurrency();


		// MatrixConstructor.hpp
		Matrix ();
		Matrix (std::pair<size_t,size_t>); // �T�C�Y�w��
		Matrix (MatrixInit_t<ty>);         // {{0,0},{0,0}}...2*2�s��
		Matrix (const Matrix_t<ty>&);      // std::vector<std::vector<ty>>
		Matrix (const Matrix&);            // �R�s�[�R���X�g���N�^
		~Matrix();                         // �f�X�g���N�^


		//MatrixOperator.hpp
		inline Matrix& operator =(MatrixInit_t<ty>);
		inline Matrix& operator =(const Matrix&);
		inline Matrix& operator =(Matrix&&);

		inline Matrix& operator +=(const Matrix&); // ���Z
		inline Matrix& operator -=(const Matrix&); // ���Z
		inline Matrix& operator ^=(const Matrix&); // ����
		inline Matrix& operator *=(const Matrix&); // �s���
		inline Matrix& operator *=(ty);            // �X�J���[�{

		inline Matrix  operator + (const Matrix&); // ���Z
		inline Matrix  operator - (const Matrix&); // ���Z
		inline Matrix  operator ^ (const Matrix&); // ����
		inline Matrix  operator * (const Matrix&); // �s���
		inline Matrix  operator * (ty);            // �X�J���[�{

		template<typename CastTy> operator Sanae::Matrix<CastTy>(); // �L���X�g

		inline Matrix& operator <<(Matrix&);         // ���n
		inline std::vector<ty>& operator [](size_t); // �v�f�փA�N�Z�X

		inline bool    operator ==(const Matrix&); // ��r ==
		inline bool    operator !=(const Matrix&); // ��r !=


		// MatrixUtil.hpp
		inline size_t Rows(); // �s�����擾
		inline size_t Cols(); // �񐔂��擾

		inline Matrix& SwapRow(size_t, size_t);    // �������s�Ƒ������s�����ւ���B
		inline Matrix& SwapColumn(size_t, size_t); // ��������Ƒ�����������ւ���B

		inline auto GetRowRef(size_t) -> std::vector<std::reference_wrapper<ty>>; // 1�s���̃f�[�^���Q�Ƃ���ϒ��z���Ԃ��B
		inline auto GetColRef(size_t) -> std::vector<std::reference_wrapper<ty>>; // 1�񕪂̃f�[�^���Q�Ƃ���ϒ��z���Ԃ��B

		inline Matrix& Resize(std::pair<size_t, size_t>); // �T�C�Y��ύX����B

		inline Matrix& Setter     (std::function<ty()>);                  // �����̊֐����Ăяo���e�v�f�֕Ԃ�l��������B
		inline Matrix& Setter     (std::function<ty(size_t,size_t,ty&)>); // �����̊֐����Ăяo���e�v�f�֕Ԃ�l��������B(�s��,��,���̍s��[�s��][��])�������Ƃ��Ď󂯎��B

		inline Matrix  Transpose  (); // �]�u����B

		static inline Matrix<ty> Identity(size_t); // �w�肳�ꂽ�s=�񐔂̒P�ʍs����쐬���܂��B 
		
		static inline Matrix<ty> Zero    (size_t); // �w�肳�ꂽ�s,�񐔂�0�s����쐬���܂��B @ �����s��
		static inline Matrix<ty> Zero    (size_t, size_t);


		// MatrixAdvCalc.hpp
		inline auto Det() -> typename std::enable_if<std::is_floating_point<ty>::value,double>::type;                // �s�񎮂����߂�B
		inline auto Inverse(ty=1e-5) -> typename std::enable_if<std::is_floating_point<ty>::value,Matrix<ty>>::type; // �t�s������߂�B

 	};

}


// �����ȍs���������O�N���X
class InvalidMatrix : std::exception{
protected:

	std::string m_ErrorMessage = "It is an invalid matrix.";

public:

	InvalidMatrix() {}
	
	// ���b�Z�[�W�����
	InvalidMatrix(const char* ArgMessage) : m_ErrorMessage(ArgMessage) {}

	// �G���[���b�Z�[�W��Ԃ��B
	const char* what() const noexcept override {return m_ErrorMessage.c_str();}

};


namespace Sanae {

	// �s��\���p
	const static std::streamsize DefaultWeight = 4;
	static       std::streamsize FontWeight = DefaultWeight;

}


// std::cout�ŏo��
template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<
(
	std::basic_ostream<CharT, Traits>& ArgOstream,
	Sanae::Matrix<MatrixType> Matrix
)
{
	for (size_t Row = 0; Row < Matrix.Rows(); Row++) {
		for (MatrixType Column : Matrix[Row])
			ArgOstream << std::setw(Sanae::FontWeight) << Column;

		ArgOstream << std::endl;
	}

	return ArgOstream;
}


#endif