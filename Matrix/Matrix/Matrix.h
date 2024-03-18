/*-------------------------------------------------------------
* Name    : Matrix.h
* Version : 4.0.5
* Author  : SanaePRJ
* Description:
*  MatrixBase�^�̒�`�t�@�C��
-------------------------------------------------------------*/




#ifndef SANAE_MATRIX_H
#define SANAE_MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>




namespace Sanae{


	template<typename ty>
	class Matrix
	{
	private:
		//Variables
		using MatrixT     = std::vector<std::vector<ty>>;
		using MatrixInitT = std::initializer_list<std::initializer_list<ty>>;

		MatrixT matrix;


		//Private Methods
		inline bool   m_CheckColumn  (MatrixT*) const;   //�񐔂����������ǂ����m�F����B

		inline size_t m_GetRowSize   (MatrixT*) const;   //�s�����擾
		inline size_t m_GetColumnSize(MatrixT*) const;   //�񐔂��擾

		inline void m_calc     (MatrixT*, const MatrixT*,std::function<ty(ty,ty)>) const;   //std::function�Ŏ󂯎�����֐��ŉ��Z

		inline void m_add      (MatrixT*, const MatrixT*) const;   //���Z      :������ += ������
		inline void m_sub      (MatrixT*, const MatrixT*) const;   //���Z      :������ -= ������
		inline void m_dotmul   (MatrixT*, const MatrixT*) const;   //����      :������ ^= ������
		inline void m_scalarmul(MatrixT*, ty      ) const;         //�X�J���[�{:������ *= ������

		inline void m_mul      (MatrixT*, const MatrixT*)      ;   //�ρ@      :������ *= ������

		inline void m_to_identity(MatrixT*) const;                 //�P�ʍs��ɂ���B

		inline void m_sweepout(MatrixT* arg_from, MatrixT* arg_store);   //�|���o���@�ɂ��t�s������߂�B
		inline ty   m_det     (MatrixT* arg);                            //�s�񎮂����߂�B


	public:
		//�s��ςŎg�p����X���b�h��
		size_t thread = std::thread::hardware_concurrency();

		//�R���X�g���N�^
		Matrix ();
		Matrix (std::pair<size_t,size_t>);  //�T�C�Y�w��
		Matrix (MatrixInitT);               //{{0,0},{0,0}}...2*2�s��
		Matrix (const MatrixT&);            //std::vector<std::vector<ty>>
		Matrix (const Matrix&);             //�R�s�[�R���X�g���N�^
		~Matrix();                          //�f�X�g���N�^

		inline Matrix& operator =(MatrixInitT);
		inline Matrix& operator =(const Matrix&);

		inline Matrix& operator +=(const Matrix&); //���Z
		inline Matrix& operator -=(const Matrix&); //���Z
		inline Matrix& operator ^=(const Matrix&); //����
		inline Matrix& operator *=(const Matrix&); //�s���
		inline Matrix& operator *=(ty);            //�X�J���[�{

		inline Matrix  operator + (const Matrix&); //���Z
		inline Matrix  operator - (const Matrix&); //���Z
		inline Matrix  operator ^ (const Matrix&); //����
		inline Matrix  operator * (const Matrix&); //�s���
		inline Matrix  operator * (ty);            //�X�J���[�{
		template<typename CastTy>
		operator Sanae::Matrix<CastTy>();   //�L���X�g

		inline Matrix& operator <<(Matrix&);         //���n
		inline std::vector<ty>& operator [](size_t); //�v�f�փA�N�Z�X
		inline std::vector<ty*> operator [](std::pair<size_t,size_t>);

		inline bool    operator ==(const Matrix&);  //��r==
		inline bool    operator !=(const Matrix&);  //��r!=

		inline size_t get_row   ();  //�s�����擾
		inline size_t get_column();  //�񐔂��擾
		
		inline ty     det       ();  //�s�񎮂����߂�B
		inline Matrix Inverse   ();  //�t�s������߂�B

		inline Matrix& Swap_Column(size_t, size_t);  //��������Ƒ�����������ւ���B
		inline Matrix& Swap_Row   (size_t, size_t);  //�������s�Ƒ������s�����ւ���B
		
		inline Matrix& resize     (std::pair<size_t,size_t>);  //�T�C�Y��ύX����B

		inline Matrix& Setter     (std::function<ty()>);                 //�����̊֐����Ăяo���e�v�f�֕Ԃ�l��������B
		inline Matrix& Setter     (std::function<ty(size_t,size_t,ty&)>);//�����̊֐����Ăяo���e�v�f�֕Ԃ�l��������B(�s��,��,���̍s��[�s��][��])�������Ƃ��Ď󂯎��B

		inline Matrix  Transpose  ();  //�]�u����B
 	};


}




#endif