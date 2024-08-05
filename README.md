# Matrix�^�̏Љ�
Matrix�^�́A�s���\�����邽�߂̃e���v���[�g�N���X�ł���A�s��̍쐬�A����A�v�Z���s�����߂̋@�\��񋟂��܂��B�ȉ���Matrix�^�̎�ȓ����Ƌ@�\��������܂��B  

WebSite:	http://sanae.starfree.jp  
Author:		SanaeProject

## ����
- **�e���v���[�g�N���X**: �v�f�̌^(`ty`)�Ɉˑ����Ē�`����邽�߁A�C�ӂ̃f�[�^�^�ōs��������܂��B
- **���I�T�C�Y**: �s���Ɨ񐔂����I�Ɍ��肳��A�T�C�Y�̕ύX�⏉�������\�ł��B
- **�s�񉉎Z�̃T�|�[�g**: ���Z�A���Z�A�X�J���[�{�A�s��ςȂǂ̉��Z����������Ă��܂��B
- **���x�ȍs��v�Z**: �t�s��̌v�Z��LU�����Ɋ�Â��s�񎮂̌v�Z�ȂǁA���x�Ȑ��w�I���삪�T�|�[�g����Ă��܂��B
- **�G���[�n���h�����O**: �s���ȍs�񑀍�ɑ΂����O�������g�ݍ��܂�Ă��܂��B
- **�����ȉ��Z**: �s��ς𕡐��̃X���b�h�Ōv�Z���邱�Ƃō����Ɍv�Z���邱�Ƃ��ł��܂��B  �܂�`CUDA`���g�p�������Z�����邱�Ƃ��ł�**���Z�A���Z�A�X�J���[�{�A�A�_�}�[���ρA�s���**�������ɉ��Z���邱�Ƃ��ł��܂��B

## ��ȃt�@�C���\��
- **Matrix.h**: Matrix�^�̒�`���܂܂�Ă���A�N���X�����o�[�A�e���v���[�g�A�s�񉉎Z�A���x�ȍs��v�Z�Ȃǂ̎������L�q����Ă��܂��B
- **MatrixUtil.hpp**: �s�񑀍�Ɋւ��郆�[�e�B���e�B�֐����錾����A�s��̊�{�I�ȑ�����T�|�[�g���܂��B
- **MatrixCalc.hpp**: �s�񉉎Z�Ɋւ���֐����錾����A���Z�A���Z�A�X�J���[�{�A�A�_�}�[���ρA�s��ςȂǂ̉��Z���`���܂��B
- **MatrixAdvCalc.hpp**: ���x�ȍs��v�Z�Ɋւ���֐����錾����A�t�s��̌v�Z�ALU�����A�s�񎮂̌v�Z�Ȃǂ��������܂��B
- **MatrixClacCUDA.cuh**: CUDA��p�������Z�A���Z�A�X�J���[�{�A�A�_�}�[���ρA�s��ςȂǂ̉��Z���`���܂��B

## ���p���@
Matrix�^�͈ȉ��̂悤�Ɏg�p�ł��܂�:
- �s��̍쐬�Ə������A�T�C�Y�̕ύX
- �s�񉉎Z�̎��s�i���Z�A���Z�A�s��ςȂǁj
- ���x�ȍs��v�Z�i�t�s��̌v�Z�A�s�񎮂̎擾�Ȃǁj

## ��O����
Matrix�^�ł͕s���ȍs�񑀍�ɑ΂����O�Ƃ���`InvalidMatrix`�N���X���g�p����A�G���[���b�Z�[�W��񋟂��܂��B

## �}���`�X���b�h
Matrix�^�ł�**�s���**���}���`�X���b�h�ŉ��Z���邱�Ƃ��ł��܂��B  
�g�p���Ȃ��ꍇ��`_SANAE_MATRIX_NOTHREADS_`��錾���܂��B  
```cpp
// �����̃X���b�h���g�p���Ȃ��ꍇ�͈ȉ���//���O���܂��B
// #define _SANAE_MATRIX_NOTHREADS_
#include "Matrix/Matrix"

int main(){
    Sanae::Matrix<double> a;
    a.thread = std::thread::hardware_concurrency(); // �g�p����X���b�h����ݒ� : �ő�l
}
```

## CUDA
Matrix�^�ł�**���Z�A���Z�A�X�J���[�{�A�A�_�}�[���ρA�s���**��GPU��Ōv�Z�����邱�Ƃɂ�荂�������邱�Ƃ��ł��܂��B  

### CUDA���g�p���Čv�Z������
Matrix���C���N���[�h����O��`_SANAE_MATRIX_ENABLE_CUDA_`��錾���܂��B
```cpp
#define _SANAE_MATRIX_ENABLE_CUDA_
#include "Matrix/Matrix"

int main(){
    Sanae::Matrix<double> a;
    a.ThreadsPerBlock = {16,16}; // �K�X�`���[�j���O�������߂��܂��B

    a.UseCUDA=true;  // ���Z��CUDA���g�p����B(�����l)
    a.UseCUDA=false; // ���Z��CUDA���g�p���Ȃ��B
}
```

## ����������
### ��
 - **CPU**
AMD Ryzen 5 5600G with Radeon Graphics 
- **������**
16.0 GB DDR4 3200
- **GPU**
NVIDIA GeForce GTX 1660
### ����
���ϒl : 0
�W���΍� : 1

### ����
#### A[2500�~2500] �~ B[2500�~2500]
- 1�X���b�h
237537[ms]
- 12�X���b�h
42962[ms] -> 5.5�{
- GPU
1840[ms] -> 23�{(CPU�݂̂Ɣ�ׂ�)

```CPU�݂̂ł�43�b������v�Z��2�b�ŏI��点�邱�Ƃ��ł���B```


#### A[10000�~10000] �~ B[10000�~10000]
- 12�X���b�h
4479320[ms]
- GPU
90190[ms] -> 50�{(CPU�݂̂Ɣ�ׂ�)

```CPU�݂̂ł�1����24��������v�Z��1��30�b�ŏI��点�邱�Ƃ��ł���B```

## �g�p��
```cpp
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




//#define _SANAE_MATRIX_NOTHREADS_

#include <functional>
#include <iostream>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>

#include "Test.hpp"
#include "Matrix/Matrix"




int main() {
    try {
        // �R���X�g���N�^�ɂ�鏉����
        Sanae::Matrix<double> mat1{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
        std::cout << "�s�� mat1:" << std::endl << mat1 << std::endl;

        Sanae::Matrix<double> mat2(std::make_pair(2, 3));
        std::cout << "�s�� mat2 (2x3):" << std::endl << mat2 << std::endl;

        Sanae::Matrix<double> mat3(mat1);
        std::cout << "�s�� mat3 (mat1�̃R�s�[):" << std::endl << mat3 << std::endl;

        // ���Z�q�ɂ�鑀��
        mat1 += mat1;
        std::cout << "�s�� mat1 ���Z�� (mat1 += mat1):" << std::endl << mat1 << std::endl;

        mat1 -= mat3;
        std::cout << "�s�� mat1 ���Z�� (mat1 -= mat3):" << std::endl << mat1 << std::endl;

        mat1 *= 2;
        std::cout << "�s�� mat1 �X�J���[�{�� (mat1 *= 2):" << std::endl << mat1 << std::endl;

        Sanae::Matrix<double> mat4 = mat1 * mat3;
        std::cout << "�s�� mat4 (mat1 * mat3):" << std::endl << mat4 << std::endl;

        // �s��̓]�u
        Sanae::Matrix<double> mat5 = mat1.Transpose();
        std::cout << "�s�� mat5 (mat1�̓]�u):" << std::endl << mat5 << std::endl;

        // �s��̃T�C�Y�ύX
        mat2.Resize(std::make_pair(3, 3));
        std::cout << "�s�� mat2 �T�C�Y�ύX�� (3x3):" << std::endl << mat2 << std::endl;

        // �P�ʍs��ƃ[���s��
        Sanae::Matrix<double> mat6 = Sanae::Matrix<double>::Identity(3);
        std::cout << "�P�ʍs�� mat6 (3x3):" << std::endl << mat6 << std::endl;

        Sanae::Matrix<double> mat7 = Sanae::Matrix<double>::Zero(3);
        std::cout << "�[���s�� mat7 (3x3):" << std::endl << mat7 << std::endl;

        // �s�񎮂Ƌt�s��
        double det = mat1.Det();
        std::cout << "�s�� mat1 �̍s��: " << det << std::endl;

        Sanae::Matrix<double> mat8 = mat1.Inverse();
        std::cout << "�s�� mat8 (mat1�̋t�s��):" << std::endl << mat8 << std::endl;

        // �L���X�g
        Sanae::Matrix<float> matFloat = mat1;
        std::cout << "�s�� matFloat (mat1����L���X�g):" << std::endl << matFloat << std::endl;

        // �A�N�Z�X
        std::cout << "�v�f mat1[1][1]: " << mat1[1][1] << std::endl;

        // ���n
        Sanae::Matrix<double> mat9{ {10, 11, 12}, {13, 14, 15}, {16, 17, 18} };
        std::cout << "�s�� mat9 (���n�O):" << std::endl << mat9 << std::endl;

        mat9 << mat1;
        std::cout << "�s�� mat9 (mat9 << mat1):" << std::endl << mat9 << std::endl;

        // ��r
        bool isEqual = (mat1 == mat3);
        std::cout << "��r (mat1 == mat3): " << std::boolalpha << isEqual << std::endl;

        bool isNotEqual = (mat1 != mat3);
        std::cout << "��r (mat1 != mat3): " << std::boolalpha << isNotEqual << "\n" << std::endl;

        // 1�s�擾
        std::cout << "1�s���擾 mat9:";
        auto mat9Row = mat9.GetRowRef(0);

        for (auto& Cols : mat9Row)
            std::cout << std::setw(5) <<  Cols.get();
        std::cout << std::endl;

        // 1��擾
        std::cout << "1�񕪎擾 mat9:";
        auto mat9Col = mat9.GetColRef(0);

        for (auto& Rows : mat9Col)
            std::cout << std::setw(5) << Rows.get();
        std::cout << "\n" << std::endl;

        //�T�C�Y�擾
        std::cout << "�T�C�Y mat9:�s" << mat9.Rows() << " ��:" << mat9.Cols() << std::endl;
    }
    catch (InvalidMatrix& e) {
        std::cerr << "�s��G���[: " << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "�G���[: " << e.what() << std::endl;
    }

    return 0;
}
```