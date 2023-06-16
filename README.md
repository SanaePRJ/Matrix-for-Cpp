# Matrix-for-Cpp目標
行列を扱う型を作成していく。

# Coding convention
関数の書き方を以下のように定める。
    
    //Comment
    ReturnClass Func
    (
        Argument1,
        Argument2,
        ...
    )
    {
        Process
    }

# メンバ一覧
## 宣言
### 引数なし
    Matrix<double> _var;
### 配列
    std::vector<double> var0 = {0,1,2,3};
    Matrix<double>      var1 = std::pair<SizeT,std::vector<double>>{ {2,2},var0 };
	  var1.View();
### 3*3行列
    Matrix<double> _var
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
    var0[4];    //4  [配列番号]
    var0[{1,1}] //4  [{列番号,行番号}]
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
    var0 += var1; //var0.Add(var1);
    
    //非破壊的
    (var0+var1);
    
### 引き算
    //破壊的
    var0 -= var1; //var0.Sub(var1);
    
    //非破壊的
    (var0+var1);
    
### 乗算
    //破壊的
    var0 *= var1; //var0.Mul(var1);
    
    //非破壊的
    (var0*var1);
    
### スカラー倍
    //破壊的
    var0 *= 5; //var0.Scalar_Mul(5);
    
    //非破壊的
    (var0*5);
    
### 行列式を求める
    var0.Det(); //12
    
### 行列の転置
    //非破壊的処理
    var0.Transpose();
    
### 逆行列を返します
    //非破壊的処理
    var0.Inverse();
    
### シグモイド関数
    //破壊的処理
    var0.Sigmoid();
