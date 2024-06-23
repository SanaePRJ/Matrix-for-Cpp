/*-------------------------------------------------------------
* Name    : Matrix.cpp
* Version : 4.0.5
* Author  : SanaePRJ
* Description:
*  テストプログラム
-------------------------------------------------------------*/




//#define SANAE_MATRIX_NOTHREADS

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
        // コンストラクタによる初期化
        Sanae::Matrix<double> mat1{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
        std::cout << "行列 mat1:" << std::endl << mat1 << std::endl;

        Sanae::Matrix<double> mat2(std::make_pair(2, 3));
        std::cout << "行列 mat2 (2x3):" << std::endl << mat2 << std::endl;

        Sanae::Matrix<double> mat3(mat1);
        std::cout << "行列 mat3 (mat1のコピー):" << std::endl << mat3 << std::endl;

        // 演算子による操作
        mat1 += mat1;
        std::cout << "行列 mat1 加算後 (mat1 += mat1):" << std::endl << mat1 << std::endl;

        mat1 -= mat3;
        std::cout << "行列 mat1 減算後 (mat1 -= mat3):" << std::endl << mat1 << std::endl;

        mat1 *= 2;
        std::cout << "行列 mat1 スカラー倍後 (mat1 *= 2):" << std::endl << mat1 << std::endl;

        Sanae::Matrix<double> mat4 = mat1 * mat3;
        std::cout << "行列 mat4 (mat1 * mat3):" << std::endl << mat4 << std::endl;

        // 行列の転置
        Sanae::Matrix<double> mat5 = mat1.Transpose();
        std::cout << "行列 mat5 (mat1の転置):" << std::endl << mat5 << std::endl;

        // 行列のサイズ変更
        mat2.ReSize(std::make_pair(3, 3));
        std::cout << "行列 mat2 サイズ変更後 (3x3):" << std::endl << mat2 << std::endl;

        // 単位行列とゼロ行列
        Sanae::Matrix<double> mat6 = Sanae::Matrix<double>::Identity(3);
        std::cout << "単位行列 mat6 (3x3):" << std::endl << mat6 << std::endl;

        Sanae::Matrix<double> mat7 = Sanae::Matrix<double>::Zero(3);
        std::cout << "ゼロ行列 mat7 (3x3):" << std::endl << mat7 << std::endl;

        // 行列式と逆行列
        double det = mat1.Det();
        std::cout << "行列 mat1 の行列式: " << det << std::endl;

        Sanae::Matrix<double> mat8 = mat1.Inverse();
        std::cout << "行列 mat8 (mat1の逆行列):" << std::endl << mat8 << std::endl;

        // キャスト
        Sanae::Matrix<float> matFloat = mat1;
        std::cout << "行列 matFloat (mat1からキャスト):" << std::endl << matFloat << std::endl;

        // アクセス
        std::cout << "要素 mat1[1][1]: " << mat1[1][1] << std::endl;

        // 譲渡
        Sanae::Matrix<double> mat9{ {10, 11, 12}, {13, 14, 15}, {16, 17, 18} };
        std::cout << "行列 mat9 (譲渡前):" << std::endl << mat9 << std::endl;

        mat9 << mat1;
        std::cout << "行列 mat9 (mat9 << mat1):" << std::endl << mat9 << std::endl;

        // 比較
        bool isEqual = (mat1 == mat3);
        std::cout << "比較 (mat1 == mat3): " << (isEqual ? "true" : "false") << std::endl;

        bool isNotEqual = (mat1 != mat3);
        std::cout << "比較 (mat1 != mat3): " << (isNotEqual ? "true" : "false") << std::endl;

    }
    catch (InvalidMatrix& e) {
        std::cerr << "行列エラー: " << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
    }

    return 0;
}