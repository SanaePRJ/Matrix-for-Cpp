# Matrix型の紹介
Matrix型は、行列を表現するためのテンプレートクラスであり、行列の作成、操作、計算を行うための機能を提供します。以下にMatrix型の主な特徴と機能を説明します。  

WebSite:	http://sanae.starfree.jp  
Author:		SanaeProject

- この説明文は`ChatGpt(Gpt3.5)`により生成しました。

## 特徴
- **テンプレートクラス**: 要素の型(`ty`)に依存して定義されるため、任意のデータ型で行列を扱えます。
- **動的サイズ**: 行数と列数が動的に決定され、サイズの変更や初期化が可能です。
- **行列演算のサポート**: 加算、減算、スカラー倍、行列積などの演算が実装されています。
- **高度な行列計算**: 逆行列の計算やLU分解に基づく行列式の計算など、高度な数学的操作がサポートされています。
- **エラーハンドリング**: 不正な行列操作に対する例外処理が組み込まれています。

## 主なファイル構成
- **Matrix.h**: Matrix型の定義が含まれており、クラスメンバー、テンプレート、行列演算、高度な行列計算などの実装が記述されています。
- **MatrixUtil.hpp**: 行列操作に関するユーティリティ関数が宣言され、行列の基本的な操作をサポートします。
- **MatrixCalc.hpp**: 行列演算に関する関数が宣言され、加算、減算、スカラー倍、アダマール積、行列積などの演算を定義します。
- **MatrixAdvCalc.hpp**: 高度な行列計算に関する関数が宣言され、逆行列の計算、LU分解、行列式の計算などを実装します。

## 利用方法
Matrix型は以下のように使用できます:
- 行列の作成と初期化、サイズの変更
- 行列演算の実行（加算、減算、行列積など）
- 高度な行列計算（逆行列の計算、行列式の取得など）

## 例外処理
Matrix型では不正な行列操作に対する例外として`InvalidMatrix`クラスが使用され、エラーメッセージを提供します。

## 使用例
```cpp
#include "Matrix.h"

using namespace Sanae;

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