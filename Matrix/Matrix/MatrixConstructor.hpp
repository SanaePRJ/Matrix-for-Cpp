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


#ifndef _INCLUDEGUARD_MATRIXCONSTRUCTOR_HPP_
#define _INCLUDEGUARD_MATRIXCONSTRUCTOR_HPP_


#include <algorithm>
#include "Matrix.h"


//�R���X�g���N�^
template<typename ty>
Sanae::Matrix<ty>::Matrix() {}

//�T�C�Y���w�肵��0�s��𐶐�����B
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	std::pair<size_t,size_t> ArgSize
) 
	:matrix(ArgSize.first, std::vector<ty>(ArgSize.second, 0))
{}

//�ȉ��̂悤�ɒ�`���邱�Ƃ��ł���B
//{
//	{0, 0},
//	{0, 0},
//	{0, 0}
//}
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	MatrixInit_t<ty> ArgInitValue
)
	:matrix(ArgInitValue.begin(), ArgInitValue.end())
{
	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	this->m_ValidateMatrix(this->matrix);
}

//std::vector<std::vector<ty>>
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	const Matrix_t<ty>& ArgInitValue
)
	:matrix(ArgInitValue.begin(), ArgInitValue.end())
{
	//�񐔂͓������Ȃ���΂Ȃ�Ȃ��B
	this->m_ValidateMatrix(this->matrix);
}

//�R�s�[�R���X�g���N�^
template<typename ty>
Sanae::Matrix<ty>::Matrix
(
	const Matrix& ArgMatrix
)
{
	//�T�C�Y�ݒ�
	matrix.resize(ArgMatrix.matrix.size());
	//�R�s�[
	std::copy(ArgMatrix.matrix.begin(),ArgMatrix.matrix.end(),matrix.begin());
}

//�f�X�g���N�^
template<typename ty>
inline Sanae::Matrix<ty>::~Matrix()
{
}


#endif