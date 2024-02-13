/*-------------------------------------------------------------
* Name    : Matrix.h
* Version : 4.0.3
* Author  : SanaePRJ
* Description:
*  MatrixBase型の定義ファイル
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
		inline bool   m_CheckColumn  (MatrixT*) const;

		inline size_t m_GetRowSize   (MatrixT*) const;
		inline size_t m_GetColumnSize(MatrixT*) const;

		inline void m_calc     (MatrixT*, const MatrixT*,std::function<ty(ty,ty)>) const;

		inline void m_add      (MatrixT*, const MatrixT*) const;
		inline void m_sub      (MatrixT*, const MatrixT*) const;
		inline void m_dotmul   (MatrixT*, const MatrixT*) const;
		inline void m_scalarmul(MatrixT*, ty      ) const;

		inline void m_mul      (MatrixT*, const MatrixT*) const;

		inline void m_to_identity(MatrixT*) const;

		inline void m_sweepout(MatrixT* arg_from, MatrixT* arg_store);
		inline ty   m_det     (MatrixT* arg);


	public:
		size_t thread = 2;

		Matrix ();
		Matrix (std::pair<size_t,size_t>);
		Matrix (MatrixInitT);
		Matrix (const MatrixT&);
		Matrix (const Matrix&);
		~Matrix();

		Matrix& operator =(MatrixInitT);
		Matrix& operator =(const Matrix&);

		Matrix& operator +=(const Matrix&);
		Matrix& operator -=(const Matrix&);
		Matrix& operator ^=(const Matrix&);
		Matrix& operator *=(const Matrix&);
		Matrix& operator *=(ty);

		Matrix  operator + (const Matrix&);
		Matrix  operator - (const Matrix&);
		Matrix  operator ^ (const Matrix&);
		Matrix  operator * (const Matrix&);
		Matrix  operator * (ty);

		Matrix& operator <<(Matrix&);
		std::vector<ty>& operator [](size_t);
		std::vector<ty*> operator [](std::pair<size_t,size_t>);

		bool    operator ==(const Matrix&);
		bool    operator !=(const Matrix&);

		size_t get_row   ();
		size_t get_column();
		
		ty     det       ();
		Matrix Inverse   ();

		Matrix& Swap_Column(size_t, size_t);
		Matrix& Swap_Row   (size_t, size_t);
		
		Matrix& resize     (std::pair<size_t,size_t>);
		Matrix& Setter     (std::function<ty()>     );

		Matrix  Transpose  ();
 	};


}




#endif