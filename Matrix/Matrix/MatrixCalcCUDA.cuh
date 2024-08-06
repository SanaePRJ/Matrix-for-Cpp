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


#ifndef _INCLUDEGUARD_MATRIXCALC_CUDA_HPP_
#define _INCLUDEGUARD_MATRIXCALC_CUDA_HPP_


#ifdef _SANAE_MATRIX_ENABLE_CUDA_


#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "Matrix.h"


// 2次元配列を1次元配列へ変換する。
template<typename ty>
inline std::vector<ty> Sanae::Matrix<ty>::Flatten
(
	const Matrix_t<ty>& Arg
)
{
	std::vector<ty> Result;

	for (const auto& Row : Arg)
		Result.insert(Result.end(), Row.begin(), Row.end());

	return Result;
}

// 1次元配列から2次元配列へ変換する。
template<typename ty>
inline Sanae::Matrix<ty>::Matrix_t<ty> Sanae::Matrix<ty>::UnFlatten
(
	const std::vector<ty>& Arg,
	size_t Rows, 
	size_t Cols
)
{
	std::vector<std::vector<ty>> Result(Rows, std::vector<ty>(Cols));

	for (size_t i = 0; i < Rows; i++)
		std::copy(Arg.begin() + i * Cols, Arg.begin() + (i + 1) * Cols, Result[i].begin());

	return Result;
}


// 2次元から1次元に変換したポインタ,行,列サイズを格納する。
template<typename ty>
struct FlatMatrix {
	ty* MatrixP;
	size_t Row;
	size_t Col;
};


/* ------カーネル関数------ */
template<typename ty>
__global__ void AddKernel
(
	FlatMatrix<ty>       ArgData1,
	const FlatMatrix<ty> ArgData2
)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	int idx = row * ArgData1.Col + col;

	ArgData1.MatrixP[idx] += ArgData2.MatrixP[idx];
}

template<typename ty>
__global__ void SubKernel
(
	FlatMatrix<ty>       ArgData1,
	const FlatMatrix<ty> ArgData2
)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	int idx = row * ArgData1.Col + col;

	ArgData1.MatrixP[idx] -= ArgData2.MatrixP[idx];
}

template<typename ty>
__global__ void HadamardMulKernel
(
	FlatMatrix<ty>       ArgData1,
	const FlatMatrix<ty> ArgData2
)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	int idx = row * ArgData1.Col + col;

	ArgData1.MatrixP[idx] *= ArgData2.MatrixP[idx];
}

template<typename ty>
__global__ void ScalarMulKernel
(
	FlatMatrix<ty> ArgData1,
	const ty       ArgData2
) 
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	int idx = row * ArgData1.Col + col;

	ArgData1.MatrixP[idx] *= ArgData2;
}

// 乗算を行うカーネル関数
template<typename ty>
__global__ void MulKernel
(
	FlatMatrix<ty> ArgStore, 

	const FlatMatrix<ty> ArgData1,
	const FlatMatrix<ty> ArgData2
)
{
	size_t PosRow = blockIdx.y * blockDim.y + threadIdx.y;
	size_t PosCol = blockIdx.x * blockDim.x + threadIdx.x;

	if (PosRow < ArgStore.Row && PosCol < ArgStore.Col) {

		ty sum = 0;

		for (size_t i = 0; i < ArgData1.Col; i++) {
			size_t index1 = PosRow * ArgData1.Col + i;
			size_t index2 = i * ArgData2.Col + PosCol;

			sum += ArgData1.MatrixP[index1] * ArgData2.MatrixP[index2];
		}

		size_t storeIndex = PosRow * ArgStore.Col + PosCol;
		ArgStore.MatrixP[storeIndex] = sum;
	}
}


// ArgData1とArgData2に対してClacOpeCodeを実行する。
template<typename ty>
inline void Sanae::Matrix<ty>::m_CalcGPU(
	Matrix_t<ty>&       ArgData1,
	const Matrix_t<ty>& ArgData2,
	CalcOpeCode     OpeCode
) 
	const
{
	// サイズが0の場合計算できない...
	if (m_IsEmpty(ArgData1) || m_IsEmpty(ArgData2))
		throw InvalidMatrix("You must have some data.");

	// 第一第二引数の行と列数が違う場合エラー
	if (!m_IsSameSize(ArgData1, ArgData2) && OpeCode!=CalcOpeCode::ScalarMul)
		throw InvalidMatrix("The number of rows and columns of data1 and data2 must be equal.");

	// 出力サイズ
	const size_t Row = this->m_GetRowSize(ArgData1);
	const size_t Col = this->m_GetColumnSize(ArgData1);

	// GPU上での演算を行うため一次元配列へ変換する
	std::vector<ty> FlatData1 = this->Flatten(ArgData1);
	std::vector<ty> FlatData2 = this->Flatten(ArgData2);

	// GPU上のポインタ格納
	ty* d_FlatData1 = nullptr;
	ty* d_FlatData2 = nullptr;

	// エラーコードをチェックし問題があれば throw する。
	auto CheckCUDAError = [&](cudaError_t Code, const std::string& Message = "") {
		if (Code != cudaSuccess) {
			// 解放
			cudaFree(d_FlatData1);
			cudaFree(d_FlatData2);

			throw std::runtime_error(Message + cudaGetErrorString(Code));
		}
	};
	
	// GPUMem領域確保
	CheckCUDAError(cudaMalloc((void**)&d_FlatData1, FlatData1.size() * sizeof(ty)));
	CheckCUDAError(cudaMalloc((void**)&d_FlatData2, FlatData2.size() * sizeof(ty)));

	// 確保領域にコピー
	CheckCUDAError(cudaMemcpy(d_FlatData1, FlatData1.data(), FlatData1.size() * sizeof(ty), cudaMemcpyHostToDevice));
	CheckCUDAError(cudaMemcpy(d_FlatData2, FlatData2.data(), FlatData2.size() * sizeof(ty), cudaMemcpyHostToDevice));

	// サイズ情報を添付
	FlatMatrix<ty> d_ArgData1 = { d_FlatData1, Row, Col };
	FlatMatrix<ty> d_ArgData2 = { d_FlatData2, Row, Col };

	// スレッドとブロックの設定
	dim3 blocksPerGrid(static_cast<unsigned int>((Col + ThreadsPerBlock.x - 1) / ThreadsPerBlock.x), static_cast<unsigned int>((Row + ThreadsPerBlock.y - 1) / ThreadsPerBlock.y)); // グリッド内のブロック数
	
	// オペコードに基づきカーネル関数をコール
	switch (OpeCode) {
	case CalcOpeCode::Add:
		AddKernel<ty> <<<blocksPerGrid, ThreadsPerBlock >>> (d_ArgData1, d_ArgData2);
		break;

	case CalcOpeCode::Sub:
		SubKernel<ty> <<<blocksPerGrid, ThreadsPerBlock >>> (d_ArgData1, d_ArgData2);
		break;

	case CalcOpeCode::HadamardMul:
		HadamardMulKernel<ty> <<<blocksPerGrid, ThreadsPerBlock >>> (d_ArgData1, d_ArgData2);
		break;

	case CalcOpeCode::ScalarMul:
		ScalarMulKernel<ty> <<<blocksPerGrid, ThreadsPerBlock >>> (d_ArgData1, FlatData2[0]);
		break;

	default:
		// 登録されていないオペコード
		cudaFree(d_FlatData1);
		cudaFree(d_FlatData2);

		throw std::invalid_argument("Processing for the entered opcode is not available.");
		break;

	}

	//カーネル関数のエラーチェック
	cudaDeviceSynchronize();
	CheckCUDAError(cudaGetLastError());

	// 実行結果をホストへコピー
	CheckCUDAError(cudaMemcpy(FlatData1.data(), d_FlatData1, FlatData1.size() * sizeof(ty), cudaMemcpyDeviceToHost));

	// 領域開放
	cudaFree(d_FlatData1);
	cudaFree(d_FlatData2);

	// 結果を1次元から2次元配列へ変換して格納
	ArgData1 = this->UnFlatten(FlatData1, Row, Col);
}

// 加算
template<typename ty>
inline void Sanae::Matrix<ty>::m_AddGPU
(
	Matrix_t<ty>& ArgData1,
	const Matrix_t<ty>& ArgData2
)
	const
{
	this->m_CalcGPU(ArgData1, ArgData2, CalcOpeCode::Add);

	return;
}

// 減算
template<typename ty>
inline void Sanae::Matrix<ty>::m_SubGPU
(
	Matrix_t<ty>& ArgData1,
	const Matrix_t<ty>& ArgData2
)
const
{
	this->m_CalcGPU(ArgData1, ArgData2, CalcOpeCode::Sub);
	return;
}

// アダマール
template<typename ty>
inline void Sanae::Matrix<ty>::m_HadamardMulGPU
(
	Matrix_t<ty>& ArgData1,
	const Matrix_t<ty>& ArgData2
)
const
{
	this->m_CalcGPU(ArgData1, ArgData2, CalcOpeCode::HadamardMul);
	return;
}

// スカラー倍
template<typename ty>
inline void Sanae::Matrix<ty>::m_ScalarMulGPU
(
	Matrix_t<ty>& ArgData1,
	ty        ArgData2
)
const
{
	Matrix_t<ty> MatrixData2(1,std::vector<ty>(1,ArgData2));

	this->m_CalcGPU(ArgData1, MatrixData2, CalcOpeCode::ScalarMul);

	return;
}

// 行列積
template<typename ty>
inline void Sanae::Matrix<ty>::m_MulGPU
(
	Matrix_t<ty>& ArgData1,
	const Matrix_t<ty>& ArgData2
)
{
	if (this->m_GetColumnSize(ArgData1) != this->m_GetRowSize(ArgData2))
		throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");

	// 乗算後のサイズ
	const size_t Row = this->m_GetRowSize(ArgData1);
	const size_t Col = this->m_GetColumnSize(ArgData2);

	// GPU上での演算を行うため一次元配列へ変換する
	std::vector<ty> FlatData1 = this->Flatten(ArgData1);
	std::vector<ty> FlatData2 = this->Flatten(ArgData2);

	// 1次元の計算結果を格納する
	std::vector<ty> FlatResult(Row * Col, 0);

	ty* d_FlatData1 = nullptr;
	ty* d_FlatData2 = nullptr;
	ty* d_FlatResult = nullptr;

	// エラーコードをチェックし問題があれば throw する。
	auto CheckCUDAError = [&](cudaError_t Code, const std::string& Message = "") {
		if (Code != cudaSuccess) {
			// 解放
			cudaFree(d_FlatData1);
			cudaFree(d_FlatData2);
			cudaFree(d_FlatResult);

			throw std::runtime_error(Message + cudaGetErrorString(Code));
		}
		};

	// 領域確保
	CheckCUDAError(cudaMalloc((void**)&d_FlatData1, FlatData1.size() * sizeof(ty)));
	CheckCUDAError(cudaMalloc((void**)&d_FlatData2, FlatData2.size() * sizeof(ty)));
	CheckCUDAError(cudaMalloc((void**)&d_FlatResult, FlatResult.size() * sizeof(ty)));

	// 確保領域へコピー
	CheckCUDAError(cudaMemcpy(d_FlatData1, FlatData1.data(), FlatData1.size() * sizeof(ty), cudaMemcpyHostToDevice));
	CheckCUDAError(cudaMemcpy(d_FlatData2, FlatData2.data(), FlatData2.size() * sizeof(ty), cudaMemcpyHostToDevice));

	// サイズ情報を添付
	FlatMatrix<ty> d_ArgData1 = { d_FlatData1, Row, this->m_GetColumnSize(ArgData1) };
	FlatMatrix<ty> d_ArgData2 = { d_FlatData2, this->m_GetRowSize(ArgData2), Col };
	FlatMatrix<ty> d_ArgStore = { d_FlatResult, Row, Col };

	// スレッドとブロックの設定
	dim3 blocksPerGrid(static_cast<unsigned int>((Col + ThreadsPerBlock.x - 1) / ThreadsPerBlock.x), static_cast<unsigned int>((Row + ThreadsPerBlock.y - 1) / ThreadsPerBlock.y));

	MulKernel<ty> <<<blocksPerGrid, ThreadsPerBlock >>> (d_ArgStore, d_ArgData1, d_ArgData2);

	// カーネル関数のエラーチェック
	cudaDeviceSynchronize();
	CheckCUDAError(cudaGetLastError());

	CheckCUDAError(cudaMemcpy(FlatResult.data(), d_FlatResult, FlatResult.size() * sizeof(ty), cudaMemcpyDeviceToHost));

	// 領域解放
	cudaFree(d_FlatData1);
	cudaFree(d_FlatData2);
	cudaFree(d_FlatResult);

	// 結果を1次元から2次元配列へ変換して格納
	ArgData1 = this->UnFlatten(FlatResult, Row, Col);
}


#endif


#endif