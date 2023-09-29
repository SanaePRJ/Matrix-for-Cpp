# Matrix
This repository has types that handle matrices.  

WebSite:	http://sanae.starfree.jp  
Author:		SanaeProject

# Coding Convention
    //Comment
    ReturnClass Func
    (
        Argument1,
        Argument2,
        ...
    )
    {
        //Process
    }
    
# Declaration
    
    using  Ulong = unsigned long long;
    using  Slong = signed   long long;
    using  SizeT = std::pair<Ulong, Ulong>;
    

    namespace Sanae {
	    constexpr auto IGNORE_ERROR = 1e-6;

	    template<typename T> class MatrixBase;
	    template<typename T> class MatrixT;

	    using Matrix      = MatrixT<double>;
	    using Matrix_Lite = MatrixT<float >;
    }

# SampleCode

    Sanae::Matrix a;
	
	Sanae::Matrix b =
	{
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};

	Sanae::Matrix c = b;

	//Vector type turn into Matrix type
	std::vector<double> d_v = {1,2,3,4,5,6,7,8,9};
	Sanae::Matrix       d   = std::pair<SizeT,std::vector<double>>{ {3,3},d_v };

	a += b; //a=a+b
	a -= b; //a=a-b
	a *= b; //a=a*b (Matrix)
	a *= 3; //a=a*3 (Scalar mul)
	a ^= b; //a=a*b (vector)
	a << b; //a<-b  (a.m_Main = std::move(b.m_Main))

	(b + c); //b+c 
	(b - c); //b-c
	(b * c); //b*c (Matrix)
	(b * 5); //b*5 (Scalar mul)
	(b ^ c); //b*c (vector)

	b[4];
	b[{1, 1}];


	Sanae::Matrix func_var0 =
	{
		{7,8,2},
		{6,0,2},
		{4,2,1}
	};
	

	func_var0.Swap_Column(0,1);
	func_var0.Swap_Line(1,2);

	func_var0.Transpose();
	func_var0.Det();
	func_var0.Inverse();
	