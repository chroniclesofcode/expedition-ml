#include <hip/hip_runtime.h>

#include <cstdio>
#include <vector>
#include <iostream>

__global__
void vecAddKernel(float *a, float *b, float *c, int n)
{
    size_t i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i < n)
    {
        c[i] = a[i] + b[i];
    }
}

int main(void)
{
    int N = 54;
    std::vector<float> data_h;
    for (int i = 0; i < N; i++)
    {
        data_h.push_back(static_cast<float>(i));
    }
    std::vector<float> data2_h;
    for (int i = 0; i < N; i++)
    {
        data2_h.push_back(static_cast<float>(i * 2));
    }
    std::vector<float> c_h(N);
    float *a_d, *b_d, *c_d;

    size_t sz_tot = N * sizeof(float);
    hipMalloc((void **)&a_d, sz_tot);
    hipMalloc((void **)&b_d, sz_tot);
    hipMalloc((void **)&c_d, sz_tot);

    hipMemcpy(a_d, data_h.data(), sz_tot, hipMemcpyHostToDevice);
    hipMemcpy(b_d, data2_h.data(), sz_tot, hipMemcpyHostToDevice);

    // vecAddKernel<<<ceil(N / 256.0), 256>>>(a_d, b_d, c_d, N);
    hipLaunchKernelGGL(vecAddKernel,
                   dim3((N + 255) / 256),
                   dim3(256),
                   0, 0,
                   a_d, b_d, c_d, N);

    hipMemcpy(c_h.data(), c_d, sz_tot, hipMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) std::cout << data_h[i] << ' '; std::cout << '\n';
    for (int i = 0; i < N; i++) std::cout << data2_h[i] << ' '; std::cout << '\n';

    std::cout << "Results are: ";
    for (int i = 0; i < N; i++)
    {
        std::cout << c_h[i] << ' ';
    }
    std::cout << std::endl;

}