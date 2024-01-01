# Matrix-for-Cpp目標
行列を扱う型を作成していく。

# メンバ一覧
## 宣言
### 引数なし
    Matrix<double> _var;
### 3*3行列
    Matrix _var
    {
      {1,2,3},
      {4,5,6},
      {7,8,9}
    };
### Matrix
    //3*3行列を定義
    Matrix<double> var0 =
    {
      {0,1,2},
      {3,4,5},
      {6,7,8}
    }
    
    Matrix<double> var1 = var0;
## 参照
    Matrix<double> var0 =
    {
      {0,1,2},
      {3,4,5},
      {6,7,8}
    };
    var0[1][1]; //4
## 代入
以下の変数が定義されているものとする。

    Matrix<double> var0;

### Matrix
    Matrix<double> var1 =
    {
      {1,2,3},
      {4,5,6},
      {7,8,9}
    };
    var0 = var1;
### 3*3行列
    var0 = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    }; 
## 演算
以下の変数が定義されているものとする。
    
    Matrix<double> var0 = 
    {
          {7,8,2},
		  {6,0,2},
		  {4,2,1}
    };
    
    Matrix<double> var1 =
    {
      {1,2,3},
      {4,5,6},
      {7,8,9}
    };
    
### 足し算
    //破壊的
    var0 += var1;
    
    //非破壊的
    (var0+var1);
    
### 引き算
    //破壊的
    var0 -= var1;
    
    //非破壊的
    (var0+var1);
    
### 乗算
    //破壊的
    var0 *= var1;
    
    //非破壊的
    (var0*var1);
    
### スカラー倍
    //破壊的
    var0 *= 5;
    
    //非破壊的
    (var0*5);
    
### 行列式を求める
    var0.det();

### 行列の転置
    //非破壊的処理
    var0.Transpose();
    
### 逆行列を返します
    //非破壊的処理
    var0.Inverse();
