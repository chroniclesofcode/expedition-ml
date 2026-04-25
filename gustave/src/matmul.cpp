#include <hip/hip_runtime.h>

#include <cstdio>
#include <vector>

__global__ void matmul_kernel(const float* A, const float* B, float* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row >= N || col >= N) return;

    float sum = 0.0f;
    for (int k = 0; k < N; ++k) {
        sum += A[row * N + k] * B[k * N + col];
    }
    C[row * N + col] = sum;
}

int main() {
    constexpr int N = 64;
    constexpr size_t bytes = N * N * sizeof(float);

    std::vector<float> h_A(N * N, 1.0f);
    std::vector<float> h_B(N * N, 2.0f);
    std::vector<float> h_C(N * N, 0.0f);

    float *d_A = nullptr, *d_B = nullptr, *d_C = nullptr;
    hipMalloc(&d_A, bytes);
    hipMalloc(&d_B, bytes);
    hipMalloc(&d_C, bytes);

    hipMemcpy(d_A, h_A.data(), bytes, hipMemcpyHostToDevice);
    hipMemcpy(d_B, h_B.data(), bytes, hipMemcpyHostToDevice);

    dim3 block(16, 16);
    dim3 grid((N + block.x - 1) / block.x, (N + block.y - 1) / block.y);
    hipLaunchKernelGGL(matmul_kernel, grid, block, 0, 0, d_A, d_B, d_C, N);

    hipMemcpy(h_C.data(), d_C, bytes, hipMemcpyDeviceToHost);

    hipFree(d_A);
    hipFree(d_B);
    hipFree(d_C);

    const float expected = static_cast<float>(N) * 1.0f * 2.0f;
    std::printf("matmul: C[0,0] = %.1f (expected %.1f)\n", h_C[0], expected);
    return (h_C[0] == expected) ? 0 : 1;
}
