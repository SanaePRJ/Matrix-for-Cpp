#pragma once

#include <functional>
#include <iostream>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>

#include "Matrix/Matrix"




//行列積高速化テストプログラム
/*
*
* 環境:Core i5 8250U,DDR4(2400Mhz)
* 実験結果:
*
計算回数  :高速化率が最も高いスレッド数
1000000   :3,4,5,6スレッド
125000000 :8スレッド
343000000 :7スレッド
1000000000:4スレッド
3375000000:4スレッド
8000000000:4スレッド
*/
static void MulTestCPU()
{
	using namespace std::chrono;

	std::default_random_engine       engine;      //エンジン
	std::normal_distribution<double> dist(0, 1);  //平均0,標準偏差1

	size_t n = 2500;

	//n*n行列を作成
	Sanae::Matrix<double> buf0 = std::pair<size_t, size_t>{ n,n };
	Sanae::Matrix<double> buf1 = std::pair<size_t, size_t>{ n,n };

	buf0.Setter([&engine, &dist]()->double {return dist(engine); });
	buf1.Setter([&engine, &dist]()->double {return dist(engine); });

	auto calc = [&buf0, &buf1, &engine, &dist]() {
		auto buf_time0 = system_clock::now();

		buf0* buf1;

		auto buf_time1 = system_clock::now();

		return duration_cast<milliseconds>(buf_time1 - buf_time0).count();
	};

	std::cout << n * n * n << "回計算されます。" << std::endl;

	//1スレッドでの計算
	buf0.thread = 1;
	std::cout << buf0.thread << "スレッドでの計算:";
	double time1 = static_cast<double>(calc());
	std::cout << time1 << "ミリ秒かかりました。" << std::endl;

	//複数スレッドでの計算
	for (size_t i = 2; i <= std::thread::hardware_concurrency(); i++) {
		buf0.thread = i;

		std::cout << buf0.thread << "スレッドでの計算:";
		double time2 = static_cast<double>(calc());
		std::cout << time2 << "ミリ秒かかりました。->";

		std::cout << (1 - time2 / time1) * 100 << "%高速化できました。" << std::endl;
	}
	system("pause");
}

#ifdef _SANAE_MATRIX_ENABLE_CUDA_

static void MulTestGPU() {
	using namespace std::chrono;

	std::default_random_engine       engine;      //エンジン
	std::normal_distribution<double> dist(0, 1);  //平均0,標準偏差1

	size_t n = 2500;

	//n*n行列を作成
	Sanae::Matrix<double> buf0 = std::pair<size_t, size_t>{ n,n };
	Sanae::Matrix<double> buf1 = std::pair<size_t, size_t>{ n,n };

	buf0.Setter([&engine, &dist]()->double {return dist(engine); });
	buf1.Setter([&engine, &dist]()->double {return dist(engine); });

	auto calc = [&buf0, &buf1, &engine, &dist]() {
		auto buf_time0 = system_clock::now();

		buf0* buf1;

		auto buf_time1 = system_clock::now();

		return duration_cast<milliseconds>(buf_time1 - buf_time0).count();
		};

	std::cout << n * n * n << "回計算されます。" << std::endl;

	std::cout << "GPU演算テスト" << std::endl;
	buf0.UseCUDA = false;
	double time1 = static_cast<double>(calc());
	std::cout << "CPU-"<< buf0.thread << "スレッドでの計算:" << time1 << "ミリ秒かかりました。" << std::endl;

	buf0.UseCUDA = true;
	double time2 = static_cast<double>(calc());
	std::cout << "GPUでは" << time2 << "ミリ秒かかりました。" << std::endl;
	std::cout << (1 - time2 / time1) * 100 << "%高速化できました。" << std::endl;

	system("pause");
}

#endif