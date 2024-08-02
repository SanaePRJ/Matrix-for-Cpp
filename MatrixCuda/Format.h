// このファイルはライブラリの記法を定めるものです。
// このリポジトリのプログラムは以下の記法により書かれます。

// 関数名などはパスカルケースによる命名とする。


// ------書式------
#ifndef _INCLUDEGUARD_ファイル名_拡張子_
#define _INCLUDEGUARD_ファイル名_拡張子_


// インクルードエリア
#include <iostream>
// include(山かっこ形式)...

#include "iostream"  
// include(引用符形式)


// Proc...
class Test {
private:

// private var
// type varname = 初期値;

// private method
// プロトタイプ宣言
// inline 返り値の型 m_関数名(引数の型1,引数の型2...); // 関数の説明
// 
// 実装
// // 関数の説明
// inline 返り値の型 m_関数名
// (
//		引数1,
//		引数2
// )

public:

// public var
// type wername = 初期値;

// public method
// プロトタイプ宣言
// inline 返り値の型 関数名(引数の型1,引数の型2...); // 関数の説明
// 
// 実装
// // 関数の説明
// inline 返り値の型 関数名
// (
//		引数1,
//		引数2
// )

};


#endif