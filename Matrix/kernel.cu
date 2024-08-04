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


#include <functional>
#include <iostream>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>

#define _SANAE_MATRIX_ENABLE_CUDA_

#include "Matrix/Matrix"
#include "Test.hpp"


int main() {
    MulTestCPU();
    MulTestGPU();

    try {
        // コンストラクタによる初期化
        Sanae::Matrix<double> mat1{ {0.1, 0.2, 0.3}, {0.4, 0.5, 0.6}, {0.7, 0.8, 0.9} };
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
        mat2.Resize(std::make_pair(3, 3));
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
        std::cout << "比較 (mat1 == mat3): " << std::boolalpha << isEqual << std::endl;

        bool isNotEqual = (mat1 != mat3);
        std::cout << "比較 (mat1 != mat3): " << std::boolalpha << isNotEqual << "\n" << std::endl;

        // 1行取得
        std::cout << "1行分取得 mat9:";
        auto mat9Row = mat9.GetRowRef(0);

        for (auto& Cols : mat9Row)
            std::cout << std::setw(5) << Cols.get();
        std::cout << std::endl;

        // 1列取得
        std::cout << "1列分取得 mat9:";
        auto mat9Col = mat9.GetColRef(0);

        for (auto& Rows : mat9Col)
            std::cout << std::setw(5) << Rows.get();
        std::cout << "\n" << std::endl;

        //サイズ取得
        std::cout << "サイズ mat9:行" << mat9.Rows() << " 列:" << mat9.Cols() << std::endl;
    }
    catch (InvalidMatrix& e) {
        std::cerr << "行列エラー: " << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
    }

    return 0;
}