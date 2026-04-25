#include <hip/hip_runtime.h>

#include <cstdio>
#include <vector>

constexpr int N = 16;

__global__ void prefix_sum_kernel(const int* in, int* out) {
    __shared__ int s[N];
    int tid = threadIdx.x;

    s[tid] = in[tid];
    __syncthreads();

    for (int offset = 1; offset < N; offset *= 2) {
        int v = (tid >= offset) ? s[tid - offset] : 0;
        __syncthreads();
        s[tid] += v;
        __syncthreads();
    }

    out[tid] = s[tid];
}

int main() {
    std::vector<int> h_in(N), h_out(N);
    for (int i = 0; i < N; ++i) h_in[i] = i + 1;

    int *d_in = nullptr, *d_out = nullptr;
    hipMalloc(&d_in, N * sizeof(int));
    hipMalloc(&d_out, N * sizeof(int));

    hipMemcpy(d_in, h_in.data(), N * sizeof(int), hipMemcpyHostToDevice);
    hipLaunchKernelGGL(prefix_sum_kernel, dim3(1), dim3(N), 0, 0, d_in, d_out);
    hipMemcpy(h_out.data(), d_out, N * sizeof(int), hipMemcpyDeviceToHost);

    hipFree(d_in);
    hipFree(d_out);

    std::printf("inclusive scan of 1..%d:\n ", N);
    for (int v : h_out) std::printf(" %d", v);
    std::printf("\n");
    return 0;
}
